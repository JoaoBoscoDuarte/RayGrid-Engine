#include "log.h"
#include <stdarg.h>

// nível padrão (você pode mudar em runtime)
LogLevel CURRENT_LOG_LEVEL = LOG_LEVEL_DEBUG;

// nomes dos níveis
static const char *level_strings[] = {
    "DEBUG",
    "INFO ",
    "WARN ",
    "ERROR"
};

// cores ANSI (Linux/macOS)
static const char *level_colors[] = {
    "\x1b[36m", // DEBUG - ciano
    "\x1b[32m", // INFO  - verde
    "\x1b[33m", // WARN  - amarelo
    "\x1b[31m"  // ERROR - vermelho
};

#define COLOR_RESET "\x1b[0m"

void log_message(LogLevel level, const char *file, int line, const char *fmt, ...) {
    if (level < CURRENT_LOG_LEVEL) return;

    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "%s[%s]%s (%s:%d): ",
            level_colors[level],
            level_strings[level],
            COLOR_RESET,
            file, line);

    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    va_end(args);
}

/*
DEBUG → detalhes internos (tecla, posição do mouse)
INFO → eventos normais (inicialização, salvar mapa)
WARN → algo errado mas não fatal (sem spawn)
ERROR → falha real (SDL_Init falhou)
*/