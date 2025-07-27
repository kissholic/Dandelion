/*
 * File: file_mapper.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include <filesystem>
#include <map>
#include <string>


namespace dandelion {


// Map monitored files to local file paths
class file_mapper {
public:
    file_mapper() = default;
    ~file_mapper() = default;

	bool exist(std::string_view name) const {
		return m_map.contains(name);
	}

	void add(std::string const& name, std::filesystem::path const& path) {
		m_map[name] = path;
	}

	void del(std::string const& name) {
		m_map.erase(name);
	}

	std::filesystem::path const& get(std::string const& name) const {
		return m_map.at(name);
	}

private:
	std::map<std::string, std::filesystem::path, std::less<>> m_map;
};



} // namespace dandelion::file_mapper
