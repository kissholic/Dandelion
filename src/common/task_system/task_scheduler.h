/*
 * File: task_scheduler.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include "task.h"
#include "task_queue.h"

#include <mutex>
#include <queue>
#include <vector>


namespace dandelion {

class task_worker;

class task_scheduler {
public:
    // TODO: M : N mapping between workers and queues
    task_scheduler(int num_worker);
    ~task_scheduler();

    task_scheduler(task_scheduler const&) = delete;
    task_scheduler& operator=(task_scheduler const&) = delete;

    // Schedule a task to a worker
    [[nodiscard]]
    std::unique_ptr<task> schedule(int worker_id) noexcept;

    void submit(task&& t) noexcept;
    void submit(std::unique_ptr<task> t) noexcept;
    void submit(std::unique_ptr<task> t, int worker_id) noexcept;

    void suspend(int worker_id) noexcept;

    void wait_finish() noexcept;

private:
    void wakeup() noexcept;

private:
    std::vector<task_queue> m_queues;
    std::vector<std::unique_ptr<task_worker>> m_workers;
    std::vector<std::thread> m_threads;

    // m_mutex is duplicated with m_queues
    std::mutex m_mutex;
    std::queue<int> m_suspend_queues;
    int m_last_pushed_worker;

};

} // namespace dandelion
