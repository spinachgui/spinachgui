
#include <shared/log.hpp>
#include <iostream>

void default_handler(const std::string& s) {
}

LogHandler handler = default_handler;

void setLogHandler(LogHandler logHandler) {
    handler = logHandler;
}

void _log_message(const std::string& s) {
    handler(s);
}
