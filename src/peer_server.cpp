/*
 * File: peer_server.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include "peer_server.h"
#include <spdlog/spdlog.h>
#include <sys/socket.h>


namespace dandelion {


constexpr const int kMaxBacklog = 128;

std::array<p2p::task_constructor*, static_cast<size_t>(p2p::peer_task_kind::TOTAL_KIND)> peer_server::m_peer_tasks;


peer_server::peer_server(event_loop* loop, std::string_view ip, int port) noexcept
	: m_event_loop(loop)
	, m_ip(ip)
	, m_port(port)
{
	register_task(p2p::peer_task_kind::REQUEST_FILE_DATA, p2p::peer_task_request_file_info::get_task_constructor());
	register_task(p2p::peer_task_kind::REQUEST_FILE_INFO, p2p::peer_task_request_file_data::get_task_constructor());

	uv_tcp_init(m_event_loop->get_loop(), &m_server);

	struct sockaddr_in addr;
	uv_ip4_addr(m_ip.c_str(), m_port, &addr);
	uv_tcp_bind(&m_server, (const struct sockaddr*)&addr, 0);
	uv_handle_set_data((uv_handle_t*)&m_server, this);

	uv_listen((uv_stream_t*)&m_server, kMaxBacklog, &peer_server::on_new_connection);
}


peer_server::~peer_server() noexcept {
}


void peer_server::on_new_connection(uv_stream_t* server, int status) noexcept {
	if (status < 0) {
		spdlog::error("Failed to accept new connection");
		return;
	}

	uv_tcp_t* peer = static_cast<uv_tcp_t*>(malloc(sizeof(uv_tcp_t)));

	event_loop* loop = static_cast<peer_server*>(uv_handle_get_data((uv_handle_t*)server))->m_event_loop;
	uv_tcp_init(loop->get_loop(), peer);
	
	if (uv_accept(server, (uv_stream_t*)peer) < 0)
		uv_read_start((uv_stream_t*)peer, alloc_buffer, dispatch);
	else
		uv_close((uv_handle_t*)peer, nullptr);
}


void peer_server::on_close_connection(uv_handle_t* handle) noexcept {
}


void peer_server::alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) noexcept {
	buf->base = static_cast<char*>(malloc(suggested_size));
	buf->len = suggested_size;
}


void peer_server::dispatch(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) noexcept {
	if (!stream->data) {
		// Check if the message length is valid
		if (nread < 4) {
			spdlog::error("Invalid message length");
			uv_close((uv_handle_t*)stream, nullptr);
			free(buf->base);
			return;
		}
	
		uint32_t* data = reinterpret_cast<uint32_t*>(buf->base);
		auto message_type = static_cast<p2p::peer_task_kind>(data[0]);

		if (message_type >= p2p::peer_task_kind::TOTAL_KIND) {
			spdlog::error("Invalid message type");
			return;
		}

		stream->data = m_peer_tasks[(uint32_t)message_type]();
	}

	auto task = static_cast<p2p::peer_task*>(stream->data);
	task->process(buf->base, nread);
}


void peer_server::register_task(p2p::peer_task_kind kind, p2p::task_constructor* ctor) {
	assert(m_peer_tasks[(uint32_t)kind] == nullptr);
	m_peer_tasks[(uint32_t)kind] = ctor;
}


p2p::task_constructor* peer_server::select_constructor(p2p::peer_task_kind kind) const noexcept {
	assert(m_peer_tasks[(uint32_t)kind] != nullptr);
	return m_peer_tasks[(uint32_t)kind];
}


} // namespace dandelion
