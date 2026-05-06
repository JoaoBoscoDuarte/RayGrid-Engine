#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "map.h"
#include "config.h"
#include "log.h"

#define L {LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT}
#define E {LIMIT,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,LIMIT}

int worldMap[MAP_HEIGHT][MAP_WIDTH] = {
    L,
    E, E, E, E, E,
    E, E, E, E, E,
    E, E, E,
    L,
};

#undef L
#undef E

void map_save(const EditorState *editor) {
    FILE *f = fopen(MAP_DAT, "wb");
    if (!f) { LOG_ERROR("failed to open map file for writing"); return; }
    fwrite(worldMap, sizeof(worldMap), 1, f);
    fwrite(editor, sizeof(EditorState), 1, f);
    fclose(f);

    f = fopen(LOG_MAP, "a");
    if (!f) { LOG_WARN("could not open map.log"); return; }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(f, "[%04d-%02d-%02d %02d:%02d:%02d] Mapa salvo | spawn=(%d,%d)\n",
        t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
        t->tm_hour, t->tm_min, t->tm_sec,
        editor->spawnX, editor->spawnY);

    fprintf(f, "  Mapa:\n");
    for (int y = 0; y < MAP_HEIGHT; y++) {
        fprintf(f, "  ");
        for (int x = 0; x < MAP_WIDTH; x++)
            fprintf(f, "%d ", worldMap[y][x]);
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
    fclose(f);

    LOG_INFO("map saved | spawn=(%d,%d)", editor->spawnX, editor->spawnY);
}

bool map_load(EditorState *editor) {
    FILE *f = fopen(MAP_DAT, "rb");
    if (!f) { LOG_WARN("map file not found, using default"); return false; }
    fread(worldMap, sizeof(worldMap), 1, f);
    fread(editor, sizeof(EditorState), 1, f);
    fclose(f);
    LOG_INFO("map loaded | spawn=(%d,%d)", editor->spawnX, editor->spawnY);
    return true;
}

void map_get_tile_color(TileType tile, uint8_t *r, uint8_t *g, uint8_t *b) {
    switch (tile) {
        case LIMIT: *r = 180; *g = 180; *b = 180; break;
        case WALL:  *r =  70; *g = 130; *b = 180; break;
        case SPAWN: *r = 220; *g =  50; *b =  50; break;
        default:    *r =  50; *g =  50; *b =  50; break;
    }
}
