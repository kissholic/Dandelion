/*
 * File: dandelion_context.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include "dandelion_context.h"

#include <csignal>
#include <thread>
#include "spdlog/spdlog.h"


namespace dandelion {


dandelion_context::dandelion_context(dandelion_config const& config) noexcept
    : m_roots{config.roots}
    , m_task_scheduler{config.nr_woker}
{
    spdlog::info("dandelion context created");
    init();
}


dandelion_context::~dandelion_context() noexcept
{
    exit();
}


bool dandelion_context::init() noexcept {
    spdlog::info("dandelion init");

    if (!setup_peer_network(m_roots)) {
        spdlog::error("Failed to set up p2p network");
        return false;
    }

    m_local_files = load_local_file();

    // Sync file info from peers
    m_p2p_context->sync_file_info(m_local_files);

    return true;
}


bool dandelion_context::exit() noexcept {
    spdlog::info("dandelion exit");
    return true;
}


void dandelion_context::run() noexcept {
    auto pending_files = [&](){
        std::vector<p2p::file_table_entry*> pending_files;

        for (auto const& [filename, latest_modified_time] : m_local_files) {
            if (!m_files.contains(filename))
                continue;

            auto peer_file = m_files[filename];

            if (peer_file->last_modified > latest_modified_time) {
                pending_files.push_back(peer_file.get());
            }
    
        }
        return pending_files;
    }();

    for (auto const& pending_file : pending_files) {
        if (pending_file->peers.empty()) {
            spdlog::error("No peers available for file {}", pending_file->filename);
            continue;
        }

        m_p2p_context->sync_file(pending_file);
        // m_task_scheduler.submit([]() -> task<int> { return {}; });
    }


    // m_task_scheduler.wait_finish();
}


bool dandelion_context::setup_peer_network(std::vector<p2p::ip_address> const& addrs) noexcept {
    m_p2p_context = std::make_unique<p2p::p2p_context>();
    if (!m_p2p_context)
        return false;

    for (auto& addr : addrs)
        m_p2p_context->add_peer(addr);

    return m_p2p_context->setup();
}


[[nodiscard]] std::vector<fast_file_entry> dandelion_context::load_local_file() const noexcept {
    assert(0);
    return {};
}

void dandelion_context::sync_file_info(std::string const& filename) noexcept {

}


} // namespace dandelion
