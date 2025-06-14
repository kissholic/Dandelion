/*
 * File: cut_point.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include "cdc_hash.h"
#include <cstddef>
#include <sys/types.h>


namespace dandelion::cdc {

struct cut_point {
    size_t m_offset;
    hash_t m_hash;
};

} // namespace dandelion
