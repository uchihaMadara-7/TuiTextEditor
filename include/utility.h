#ifndef __UTILITY__
#define __UTILITY__

#include <map>
#include <string>

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