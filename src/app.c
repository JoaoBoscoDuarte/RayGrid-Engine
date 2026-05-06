#include "config.h"
#include "log.h"
#include "grid.h"

int main(void) {
    log_init(LOG_SESSION);

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
