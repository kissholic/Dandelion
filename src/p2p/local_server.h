/*
 * File: local_server.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include <atomic>
#include <memory>
#include "p2p_socket.h"



namespace dandelion::p2p {

class p2p_context;

// Listen for incoming connections and handle rpc requests
class local_server {
public:
	local_server(p2p_context* ctx) noexcept;
	~local_server() noexcept;

	bool init(int port) noexcept;
	void run() noexcept;
	void stop() noexcept;

private:
	void process(connection conn) noexcept;

private:
	p2p_context* m_p2p_context;
	int m_socket_fd;
	int m_port;

	std::atomic_bool m_should_stop;
};


} // namespace dandelion::p2p
