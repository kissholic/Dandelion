/*
 * File: peer_task.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include "peer_task.h"
#include "spdlog/spdlog.h"
#include "uv.h"


namespace dandelion::p2p {


void peer_task_request_file_info::process(char const* buff, ssize_t size) noexcept {
	// Send cdc info to peer
	spdlog::info("peer_task_request_file_info: info sent.");
}


void peer_task_request_file_data::process(char const* buff, ssize_t size) noexcept {
	// Send cdc chunk to peer
	spdlog::info("peer_task_request_file_data: data sent.");
}



// Use coroutine to receive data from tcp multiple times to read data
task peer_task_request_file_info(char const* buff, ssize_t size) noexcept {

	
	// Send the file info to the peer
	co_return ;
}


} // namespace dandelion::p2p
