/*
 * File: signal.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include <array>
#include <csignal>
#include <functional>
#include <list>



namespace dandelion {


class signal_handler {
public:
	using handler_type = std::function<void()>;
	static void reigster(int signum, handler_type handler) noexcept {
		m_handlers[signum].push_back(handler);
	}

	static void invoke(int signum) noexcept {
		for (auto& handler : m_handlers[signum]) {
			handler();
		}
	}


private:
	constexpr static int MAX_SIGNALS = 32;
	static std::array<std::list<handler_type>, MAX_SIGNALS> m_handlers;
};


} // namespace dandelion
