/*
 * File: peer_network.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include "peer.h"

#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "file_table.h"


namespace dandelion::p2p {


struct peer_network_config {
    std::vector<endpoint> initial_peers; // initial peers to connect
    float heartbeat_interval; // in seconds
    int max_peers; // maximum number of peers to connect
    int port;
};


// Manages all connections to peers, including heart-beating detection
// and find new peers
class peer_network {
public:
    peer_network(peer_network_config const& config) noexcept;
    ~peer_network() noexcept;

    // Add a peer to the network and try to connect it.
    int add_peer(std::shared_ptr<peer> p) noexcept;

    // Remove a peer from the network by its address.
    void remove_peer(int id) noexcept;

    // void sync_file_meta(file_table_entry* entry) noexcept;

    int generate_peer_id() const noexcept {
        static int id = 0;
        return ++id;
    }

private:
    std::mutex m_mutex;
    int m_max_peers;
    std::vector<std::shared_ptr<peer>> m_peers;
    std::map<int, std::shared_ptr<peer>> m_peer_map;
};

} // namespace dandelion::p2p
