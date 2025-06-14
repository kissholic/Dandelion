/*
 * File: p2p.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */


#include "p2p.h"
#include "spdlog/spdlog.h"
#include "sys/socket.h"


namespace dandelion::p2p {


bool p2p_context::setup() noexcept {
    m_peer_manager->connect_all();
    if (m_peer_manager->empty()) {
        spdlog::error("No peers to connect");
        return false;
    }

    m_peer_manager->start_heartbeat();

    return true;
}

void p2p_context::run() noexcept {

}


int p2p_context::setup_socket(int port) noexcept {
    return 0;
}

void p2p_context::sync_file_info(std::vector<fast_file_entry> const& file_entry) noexcept {
    
}


void p2p_context::sync_file(file_table_entry* entry) noexcept {

}

} // namespace dandelion::p2p
