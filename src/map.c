#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "map.h"
#include "config.h"

// Resolve caminho relativo à raiz do projeto (diretório pai do binário em bin/)
static void project_path(const char *relative, char *out, size_t size) {
    char exe[512] = {0};
    readlink("/proc/self/exe", exe, sizeof(exe) - 1);
    // Remove "bin/raygrid" — sobe dois níveis (bin/ -> raiz)
    char *slash = strrchr(exe, '/');
    if (slash) *slash = '\0'; // remove nome do binário
    slash = strrchr(exe, '/');
    if (slash) *slash = '\0'; // remove bin/
    snprintf(out, size, "%s/%s", exe, relative);
}

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
    char path[512];

    project_path("assets/map.dat", path, sizeof(path));
    FILE *f = fopen(path, "wb");
    if (!f) return;
    fwrite(worldMap, sizeof(worldMap), 1, f);
    fwrite(editor, sizeof(EditorState), 1, f);
    fclose(f);

    project_path("logs/map.log", path, sizeof(path));
    f = fopen(path, "a");
    if (!f) return;

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
}

bool map_load(EditorState *editor) {
    char path[512];
    project_path("assets/map.dat", path, sizeof(path));
    FILE *f = fopen(path, "rb");
    if (!f) return false;
    fread(worldMap, sizeof(worldMap), 1, f);
    fread(editor, sizeof(EditorState), 1, f);
    fclose(f);

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
