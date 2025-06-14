/*
 * File: peer.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "peer.h"
#include "spdlog/spdlog.h"


namespace dandelion::p2p {

peer::peer(ip_address const& addr) noexcept
    : m_addr(addr)
    , m_control_fd(-1)
    , m_data_fd(-1)
    , m_connected(false)
{
}

peer::~peer() noexcept {
    disconnect();
}

bool peer::is_alive() const noexcept {
    return m_connected;
}

bool peer::is_connected() const noexcept {
    return m_connected;
}


void peer::cmd_send(const char* msg, size_t size) noexcept {
    assert(0);
}


void peer::cmd_receive(char* buffer, size_t* size) noexcept {
    assert(0);
}


void peer::data_send(char const* msg, size_t size) noexcept {
    if (m_connected && m_data_fd != -1) {
        ::send(m_data_fd, msg, size, 0);
    }
    else
        spdlog::error("Peer is not connected or control fd is invalid");
}


void peer::data_receive(char* buffer, size_t* size) noexcept {
    if (m_connected && m_data_fd != -1) {
        ::recv(m_data_fd, const_cast<char*>(buffer), *size, 0);
    }
    else
        spdlog::error("Peer is not connected or control fd is invalid");
}

bool peer::connect() noexcept {
    m_control_fd = try_connect(m_addr.control_port);
    if (m_control_fd < 0) {
        spdlog::error("Failed to connect to peer {} at control port {}", m_addr.ip_addr, m_addr.control_port);
        return false;
    }

    m_data_fd = try_connect(m_addr.data_port);
    if (m_data_fd < 0) {
        spdlog::error("Failed to connect to peer {} at data port {}", m_addr.ip_addr, m_addr.data_port);
        close(m_control_fd);
        return false;
    }

    m_connected = true;

    return m_connected;
}


void peer::disconnect() noexcept {
    if (m_connected) {
        if (m_control_fd != -1) {
            close(m_control_fd);
            m_control_fd = -1;
        }

        if (m_data_fd != -1) {
            close(m_data_fd);
            m_data_fd = -1;
        }

        m_connected = false;
    }
}


int peer::try_connect(int port) noexcept {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(m_addr.ip_addr.c_str());

    if (::connect(socket_fd, (struct sockaddr*)&m_addr, sizeof(m_addr)) < 0) {
        close(socket_fd);
        m_connected = false;
        return false;
    }

    return true;
}

} // namespace dandelion::p2p
