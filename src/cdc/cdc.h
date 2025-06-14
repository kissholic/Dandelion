/*
 * File: cdc.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include "cdc_chunk.h"

#include <cstdio>
#include <vector>


namespace dandelion::cdc {

[[nodiscard]]
std::vector<cdc_chunk> cdc(std::byte const* data, size_t const length) noexcept;


} // namespace dandelion
