/*
 * File: file_monitor.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include <filesystem>
#include <set>
#include <string>

namespace dandelion {


class file_monitor {
public:
    file_monitor() = default;

    bool add(std::filesystem::path) noexcept;
    bool ignore(std::filesystem::path) noexcept;

private:
    std::set<std::filesystem::path> m_whitelist;
    std::set<std::filesystem::path> m_blacklist;
};

} // namespace dandelion
