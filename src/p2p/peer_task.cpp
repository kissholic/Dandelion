/*
 * File: peer_task.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include "file_manager.h"
#include <filesystem>
#include <memory>
#include "peer_server.h"
#include "peer_task.h"
#include "spdlog/spdlog.h"
#include "stream_buffer.h"
#include "uv.h"
#include <vector>


namespace dandelion::p2p {


peer_task::~peer_task() noexcept {
	if (m_handle) {
		uv_close(m_handle, nullptr);
	}
}


// Constructor is required to be noexcept
peer_task_request_file_info::peer_task_request_file_info(uv_handle_t* handle, uv_buf_t const* buf, ssize_t size) noexcept 
	: peer_task_crtp(handle)
{
	uv_stream_t* stream = (uv_stream_t*)m_handle;
	peer_server* server = (peer_server*) uv_handle_get_data((uv_handle_t*)stream);

	// Parse file names
	stream_buffer sbuffer(buf->base, size);
	{
		auto kind [[maybe_unused]] = sbuffer.read<uint32_t>();
		auto padding [[maybe_unused]] = sbuffer.read<uint32_t>();
		auto length = sbuffer.read<uint64_t>();
		if (length > size) {
			spdlog::error("Invalid file length");
			return;
		}
	}

	auto file_num = sbuffer.read<uint32_t>();
	if (!file_num) {
		spdlog::error("Invalid file number");
		return;
	}

	std::vector<std::string> file_names;
	for (uint32_t i = 0; i < file_num; ++i) {
		auto file_name = sbuffer.read<std::string>();
		if (!file_name) {
			spdlog::error("Invalid file name");
			return;
		}
		file_names.push_back(file_name.value());
	}

	file_manager* file_mgr = server->get_file_manager();
	std::vector<response_file_info> file_infos;
	for (auto const& file_name : file_names) {
		auto file_chunks = file_mgr->get_chunks(file_name);

		file_infos.emplace_back(file_name, std::move(file_chunks));
	}

	// Serialize and send file info to peers
	writable_stream_buffer wsb{4096};
	wsb.write<uint32_t>((uint32_t)p2p::peer_task_kind::REQUEST_FILE_INFO);
	wsb.write<uint32_t>(0);
	wsb.write<uint64_t>(0);
	
	wsb.write<uint32_t>(file_infos.size());
	for (auto const& file_info : file_infos) {
		wsb.write(file_info.file_name);
		wsb.write<uint32_t>(file_info.chunks->size());
		for (auto const& chunk : *file_info.chunks) {
			wsb.write<uint64_t>(chunk.hash);
			wsb.write<uint64_t>(chunk.size);
			wsb.write<uint64_t>(chunk.offset);
		}
	}

	wsb.finalize();

	uv_write_t req;
	uv_write(&req, stream, wsb.data(), wsb.num(), nullptr);
}


void peer_task_request_file_info::process(char const* buff, ssize_t size) noexcept {
	// Send cdc info to peer
	spdlog::info("peer_task_request_file_info: info sent.");
}


peer_task_request_file_data::peer_task_request_file_data(uv_handle_t* handle, uv_buf_t const* buf, ssize_t size) noexcept 
	: peer_task_crtp(handle)
{
}


void peer_task_request_file_data::process(char const* buff, ssize_t size) noexcept {
	// Send cdc chunk to peer
	spdlog::info("peer_task_request_file_data: data sent.");
}


} // namespace dandelion::p2p
