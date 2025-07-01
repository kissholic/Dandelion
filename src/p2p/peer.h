/*
 * File: peer.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once


#include <memory>
#include <string>

#include "common/time.h"
#include "p2p_socket.h"

namespace dandelion::p2p {


class peer {
public:
    peer(endpoint const& addr) noexcept;
    ~peer() noexcept;
    peer(peer const&) = delete;
    peer& operator=(peer const&) = delete;

    [[nodiscard]]
    bool is_alive() const noexcept;

    [[nodiscard]]
    bool is_connected() const noexcept;

    void cmd_send(const char* msg, size_t size) noexcept;
    void cmd_receive(char* buffer, size_t* size) noexcept;

    void data_send(const char* msg, size_t size) noexcept;
    void data_receive(char* buffer, size_t* size) noexcept;

    bool connect() noexcept;
    void disconnect() noexcept;

    void mark_unreachable() noexcept {
        m_unreachable = true;
    }

    bool is_unreachable() const noexcept {
        return m_unreachable;
    }

    operator std::string() const noexcept {
        return m_addr.addr + ":" + std::to_string(m_addr.port);
    }


private:

    int try_connect(int port) noexcept;

private:
    endpoint m_addr;
    int m_handle;
    bool m_connected;

    bool m_unreachable;
    time_point m_last_heartbeat;
};


} // namespace dandelion::p2p
