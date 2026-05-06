#ifndef CONFIG_H
#define CONFIG_H

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define TILE_SIZE 32

#define MAP_WIDTH (SCREEN_WIDTH / TILE_SIZE)
#define MAP_HEIGHT (SCREEN_HEIGHT / TILE_SIZE)

// ============================================================
//  Caminhos de arquivos
// ============================================================
#define MAP_DAT     PROJECT_ROOT "/assets/map.dat"
#define LOG_MAP     PROJECT_ROOT "/logs/map.log"
#define LOG_SESSION PROJECT_ROOT "/logs/session.log"

#endif