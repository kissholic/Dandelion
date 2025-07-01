/*
 * File: p2p_socket.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include <memory>
#include <string>
#include <unistd.h>

namespace dandelion::p2p {


struct endpoint {
    std::string addr;
    int port;
};


using socket_handle = std::unique_ptr<int, decltype(&::close)>;
using conn_handle = std::unique_ptr<int, decltype(&::close)>;


struct connection {
    int fd;

    connection(int fd) noexcept
        : fd(fd) {}
    ~connection() noexcept {
        if (fd >= 0) {
            ::close(fd);
        }
    }

    connection(connection&& other) noexcept
        : fd(other.fd) {
        other.fd = -1; // Transfer ownership
    }


    connection& operator=(connection&& other) noexcept {
        if (this != &other) {
            if (fd >= 0) {
                ::close(fd);
            }
            fd = other.fd;
            other.fd = -1; // Transfer ownership
        }
        return *this;
    }

    
    operator bool() const noexcept {
        return fd >= 0;
    }
};

} // namespace dandelion::p2p
