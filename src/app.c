#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <unistd.h>
#include "log.h"
#include "grid.h"

static void resolve_path(const char *relative, char *out, size_t size) {
    char exe[512] = {0};
    readlink("/proc/self/exe", exe, sizeof(exe) - 1);
    char *slash = strrchr(exe, '/');
    if (slash) *slash = '\0';
    
    slash = strrchr(exe, '/');
    if (slash) *slash = '\0';
    snprintf(out, size, "%s/%s", exe, relative);
}

int main(void) {
    char log_path[512];
    resolve_path("logs/session.log", log_path, sizeof(log_path));
    log_init(log_path);

    LOG_INFO("app started");

    int map_ready = grid_run();

    if (map_ready == 0) {
        LOG_INFO("map confirmed, ready for next phase");
        // TODO: game_run();
    } else {
        LOG_INFO("editor closed without saving");
    }

    log_close();
    return 0;
}
