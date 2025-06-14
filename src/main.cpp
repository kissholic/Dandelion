/*
 * File: dandelion.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include <cstdio>
#include <csignal>
#include <filesystem>
#include <string>
#include <vector>
#include "spdlog/spdlog.h"
 
#include "common/task_system/task_scheduler.h"

#include "dandelion_context.h"
#include "p2p/p2p.h"

struct dandelion_config {
    int nr_woker;
    std::vector<dandelion::p2p::ip_address> roots;
};


static void exit_sig_handler(int sig) {
    std::exit(sig);
}


static void setup_signal_handler() {
    std::signal(SIGINT, exit_sig_handler);
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        spdlog::error("usage: {} <filename>", argv[0]);
        return 1;
    }

    std::string filename = argv[1];

    setup_signal_handler();


    dandelion::task_scheduler scheduler(1);
    scheduler.wait_finish();

    dandelion::p2p::p2p_context p2p_context;
    p2p_context.run();


    return 0;
}
 