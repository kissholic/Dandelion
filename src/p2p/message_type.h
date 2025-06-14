/*
 * File: message_type.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include <cstdint>

namespace dandelion::p2p {

enum class message_type : uint8_t {
    // Control message
    HEARTBEAT,
    SYNC_PEERS,
    SYNC_FILES,

    QUERY_FILES,

    // Data message
};


} // namespace dandelion::p2p
