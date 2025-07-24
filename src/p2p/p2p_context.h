/*
 * File: p2p_context.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include "peer_net.h"

#include <array>
#include <memory>
#include <optional>
#include <vector>

#include "common/task_system/task_scheduler.h"
#include "common/time.h"
#include "file_entry.h"
#include "net_msg.h"


namespace dandelion::p2p {

class local_server;

// p2p_context abstracts the interface of the P2P network, which
// includes managing peers, setting up connections, and synchronizing files.
class p2p_context {
public:
    p2p_context() noexcept;
    ~p2p_context() noexcept;

    p2p_context(p2p_context const&) = delete;
    p2p_context& operator=(p2p_context const&) = delete;

    bool init(peer_network_config const& config) noexcept;
    
    // Process incoming connection and handle RPC requests.
    void dispatch(connection&& conn) noexcept;

    void add_peer(endpoint const& addr) noexcept;
    void remove_peer(endpoint const& addr) noexcept;

    void sync_file_info(std::vector<fast_file_entry> const& file_entry) noexcept;
    void sync_file(file_table_entry* entry) noexcept;

    void run() noexcept;

    std::optional<time_point> query_file_modified_time(std::string const& filename) noexcept;

private:
    using net_msg_handler = std::function<task(connection&&)>;
    void register_rpc_handlers() noexcept;
    task invoke(connection&& conn) noexcept;

    task rpc_send_heartbeat(connection&& conn) noexcept;
    task rpc_resve_heartbeat(connection&& conn) noexcept;
    task rpc_sync_peers(connection&& conn) noexcept;
    task rpc_sync_files(connection&& conn) noexcept;
    task rpc_query_files(connection&& conn) noexcept;

private:
    std::unique_ptr<peer_network> m_peer_network;
    std::unique_ptr<local_server> m_local_server;
    dandelion::task_scheduler m_scheduler;

    std::array<net_msg_handler, peer_msg_type::TOTAL_MSG_TYPE> m_rpc_handlers;
};


} // namespace dandelion::p2p
