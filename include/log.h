#ifndef LOG_H
#define LOG_H

#include <stdio.h>

// Níveis de log
typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel;

// Configuração global
extern LogLevel CURRENT_LOG_LEVEL;

// Função base
void log_message(LogLevel level, const char *file, int line, const char *fmt, ...);

// Macros (o que você vai usar no código)
#define LOG_DEBUG(...) log_message(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)  log_message(LOG_LEVEL_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)  log_message(LOG_LEVEL_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) log_message(LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)

#endif