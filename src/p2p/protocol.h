/*
 * File: protocol.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include <string>
#include "task_kind.h"
#include <vector>


namespace dandelion {
namespace p2p {


/* Peer protocol
 * Message is divided into multiple segments which can be sent in same tcp connection.
*/


/* Task header
 *
 * 1. kind: task kind
 * 2. reserved: reserved for future use
 * 3. length: task data length
*/
struct task_header {
	peer_task_kind kind;
	uint32_t reserved;
	uint64_t length;
};

static_assert(sizeof(peer_task_kind) == 4, "peer_task_kind size must be 4");
static_assert(sizeof(task_header) == 16, "task_header size must be 16");


// struct request_file_info_task_protocol : task_header{
// 	std::vector<std::string> file_names;
// };


// struct request_file_data_task_protocol : task_header{
// 	uint32_t file_num;
// 	std::vector<std::string> file_names;
// };



} // namespace p2p
} // namespace dandelion
