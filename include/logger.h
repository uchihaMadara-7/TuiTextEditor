#ifndef __LOGGER__
#define __LOGGER__

#include <iostream>
#include <cstdio>

#define NO_LOG_FILE "NO_LOG_FILE"
#define ERROR_STR "[ERROR]: "
#define INFO_STR "[INFO]: "
#define DEBUG_STR "[DEBUG]: "

enum LogLevel {
    LOG_ERROR,
    LOG_INFO,
    LOG_DEBUG
};

class Logger {
public:
    Logger();
    Logger(std::string log_file);
    ~Logger();

    static Logger& getInstance() {
        return getInstance(NO_LOG_FILE);
    }

    static Logger& getInstance(std::string log_file) {
        static Logger instance(log_file);
        return instance;
    }

    bool is_initialized();

    void set_level(LogLevel level);
    void set_verbosity(bool verbosity);

    std::string _get_level_str(LogLevel c_level) {
        switch(c_level) {
            case LOG_ERROR: return ERROR_STR;
            case LOG_INFO: return INFO_STR;
            case LOG_DEBUG: return DEBUG_STR;
        }
        /* Fallback to INFO level */
        return INFO_STR;
    }

    template<typename T>
    static auto convert(const T& arg) {
        if constexpr(std::is_same_v<T, std::string>) return arg.c_str();
        else return arg;
    }

    template<typename... Args>
    void trace(LogLevel c_level, std::string format, Args&&... args) {
        if (c_level > m_level) {
            return;
        }
        std::string meta = _get_level_str(c_level);
        format.insert(0, meta);
        format.append(1, '\n');
        if (m_log_file) {
            fprintf(m_log_file, format.c_str(), convert(args)...);
        }
        if (m_verbose) {
            printf(format.c_str(), convert(args)...);
        }
    }

    template<typename... Args>
    void error(std::string format, Args&&... args) {
        trace(LOG_ERROR, format, args...);
    }

    template<typename... Args>
    void info(std::string format, Args&&... args) {
        trace(LOG_INFO, format, args...);
    }

    template<typename... Args>
    void debug(std::string format, Args&&... args) {
        trace(LOG_DEBUG, format, args...);
    }

private:
    LogLevel m_level;
    bool m_verbose;
    bool m_initialized;
    FILE* m_log_file;
};

#endif /* __LOGGER__ */
