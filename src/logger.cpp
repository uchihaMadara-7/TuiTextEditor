#include "logger.h"

Logger::Logger() : m_level(LOG_INFO), m_verbose(false),
                   m_initialized(true), m_log_file(nullptr) {}

Logger::Logger(std::string log_file) : Logger() {
    if (log_file == NO_LOG_FILE) {
        m_verbose = true;
        return;
    }
    else if (!log_file.empty()) {
        m_log_file = fopen(log_file.c_str(), "w");
        if (!m_log_file) {
            m_initialized = false;
            return;
        }
    }
    else {
        m_initialized = false;
    }
}

Logger::~Logger() {
    if (m_log_file) {
        fclose(m_log_file);
    }
}

bool Logger::is_initialized() {
    return m_initialized;
}

void Logger::set_level(LogLevel level) {
    m_level = level;
}

void Logger::set_verbosity(bool verbosity) {
    m_verbose = verbosity;
}

std::string Logger::_get_level_str(LogLevel c_level) {
    switch(c_level) {
        case LOG_ERROR: return ERROR_STR;
        case LOG_INFO: return INFO_STR;
        case LOG_DEBUG: return DEBUG_STR;
    }
    /* Fallback to INFO level */
    return INFO_STR;
}

void Logger::error(std::string msg) {
    _trace(LOG_ERROR, "%s", msg);
}

void Logger::info(std::string msg) {
    _trace(LOG_INFO, "%s", msg);
}

void Logger::debug(std::string msg) {
    _trace(LOG_DEBUG, "%s", msg);
}