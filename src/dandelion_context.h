/*
 * File: dandelion_context.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include <memory>
#include <vector>

#include "common/task_system/task_scheduler.h"
#include "file_entry.h"
#include "p2p/p2p_context.h"
#include "event_loop.h"

namespace dandelion {

struct dandelion_config {
    int nr_woker;
    std::vector<p2p::endpoint> roots;
    std::vector<std::string> local_files;
};


class dandelion_context {
public:
    dandelion_context(dandelion_config const& config) noexcept;
    ~dandelion_context() noexcept;

    bool init() noexcept;
    bool exit() noexcept;

    void add_file(fast_file_entry const& file) noexcept;

    void add_files(std::vector<fast_file_entry> const& files) noexcept;

    void remove_file(fast_file_entry const& file) noexcept {
        auto it = std::remove_if(m_local_files.begin(), m_local_files.end(),
            [&file](fast_file_entry const& f) { return f.filename == file.filename; });
            m_local_files.erase(it, m_local_files.end());
    }


    void run() noexcept;

private:
    bool setup_peer_network(std::vector<p2p::endpoint> const& addrs) noexcept;

    [[nodiscard]] std::vector<fast_file_entry> load_local_file() const noexcept;
    void sync_file_info(std::string const& filename) noexcept;

private:
    std::unique_ptr<p2p::p2p_context> m_p2p_context;
    std::vector<fast_file_entry> m_local_files;

    p2p::file_table m_files;

    std::vector<p2p::endpoint> m_roots;

    task_scheduler m_task_scheduler;

    std::unique_ptr<event_loop> m_event_loop;
};


} // namespace dandelion
