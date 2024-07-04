/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard imports */
#include <csignal>

/* custom imports */
#include "include/utils.h"
#include "include/logger.h"

std::map<std::string, CommandType> command_codes = {
    std::make_pair("i", CommandType::INSERT),
    std::make_pair("insert", CommandType::INSERT),
    std::make_pair("q", CommandType::QUIT),
    std::make_pair("quit", CommandType::QUIT),
    std::make_pair("w", CommandType::WRITE),
    std::make_pair("write", CommandType::WRITE),
    std::make_pair("wq", CommandType::WRITE_CLOSE)
};

CommandType get_command_code(std::string command) {
    if (::command_codes.count(command)) {
        return ::command_codes[command];
    }
    return CommandType::INVALID;
}

std::vector<std::string> split(std::string text, std::string delimiter) {
    std::vector<std::string> ret;
    size_t index = 0;
    while (true) {
        size_t next_index = text.find_first_of(delimiter, index);
        std::string part = text.substr(index, (next_index-index));
        ret.push_back(part);
        if (next_index == std::string::npos) break;
        index = next_index + 1;
    }
    return ret;
}

std::vector<std::string> split(std::string text, char delimiter) {
    std::vector<std::string> ret;
    size_t index = 0;
    while (true) {
        size_t next_index = text.find_first_of(delimiter, index);
        std::string part = text.substr(index, (next_index-index));
        ret.push_back(part);
        if (next_index == std::string::npos) break;
        index = next_index + 1;
    }
    return ret;
}

std::string basename(std::string text) {
    return split(text, '/').back();
}

void setup_signal_handlers() {
    std::signal(SIGSEGV, handle_signal);
    std::signal(SIGINT, handle_signal);
}

void handle_signal(int signal) {
    FLUSH_LOGS;
    std::signal(signal, SIG_DFL);
    std::raise(signal);
}
