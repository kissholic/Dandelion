/*
 * File: file_table.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include <map>
#include <memory>
#include <vector>
#include "common/time.h"


namespace dandelion::p2p {


struct file_table_entry {
    std::string filename;
    time_point last_modified;
    std::vector<int> peers;
};

using file_table = std::map<std::string, std::shared_ptr<file_table_entry>>;


} // namespace dandelion::p2p
