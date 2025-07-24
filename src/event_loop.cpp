/*
 * File: event_loop.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include "event_loop.h"
#include "spdlog/spdlog.h"
#include "uv.h"




namespace dandelion {

event_loop::event_loop() noexcept
	: m_loop(nullptr)
{
	m_loop = static_cast<uv_loop_t*>(std::malloc(sizeof(uv_loop_t)));
	uv_loop_init(m_loop);
	uv_loop_set_data(m_loop, this);

	uv_idle_init(m_loop, &m_idle);
	uv_idle_start(&m_idle, [](uv_idle_t* idle) {


		uv_idle_stop(idle);
	});

	spdlog::info("event_loop initialized.");
}


event_loop::~event_loop() noexcept {
	uv_loop_close(m_loop);
	std::free(m_loop);

	spdlog::info("event_loop destroyed.");
}


void event_loop::run() noexcept {
	uv_run(m_loop, UV_RUN_DEFAULT);
}


void event_loop::stop() noexcept {
	uv_stop(m_loop);
}




} // namespace dandelion
