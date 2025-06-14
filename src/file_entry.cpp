/*
 * File: file_entry.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include "file_entry.h"
#include <filesystem>
#include "spdlog/spdlog.h"
#include <sys/mman.h>

namespace dandelion {

void file_entry::release_file(FILE* fp) noexcept {
    if (fp) {
        fclose(fp);
    }
}

void file_entry::release_data(data_desc* desc) noexcept {
    if (desc->data) {
        munmap(desc->data, desc->size);
    }
}

file_entry::file_entry(std::string const& filename)
    : m_filename(filename), m_file(nullptr, release_file), m_data(nullptr, &release_data) {
    load();
}

file_entry::~file_entry() {
}


void file_entry::load() noexcept {
    if (!std::filesystem::exists(m_filename)) {
        spdlog::error("file not found: {}", m_filename);
        return;
    }

    FILE* fp = fopen(m_filename.c_str(), "r");
    if (!fp) {
        spdlog::error("failed to open file: {}", m_filename);
        return;
    }
    m_file.reset(fp);

    struct stat st;
    if (fstat(fileno(m_file.get()), &st) != 0) {
        spdlog::error("failed to get file size: {}", m_filename);
        return;
    }

    void* data = mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fileno(m_file.get()), 0);
    if (data == MAP_FAILED) {
        spdlog::error("failed to mmap: {}", m_filename);
        return;
    }

    if ((uintptr_t)data % 4096 != 0) {
        spdlog::error("mmap is not aligned: {}", m_filename);
        munmap(data, st.st_size);
        return;
    }

    m_data.reset(new data_desc{static_cast<std::byte*>(data), (size_t) st.st_size});
}

std::byte* file_entry::data() const noexcept {
    return m_data->data;
}

size_t file_entry::size() const noexcept {
    return m_data->size;
}

file_entry::operator bool() const noexcept {
    return m_data->data != nullptr;
}


} // namespace dandelion
