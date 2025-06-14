/*
 * File: task.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include <coroutine>

#include "spdlog/spdlog.h"

namespace dandelion {


class task {
public:
    struct promise_type;
    using coroutine_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        task get_return_object() noexcept
        {
            return task{coroutine_type::from_promise(*this)};
        }

        std::suspend_always initial_suspend() noexcept {
            return {};
        }

        std::suspend_never final_suspend() noexcept {
            return {};
        }

        void unhandled_exception() noexcept {}

        void return_void() noexcept {}
    };

    task() noexcept
        : m_coroutine(nullptr)
    {}

    task(coroutine_type&& coroutine) noexcept
        : m_coroutine(std::move(coroutine))
    {}

    task(task&& other) noexcept
        : m_coroutine(other.m_coroutine)
    {
        other.m_coroutine = nullptr;
    }

    ~task() noexcept {
        if (m_coroutine && !m_coroutine.done())
            m_coroutine.destroy();
    }

    task& operator=(task&& other) noexcept {
        if (this != &other) {
            m_coroutine = other.m_coroutine;
            other.m_coroutine = nullptr;
        }
        return *this;
    }

    struct awatiable {
        bool await_ready() noexcept {
            return false;
        }

        void await_suspend(std::coroutine_handle<> coroutine) noexcept {
            m_coroutine = coroutine;
        }

        void await_resume() noexcept {}

        std::coroutine_handle<> m_coroutine;
    };

    auto operator co_await() noexcept {
        return awatiable{m_coroutine};
    }

    bool operator()() noexcept {
        if (m_coroutine && !m_coroutine.done()) {
            m_coroutine.resume();
            return true;
        }

        return false;
    }

    operator bool() const noexcept {
        return m_coroutine && !m_coroutine.done();
    }

private:
    coroutine_type m_coroutine;
};

} // namespace dandelion
