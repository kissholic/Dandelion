/*
 * File: net_msg.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>


namespace dandelion::p2p {


enum peer_msg_type : uint8_t {
	SHUTDOWN,

    SYNC_PEERS,
    SYNC_FILES,
    QUERY_FILES,

	UNKOWN_MSG,

	TOTAL_MSG_TYPE = UNKOWN_MSG + 1
};


// TODO: Protocol format may be defined here.


} // namespace dandelion::p2p

