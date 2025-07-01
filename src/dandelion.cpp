/*
 * File: dandelion.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include "dandelion.h"

#include "common/task_system/task_scheduler.h"
#include "dandelion_context.h"
#include "p2p/p2p_context.h"
#include "spdlog/spdlog.h"

#include <csignal>
#include <memory>


int dandelion_main(dandelion_config& config) noexcept {
    // if (config.peers.empty()) {
    //     spdlog::error("No peers specified in the configuration.");
    //     return -1;
    // }

    // dandelion::task_scheduler scheduler(1);
    // scheduler.wait_finish();

    auto p2p_ctx = std::make_unique<dandelion::p2p::p2p_context>();

    dandelion::p2p::peer_network_config p2p_config{
        .initial_peers = config.peers,
        .heartbeat_interval = 5.0f, // 5 seconds
        .max_peers = 10, // Maximum number of peers to connect
        .port = 10101 // Default port for P2P connections
    };
    if (!p2p_ctx->init(p2p_config)) {
        spdlog::error("Failed to set up p2p context.");
        return -1;
    }

    p2p_ctx->run();


    return 0;
}

