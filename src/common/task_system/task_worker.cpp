/*
 * File: task_worker.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include "task_worker.h"
#include "task_scheduler.h"

namespace dandelion {

task_worker::task_worker(task_scheduler& scheduler, int worker_id)
    : m_scheduler(scheduler)
    , m_worker_id(worker_id)
    , m_running(true) {
}

task_worker::~task_worker() {
    stop();
}


void task_worker::execute() noexcept {
    while (m_running) {
        if (auto t = fetch()) {
            (*t)();

            if (*t)
                push(std::move(t));
        }
        else
            wait();
    }
}

void task_worker::wait() noexcept {
    m_scheduler.suspend(m_worker_id);

    std::unique_lock<std::mutex> lock(m_wait_mutex);
    m_wait_cv.wait(lock);
}

void task_worker::wakeup() noexcept {
    m_wait_cv.notify_one();
}


void task_worker::push(std::unique_ptr<task> t) noexcept {
    m_scheduler.submit(std::move(t), m_worker_id);
}

std::unique_ptr<task> task_worker::fetch() noexcept {
    return m_scheduler.schedule(m_worker_id);    
}



} // namespace dandelion
