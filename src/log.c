#include "log.h"
#include <stdarg.h>

LogLevel CURRENT_LOG_LEVEL = LOG_LEVEL_DEBUG;

static const char *level_strings[] = { "DEBUG", "INFO ", "WARN ", "ERROR" };
static const char *level_colors[]  = { "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m" };
#define COLOR_RESET "\x1b[0m"

static FILE *log_file = NULL;

void log_init(const char *path) {
    log_file = fopen(path, "a");
}

void log_close(void) {
    if (log_file) { fclose(log_file); log_file = NULL; }
}

void log_message(LogLevel level, const char *file, int line, const char *fmt, ...) {
    if (level < CURRENT_LOG_LEVEL) return;

    va_list args;
    va_start(args, fmt);

    // Terminal com cor
    fprintf(stderr, "%s[%s]%s (%s:%d): ", level_colors[level], level_strings[level], COLOR_RESET, file, line);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    va_end(args);

    // Arquivo sem cor
    if (log_file) {
        va_start(args, fmt);
        fprintf(log_file, "[%s] (%s:%d): ", level_strings[level], file, line);
        vfprintf(log_file, fmt, args);
        fprintf(log_file, "\n");
        fflush(log_file);
        va_end(args);
    }
}