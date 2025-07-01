/*
 * File: peer_network.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include "peer_net.h"
#include "spdlog/spdlog.h"
#include <algorithm>
#include <thread>

#include "common/random.h"


namespace dandelion::p2p {


peer_network::peer_network(peer_network_config const& config) noexcept
    : m_max_peers(config.max_peers)
{
    m_peers.reserve(m_max_peers);

    spdlog::info("Peer network initialized");
}


peer_network::~peer_network() noexcept {
    
    spdlog::info("Peer network destroyed");
}


// Connect to peers
// TODO: Use task system to connect peers in async way
int peer_network::add_peer(std::shared_ptr<peer> p) noexcept {
    // TODO: Check the maximum number of peers, and free the least recently used peer if necessary

    if (!p->connect()) {
        spdlog::error("Failed to connect to peer %s", std::string(*p).c_str());
        return -1;
    }

    int peer_id = -1;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
         
        peer_id = generate_peer_id();
        m_peer_map[peer_id] = p;
        m_peers.push_back(p);
    }

    spdlog::info("Added peer %s with ID %d", std::string(*p).c_str(), peer_id);
    return peer_id;
}


void peer_network::remove_peer(int id) noexcept {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_peer_map.find(id);
    if (it != m_peer_map.end()) {
        auto peer = it->second;
        peer->disconnect(); // Ensure the peer is disconnected before removing

        m_peer_map.erase(it);
        auto [first, last] = std::ranges::remove_if(m_peers, [&peer](auto p){ return p == peer; });
        m_peers.erase(first, last);

        spdlog::info("Removed peer %s with ID %d", std::string(*peer).c_str(), id);
    }
    else {
        spdlog::warn("Peer with ID %d not found", id);
    }
}





} // namespace dandelion::p2p
