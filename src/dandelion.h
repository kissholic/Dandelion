/*
 * File: dandelion.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include "p2p/address.h"

#include <filesystem>
#include <vector>


struct dandelion_config {
	std::vector<dandelion::p2p::ip_address> peers;
	std::filesystem::path base_dir;
	std::vector<std::string> base_files;
};

int dandelion_main(dandelion_config& config) noexcept;
