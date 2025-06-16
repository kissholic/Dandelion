/*
 * File: dandelion.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include "dandelion.h"

#include "common/task_system/task_scheduler.h"

#include "dandelion_context.h"
#include "p2p/p2p.h"
#include <csignal>
#include "spdlog/spdlog.h"



int dandelion_main(dandelion_config& config) noexcept {
    dandelion::task_scheduler scheduler(1);
    scheduler.wait_finish();

    dandelion::p2p::p2p_context p2p_context;
    p2p_context.run();
    return 0;
}

