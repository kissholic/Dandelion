/*
 * File: arg_option.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include "spdlog/spdlog.h"

#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>


namespace dandelion {


class arg_option {
public:
    using action_type = std::function<bool(std::vector<std::string>const&)>;


    arg_option& add_option(std::string const& key, action_type action, std::string const& description = "", int nr_args = 0) {
        m_option_map.insert({key, {key, nr_args, action, description}});
        return *this;
    }

    bool parse(std::vector<std::string> const& args) {
        int arg_index = 0;

        while (arg_index < args.size()) {
            std::string const& key = args[arg_index];
            
            if (!m_option_map.contains(key)) {
                spdlog::error("Unknown option: {}", key);
                return false;
            }

            m_options[key] = {};

            auto const& option = m_option_map[key];
            int nr_args = option.nr_args;
            if (nr_args < 0) {
                int next_index = arg_index + 1;
                while (next_index < args.size() && !args[next_index].starts_with('-')) {
                    m_options[key].push_back(args[next_index]);
                    next_index++;
                }
                arg_index = next_index - 1;
            } else {
                if (arg_index + nr_args >= args.size()) {
                    spdlog::error("Not enough arguments for option: {}", key);
                    return false;
                }

                for (int i = 0; i < nr_args; ++i)
                    m_options[key].push_back(args[arg_index + 1 + i]);

                arg_index += nr_args;
            }

            arg_index++;
        }

        return true;
    }

    bool has_option(std::string const& key) const noexcept {
        return m_options.contains(key);
    }

    bool trigger(std::string const& key) const noexcept {
        if (m_options.contains(key)) {
            auto const& option = m_option_map.at(key);
            if (option.action) {
                return option.action(m_options.at(key));
            }
        }
        return true;
    } 

    auto const& get_option(std::string const& key) const noexcept {
        if (m_option_map.contains(key)) {
            return m_option_map.at(key);
        }
        return m_option_map.at("");
    }

    auto const get_options_map() const {
        return m_option_map;
    }
    
    auto const get_options() const {
        return m_options;
    }

private:


private:
    struct option {
        std::string key;
        int nr_args;
        action_type action;
        std::string description;
    };

    std::map<std::string, option> m_option_map;
    std::map<std::string, std::vector<std::string>> m_options;
};

using arg_action_arg_t = std::vector<std::string> const&;
inline bool empty_arg_action(arg_action_arg_t) {
    return true;
}

} // namespace dandelion

