/*
 * File: random.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include "random.h"

#include <chrono>

namespace dandelion {


int random_int(int min, int max) noexcept {
    static thread_local unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();

    seed = (seed * 1103515245 + 12345) ^ (seed >> 16);
    return min + (seed % (max - min + 1));
}


} // namespace dandelion
