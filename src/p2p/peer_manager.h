/*
 * File: peer_manager.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include "peer.h"
#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include "file_table.h"


namespace dandelion::p2p {


// Manages all connections to peers, including heart-beating detection
// and find new peers
class peer_manager {
public:
    peer_manager() = default;
    ~peer_manager() = default;

    bool add_peer(std::shared_ptr<peer> p) noexcept {
        if (!p)
            return false;

        m_peers.push_back(std::move(p));
        return true;
    }

    void remove_peer(int id) noexcept;

    bool connect_all() noexcept;
    bool empty() const noexcept {
        return m_peers.empty();
    }

    void start_heartbeat() noexcept;
    void sync_file_meta(file_table_entry* entry) noexcept;

private:
    std::mutex m_mutex;
    std::vector<std::shared_ptr<peer>> m_peers;
    std::map<int, std::shared_ptr<peer>> m_peer_map;
};

} // namespace dandelion::p2p
