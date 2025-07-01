/*
 * File: signal.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include "signal.h"

namespace dandelion {

std::array<std::list<signal_handler::handler_type>, signal_handler::MAX_SIGNALS> signal_handler::m_handlers;

} // namespace dandelion
