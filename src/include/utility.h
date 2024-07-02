/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __UTILITY__
#define __UTILITY__

/* standard import */
#include <map>
#include <string>
#include <vector>

enum class CommandType {
    INSERT,
    WRITE,
    WRITE_CLOSE,
    INVALID,
    QUIT,
};

extern std::map<std::string, CommandType> command_codes;

/* Return the command code based on commmand_codes map defined above */
CommandType get_command_code(std::string);

/* split string based on delimiter and return vector of strings */
std::vector<std::string> split(std::string, std::string);
std::vector<std::string> split(std::string, char);

std::string basename(std::string);

#endif /* __UTILITY__ */
