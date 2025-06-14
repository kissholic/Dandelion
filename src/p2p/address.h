/*
 * File: address.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once


#include <string>

namespace dandelion::p2p {


struct ip_address {
    std::string ip_addr;
    int control_port;
    int data_port;
};


} // namespace dandelion::p2p
