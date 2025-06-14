/*
 * File: p2p.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include "peer_manager.h"
#include <memory>
#include <optional>
#include <vector>

#include "common/time.h"
#include "file_entry.h"


namespace dandelion::p2p {


class p2p_context {
public:
    p2p_context() noexcept
        : m_peer_manager(std::make_unique<peer_manager>())
        , m_should_stop(false)
    {}

    ~p2p_context() noexcept = default;

    p2p_context(p2p_context const&) = delete;
    p2p_context& operator=(p2p_context const&) = delete;

    void add_peer(ip_address const& addr) noexcept;
    void remove_peer(ip_address const& addr) noexcept;

    // Setup all connections to peers
    bool setup() noexcept;

    bool should_stop() const noexcept {
        return m_should_stop;
    }

    void sync_file_info(std::vector<fast_file_entry> const& file_entry) noexcept;
    void sync_file(file_table_entry* entry) noexcept;

    void run() noexcept;

    std::optional<time_point> query_file_modified_time(std::string const& filename) noexcept;

private:

    static int setup_socket(int port) noexcept;

private:
    std::unique_ptr<peer_manager> m_peer_manager;
    int control_port;
    int data_port;
    bool m_should_stop;
};


} // namespace dandelion::p2p
