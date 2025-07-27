/*
 * File: task_kind.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include <cstdint>

namespace dandelion::p2p {


enum class peer_task_kind : uint32_t {
	REQUEST_FILE_INFO,
	REQUEST_FILE_DATA,

	TOTAL_KIND,
};


} // namespace dandelion::p2p
