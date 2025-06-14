/*
 * File: task_scheduler.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include "task_scheduler.h"
#include "task_worker.h"

#include <condition_variable>
#include <thread>


namespace dandelion {


task_scheduler::task_scheduler(int num_worker)
    : m_queues(num_worker)
    , m_workers(num_worker)
    , m_suspend_queues()
    , m_last_pushed_worker(0)
{
    for (int i = 0; i < num_worker; ++i) {
        m_workers[i] = std::make_unique<task_worker>(*this, i);
        std::thread th([worker = m_workers[i].get()] {
            worker->execute();
        });
        m_threads.push_back(std::move(th));
    }

    while (true) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_suspend_queues.size() == num_worker)
            break;
        std::this_thread::yield();
    }
}

task_scheduler::~task_scheduler() {
    for (auto& worker : m_workers)
        worker->stop();

    for (auto& t : m_threads)
        t.join();
}

std::unique_ptr<task> task_scheduler::schedule(int worker_id) noexcept {
    std::unique_ptr<task> work = nullptr;

    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_queues[worker_id].empty()) {
        // Steal work from other queues
        for (int i = 0; i < m_queues.size(); ++i) {
            if (i == worker_id)
                continue;

            work = m_queues[i].steal();
            if (work)
                break;
        }
    }
    else {
        work = m_queues[worker_id].pop();
    }

    return work;
}


void task_scheduler::submit(task&& t) noexcept {
    auto new_task = std::make_unique<task>();
    *new_task = std::move(t);

    submit(std::move(new_task));
}

void task_scheduler::submit(std::unique_ptr<task> t) noexcept {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_suspend_queues.empty()) {
        int worker_id = m_suspend_queues.front();
        m_suspend_queues.pop();
        m_queues[worker_id].push(std::move(t));
        m_workers[worker_id]->wakeup();
        return;
    }

    // Submit to the worker with the least amount of work
    m_queues[m_last_pushed_worker].push(std::move(t));
    m_last_pushed_worker = (m_last_pushed_worker + 1) % m_queues.size();
}


void task_scheduler::submit(std::unique_ptr<task> t, int worker_id) noexcept {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queues[worker_id].push(std::move(t));
}


void task_scheduler::suspend(int worker_id) noexcept {
    spdlog::info("suspend worker: {}", worker_id);

    std::lock_guard<std::mutex> lock(m_mutex);
    
    m_suspend_queues.push(worker_id);
}


void task_scheduler::wait_finish() noexcept {
    std::condition_variable wait_finish_cv;
    bool finish = false;

    while (true) {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        // Check if all workers are suspended
        if (m_suspend_queues.size() == m_queues.size())
            break;

        auto sig_task = [&]()->task {
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                finish = true;
            }
            wait_finish_cv.notify_one();
            co_return;
        }();

        // Create a task which invokes wait_finish_cv
        for (int i = 0; i < m_queues.size(); ++i) {
            if (!m_queues[i].empty()) {
                m_queues[i].push(std::make_unique<task>(std::move(sig_task)));
                break;
            }
        }

        // Future & Promise pattern might be better
        wait_finish_cv.wait(lock, [&] { return finish; });
    }
}

} // namespace dandelion
