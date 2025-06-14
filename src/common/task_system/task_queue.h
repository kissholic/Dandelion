/*
 * File: task_queue.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include "task.h"

#include <deque>
#include <memory>
#include <mutex>


namespace dandelion {

class task_queue {
public:
    task_queue() = default;
    ~task_queue() = default;

    task_queue(task_queue const&) = delete;
    task_queue& operator=(task_queue const&) = delete;

    void push(std::unique_ptr<task> t) noexcept {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_tasks.push_back(std::move(t));
    }

    [[nodiscard]]
    std::unique_ptr<task> pop() noexcept {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto t = std::move(m_tasks.front());
        m_tasks.pop_front();
        return t;
    }

    [[nodiscard]]
    std::unique_ptr<task> steal() noexcept {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto t = std::move(m_tasks.back());
        m_tasks.pop_back();
        return t;
    }

    [[nodiscard]]
    bool empty() const noexcept {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_tasks.empty();
    }

private:
    mutable std::mutex m_mutex;
    std::deque<std::unique_ptr<task>> m_tasks;
};


} // namespace dandelion
