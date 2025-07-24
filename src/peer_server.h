/*
 * File: peer_server.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include <array>
#include "common/task_system/task_scheduler.h"
#include "event_loop.h"
#include <functional>
#include "p2p/peer_task.h"
#include <string>
#include "uv.h"



namespace dandelion {

// Listen for incoming connections and handle rpc requests
class peer_server {
public:
    peer_server(event_loop* loop, std::string_view ip, int port) noexcept;
    ~peer_server() noexcept;

	static void on_new_connection(uv_stream_t* server, int status) noexcept;
	static void on_close_connection(uv_handle_t* handle) noexcept;

	static void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) noexcept;
	
	static void dispatch(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) noexcept;

private:
	void register_task(p2p::peer_task_kind kind, p2p::task_constructor* ctor);
	p2p::task_constructor* select_constructor(p2p::peer_task_kind) const noexcept;

private:
	// task_scheduler* m_task_scheduler;
    event_loop* m_event_loop;
	uv_tcp_t m_server;
	std::string m_ip;
	int m_port;

	static std::array<p2p::task_constructor*, static_cast<size_t>(p2p::peer_task_kind::TOTAL_KIND)> m_peer_tasks;
};


} // namespace dandelion
