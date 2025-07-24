/*
 * File: lcoal_server.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include "local_server.h"
#include "spdlog/spdlog.h"
#include "sys/socket.h"
#include <memory>
#include <netinet/in.h>
#include "p2p_context.h"
#include "p2p_socket.h"
#include "dsignal.h"


namespace dandelion::p2p {


local_server::local_server(p2p_context* ctx) noexcept
	: m_p2p_context(ctx), m_socket_fd(-1), m_port(0), m_should_stop(false) {
	spdlog::info("Local server created");
}


local_server::~local_server() noexcept {
	if (m_socket_fd >= 0)
		::close(m_socket_fd);

	spdlog::info("Local server socket closed");
}


bool local_server::init(int port) noexcept {
    int const max_retries = 5;
    int const max_pending_connections = 5;
    
    for (int i = 0; i < max_retries; ++i) {
        int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (listen_fd >= 0) {
            struct sockaddr_in server_addr{};
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all interfaces
            server_addr.sin_port = htons(port + i); // Increment port for each retry

            if (::bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
                spdlog::error("Failed to bind socket: {}", strerror(errno));
                ::close(listen_fd);
                continue; // Try the next port
            }

            if (::listen(listen_fd, max_pending_connections) == -1) {
                spdlog::error("Failed to listen on socket: {}", strerror(errno));
                ::close(listen_fd);
                continue; // Try the next port
            }

			m_port = port + i;
			m_socket_fd = listen_fd;
            return true;
        }
        spdlog::error("Failed to create socket on port {}", port + i);
    }

	signal_handler::reigster(SIGINT, [this]() {
		spdlog::info("Received SIGINT, stopping local server");
		stop();
	});

    return false;
}


void local_server::run() noexcept {
	while (!m_should_stop) {
		connection conn{::accept(m_socket_fd, nullptr, nullptr)};
		if (!conn) {
			spdlog::error("Failed to accept connection: {}", strerror(errno));
			continue;
		}

		process(std::move(conn));
	}

	spdlog::info("Local server stopped");
}


void local_server::stop() noexcept {
	m_should_stop = true;

	if (m_socket_fd >= 0) {
		struct sockaddr_in addr{};
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_LOOPBACK; // Connect to localhost
		addr.sin_port = htons(m_port); // Use the same port as the server
		if (::connect(m_socket_fd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
			spdlog::error("Failed to connect to local server: {}", strerror(errno));
			return;
		}
	
		peer_msg_type msg_type = peer_msg_type::SHUTDOWN;
		ssize_t bytes_sent [[maybe_unused]] = send(m_socket_fd, &msg_type, sizeof(msg_type), 0);
	}
}


void local_server::process(connection conn) noexcept {
	m_p2p_context->dispatch(std::move(conn));
}



} // namespace dandelion::p2p
