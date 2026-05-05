#ifndef MAP_H
#define MAP_H

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

#endif
