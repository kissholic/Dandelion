/*
 * File: file_manager.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include "cdc/cdc.h"
#include "file_mapper.h"
#include <optional>
#include <vector>


namespace dandelion {

class file_manager {
public:
	file_manager() = default;
	~file_manager() = default;

	std::unique_ptr<std::vector<cdc::cdc_chunk>> get_chunks(std::string const& name) const noexcept;


	void add(std::string const& name, std::filesystem::path const& path) noexcept;

	void del(std::string const& name) noexcept;

	std::optional<std::filesystem::path> get(std::string const& name) const noexcept;

private:
	dandelion::file_mapper m_file_mapper;
};

} // namespace dandelion::file_manager
