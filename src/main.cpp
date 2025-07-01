/*
 * File: dandelion.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include "common/arg_option.h"
#include "dandelion.h"
#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"
#include "signal.h"

#include <csignal>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>


namespace {


bool show_help(dandelion::arg_option& options) noexcept {
    spdlog::set_pattern("%v");
    spdlog::log(spdlog::level::info, "Usage:");
    for (const auto& [key, option] : options.get_options_map())
        spdlog::log(spdlog::level::info, "{:>8}: {}", key, option.description);

    return false;
}


// Print dandelion version.
// TODO: The version should be defined in a config file.
bool show_version(std::vector<std::string> const&) noexcept {
    spdlog::set_pattern("%v");
    spdlog::log(spdlog::level::info, "Dandelion version 1.0.0");
    return false;
}


bool load_config(dandelion_config& config, std::string const& filename) noexcept {
    if (!std::filesystem::exists(filename)) {
        spdlog::error("Configuration file does not exist: {}", filename);
        return false;
    }

    std::ifstream config_file(filename);
    if (!config_file) {
        spdlog::error("Failed to open configuration file: {}", filename);
        return false;
    }

    nlohmann::json json_config = nlohmann::json::parse(config_file, nullptr, false);
    
    if (json_config.is_discarded()) {
        spdlog::error("Failed to parse configuration file: {}", filename);
        return false;
    }

    try {
        if (json_config.contains("peers")) {
            for (const auto& peer : json_config["peers"]) {
                dandelion::p2p::endpoint addr;
                addr.addr = peer["ip"].get<std::string>();
                addr.port = peer["port"].get<int>();
                config.peers.push_back(addr);
            }
        }

        if (json_config.contains("base.dir")) {
            config.base_dir = json_config["base.dir"].get<std::filesystem::path>();
        }

        if (json_config.contains("base.files")) {
            for (const auto& file : json_config["base.files"]) {
                config.base_files.push_back(file.get<std::string>());
            }
        }
    } catch (const nlohmann::json::exception& e) {
        spdlog::error("Error parsing configuration file: {}", e.what());
        return false;
    }

    return true;
}


constexpr std::string default_config_path = "./dandelion.conf";
bool load_default_config(dandelion_config& config) noexcept {
    if (!std::filesystem::exists(default_config_path)) {
        // Create a default configuration file if it does not exist.
        std::ofstream config_file(default_config_path);
        if (!config_file) {
            spdlog::error("Failed to create default configuration file: {}", default_config_path);
            return false;
        }
        config_file << "{}";
    }

    return load_config(config, default_config_path);
}


static void setup_signal_handler() {
    std::signal(SIGINT, dandelion::signal_handler::invoke);
}

} // namespace


int main(int argc, char* argv[]) {
    setup_signal_handler();

    dandelion_config config;
    dandelion::arg_option options;

    options.add_option("-h", [&options](dandelion::arg_action_arg_t){ return show_help(options); }, "Show help message")
           .add_option("-v", show_version, "Show dandelion version")
           .add_option("-c", [&config](dandelion::arg_action_arg_t path){ return load_config(config, path[0]); }, "Specify configuration file", 1);

    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i)
        args.emplace_back(argv[i]);

    if (!options.parse(args)) return 1;

    if (!options.trigger("-h")) return 0;
    if (!options.trigger("-v")) return 0;

    if (options.trigger("-c")) {
        if (!load_default_config(config)) {
            spdlog::error("Failed to load default configuration");
            return 1;
        }
    }

    return dandelion_main(config);
}
 