#include "utility.h"

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
