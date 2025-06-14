/*
 * File: task_worker.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include "task.h"
#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>


namespace dandelion {

class task_scheduler;

class task_worker {
public:
    task_worker(task_scheduler& scheduler, int worker_id);
    ~task_worker();

    void stop() noexcept {
        m_running = false;
        m_wait_cv.notify_one();
    }

    void execute() noexcept;
    void wait() noexcept;
    void wakeup() noexcept;

private:
    void push(std::unique_ptr<task> t) noexcept;
    std::unique_ptr<task> fetch() noexcept;

private:
    task_scheduler& m_scheduler;
    std::condition_variable m_wait_cv;
    std::mutex m_wait_mutex;
    int m_worker_id;
    std::atomic_bool m_running;
};

} // namespace dandelion
