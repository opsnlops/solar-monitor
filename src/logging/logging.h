
#pragma once

#include "controller-config.h"

#include <cstring>
#include <cstdio>

#define LOG_LEVEL_VERBOSE 5
#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_FATAL 0


struct LogMessage {
    uint8_t level;
    char message[LOGGING_MESSAGE_MAX_LENGTH];
} __attribute__((packed));


void logger_init();

void __unused verbose(const char *message, ...);

void debug(const char *message, ...);

void info(const char *message, ...);

void warning(const char *message, ...);

void error(const char *message, ...);

void __unused fatal(const char *message, ...);

LogMessage createMessageObject(uint8_t level, const char *message, va_list args);

bool _is_safe_to_log();


void start_log_reader();
