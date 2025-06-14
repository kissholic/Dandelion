/*
 * File: file_entry.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include <chrono>
#include <memory>
#include <string>

#include "common/time.h"

namespace dandelion {

class file_entry {
public:
    file_entry(std::string const& filename);
    ~file_entry();

    std::byte* data() const noexcept;
    size_t size() const noexcept;

    operator bool() const noexcept;

private:
    void load() noexcept;

    struct data_desc {
        std::byte* data;
        size_t size;
    };

    static void release_file(FILE* fp) noexcept;
    static void release_data(data_desc* desc) noexcept;

private:
    std::string m_filename;
    std::unique_ptr<FILE, decltype(&release_file)> m_file;
    std::unique_ptr<data_desc, decltype(&release_data)> m_data;
};


struct fast_file_entry {
    std::string filename;
    time_point last_modified;
};

} // namespace dandelion
