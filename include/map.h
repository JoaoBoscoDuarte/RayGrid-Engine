#ifndef MAP_H
#define MAP_H

#include <stdint.h>
#include "config.h"
typedef enum {
    EMPTY = 0,
    LIMIT = 1,
    WALL = 2,
    SPAWN = 3
} TileType;

typedef struct {
    TileType currentSelection;
    bool hasSpawn;
    int spawnX, spawnY;
} EditorState;

extern int worldMap[MAP_HEIGHT][MAP_WIDTH];

void map_get_tile_color(TileType tile, uint8_t *r, uint8_t *g, uint8_t *b);
void map_save(const EditorState *editor);
bool map_load(EditorState *editor);

#endif
