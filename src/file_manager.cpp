/*
 * File: file_manager.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include "file_manager.h"
#include <filesystem>
#include <fstream>


namespace dandelion {


std::unique_ptr<std::vector<cdc::cdc_chunk>> file_manager::get_chunks(std::string const& name) const noexcept {
	if (!m_file_mapper.exist(name)) {
		return {};
	}

	auto local_path = std::filesystem::path(m_file_mapper.get(name));
	auto data_size = std::filesystem::file_size(local_path);

	auto data = std::vector<char>(data_size);
	auto file = std::ifstream(local_path, std::ios::binary);
	file.read(data.data(), data_size);

	return cdc::cdc(data.data(), data_size);
}


} // namespace dandelion::file_manager
