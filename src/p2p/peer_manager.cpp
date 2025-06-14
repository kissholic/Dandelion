/*
 * File: peer_manager.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include "peer_manager.h"
#include "spdlog/spdlog.h"
#include <algorithm>
#include <thread>

#include "common/random.h"
#include "message_type.h"

namespace dandelion::p2p {


bool peer_manager::connect_all() noexcept {
    for (auto& peer : m_peers) {
        if (!peer->connect()) {
            spdlog::error("Failed to connect to peer %s", peer->m_addr.ip_addr.c_str());
        }
    }

    auto [begin, end] = std::ranges::remove_if(m_peers, [](auto& peer) { return !peer->is_connected();});
    m_peers.erase(begin, end);
    if (m_peers.empty()) {
        spdlog::error("No peers connected");
        return false;
    }

    return true;
}


void peer_manager::start_heartbeat() noexcept {
    std::thread heartbeat_thread([this]() {
        while (true) {
            std::lock_guard<std::mutex> lock(m_mutex);

            char buffer[sizeof(message_type)];
            buffer[0] = static_cast<char>(message_type::HEARTBEAT);

            std::vector<int> unreachable_peers;

            for (auto& [id, peer] : m_peer_map) {
                if (!peer->is_alive()) {
                    if (peer->is_unreachable()) {
                        unreachable_peers.push_back(id);
                    }
                    else {
                        peer->mark_unreachable();
                        peer->cmd_send(&buffer[0], sizeof(buffer));
                    }
                }
            }

            for (auto id : unreachable_peers) {
                auto peer = m_peer_map[id];
                m_peer_map.erase(id);
                m_peers.erase(std::remove(m_peers.begin(), m_peers.end(), peer), m_peers.end());
                spdlog::info("Peer %s is unreachable", peer->m_addr.ip_addr.c_str());
            }

            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    });
    heartbeat_thread.detach();
    spdlog::info("Heartbeat thread started");
}


void peer_manager::sync_file_meta(file_table_entry* entry) noexcept {

}


} // namespace dandelion::p2p
