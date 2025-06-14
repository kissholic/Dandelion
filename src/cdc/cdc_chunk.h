/*
 * File: cdc_chunk.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include "cdc_hash.h"
#include <cstddef>

namespace dandelion::cdc {

consteval inline unsigned long long operator ""_KB (unsigned long long n) {
    return n * 1024;
};

consteval inline unsigned long long operator ""_MB (unsigned long long n) {
    return n * 1024 * 1024;
};

constinit unsigned long long const MIN_CHUNK_SIZE = 4_KB;
constinit unsigned long long const MAX_CHUNK_SIZE = 4_MB;

struct cdc_chunk {
    hash_t hash;
    size_t size;
    size_t offset;
};

} // namespace dandelion::cdc
