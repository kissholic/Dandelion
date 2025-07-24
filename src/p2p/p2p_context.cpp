/*
 * File: p2p.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */


#include "p2p_context.h"
#include "dsignal.h"
#include "spdlog/spdlog.h"
#include "sys/socket.h"
#include<netinet/in.h>
#include "local_server.h"
#include <thread>


namespace dandelion::p2p {


p2p_context::p2p_context() noexcept
    : m_peer_network(nullptr)
    , m_local_server(nullptr)
    , m_scheduler(10)
{
    register_rpc_handlers();
}


p2p_context::~p2p_context() noexcept {
}


bool p2p_context::init(peer_network_config const& config) noexcept {
    m_local_server = std::make_unique<local_server>(this);
    if (!m_local_server->init(10101)) [[unlikely]] {
        spdlog::error("Failed to set up local server");
        return false;
    }

    m_peer_network = std::make_unique<peer_network>(config);

    if (!m_peer_network) [[unlikely]] {
        spdlog::error("Peer network is not set up");
        return false;
    }

    for (const auto& addr : config.initial_peers) {
        auto p = std::make_shared<peer>(addr);
        m_peer_network->add_peer(p);
    }

    signal_handler::reigster(SIGINT, []() {
        spdlog::info("Received SIGINT, shutting down P2P context");
        // m_peer_network->stop();
    });

    return true;
}


void p2p_context::run() noexcept {
    std::thread server_thread([this]() {
        m_local_server->run();
    });
    server_thread.detach();

    m_scheduler.wait_finish();
}


void p2p_context::dispatch(connection&& conn) noexcept {
    auto dispatch_rpc = [this](connection&& conn) -> task {
        co_await this->invoke(std::move(conn));
    };

    // Process the incoming connection in a separate thread
    // TODO: Schedule this task to the task scheduler
    std::thread rpc_thread(dispatch_rpc, std::move(conn));
    rpc_thread.detach();

    m_scheduler.submit(
        [this](auto&& conn) -> task {
            co_await this->invoke(std::move(conn));
        }(std::move(conn))
    );
}

void p2p_context::add_peer(endpoint const& addr) noexcept {
    assert(0);
}

task p2p_context::invoke(connection&& conn) noexcept {
    if (conn) [[unlikely]] {
        spdlog::error("Invalid socket file descriptor: {}", conn.fd);
        co_return;
    }

    peer_msg_type msg_type{};
    ssize_t bytes_received = recv(conn.fd, &msg_type, sizeof(msg_type), 0);
    if (bytes_received != sizeof(msg_type)) [[unlikely]] {
        spdlog::error("Failed to receive message header from socket {}", conn.fd);
        co_return;
    }

    co_await m_rpc_handlers[msg_type](std::move(conn));
}


void p2p_context::register_rpc_handlers() noexcept {
    m_rpc_handlers[peer_msg_type::SHUTDOWN] = [](connection&& conn) -> task {
        // TODO: Check if the connection comes from self
        spdlog::info("Received shutdown request from peer");
        co_return;
    };

    m_rpc_handlers[peer_msg_type::SYNC_PEERS] = [this](connection&& conn) -> task {
        co_await this->rpc_sync_peers(std::move(conn));
    };

    m_rpc_handlers[peer_msg_type::SYNC_FILES] = [this](connection&& conn) -> task {
        co_await this->rpc_sync_files(std::move(conn));
    };

    m_rpc_handlers[peer_msg_type::QUERY_FILES] = [this](connection&& conn) -> task {
        co_await this->rpc_query_files(std::move(conn));
    };
}


task p2p_context::rpc_send_heartbeat(connection&& conn) noexcept {
    // Process heartbeat message
    spdlog::info("Received heartbeat from peer");
    
    // Send response back to peer
    // char response_msg = static_cast<char>(peer_msg_type::RESVE_HEARTBEAT);
    // ssize_t bytes_sent = send(socket_fd, &response_msg, sizeof(response_msg), 0);
    // if (bytes_sent != sizeof(response_msg)) [[unlikely]] {
    //     spdlog::error("Failed to send heartbeat response to peer on socket {}", socket_fd);
    //     return;
    // }

    co_return;
}


task p2p_context::rpc_resve_heartbeat(connection&& conn) noexcept {
    // Process response to heartbeat message
    spdlog::info("Received heartbeat response from peer");

    // TODO: Update peer heartbeat status
    co_return;
}


task p2p_context::rpc_sync_peers(connection&& conn) noexcept {
    // Process sync peers message
    spdlog::info("Received sync peers message from peer");

    co_return;
}


task p2p_context::rpc_sync_files(connection&& conn) noexcept {
    // Process sync files message
    spdlog::info("Received sync files message from peer");
    co_return;
}


task p2p_context::rpc_query_files(connection&& connsocket) noexcept {
    // Process query files message
    spdlog::info("Received query files message from peer");
    co_return;
}




void p2p_context::sync_file_info(std::vector<fast_file_entry> const& file_entry) noexcept {
    
}


void p2p_context::sync_file(file_table_entry* entry) noexcept {

}

} // namespace dandelion::p2p
