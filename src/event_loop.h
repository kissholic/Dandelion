/*
 * File: event_loop.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include "uv.h"

namespace dandelion {


class event_loop {
public:
	event_loop() noexcept;
	~event_loop() noexcept;

	void run() noexcept;
	void stop() noexcept;

	uv_loop_t* get_loop() noexcept {
		return m_loop;
	}

private:
	uv_loop_t* m_loop;
	uv_idle_t m_idle;
};


} // namespace dandelion
