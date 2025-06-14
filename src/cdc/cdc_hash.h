/*
 * File: cdc_hash.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace dandelion::cdc {

constinit int const CDC_HASH_SIZE = 8;
constinit int const CDC_PATTERN_MASK_BITS = 16;
constinit std::uint32_t const CDC_PATTERN_MASK  = 0xFFFF;
constinit std::uint32_t const CDC_PATTERN_MAGIC = 0x488B;

using hash_t = std::uint64_t;


} // namespace dandelion::cdc