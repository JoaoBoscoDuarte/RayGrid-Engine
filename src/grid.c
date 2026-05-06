#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include "config.h"
#include "map.h"
#include "log.h"

static void resolve_path(const char *relative, char *out, size_t size) {
    
    char exe[512] = {0};
    readlink("/proc/self/exe", exe, sizeof(exe) - 1);
    char *slash = strrchr(exe, '/');
    
    if (slash) *slash = '\0';
    slash = strrchr(exe, '/');

    if (slash) *slash = '\0';
    snprintf(out, size, "%s/%s", exe, relative);
}

static void render_grid(SDL_Surface *surface) {
    LOG_DEBUG("rendering grid");

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Uint8 r, g, b;
            map_get_tile_color((TileType)worldMap[y][x], &r, &g, &b);

            SDL_Rect rect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE - 1, TILE_SIZE - 1 };
            SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, r, g, b));
        }
    }
}

int main(void) {
    char log_path[512];
    resolve_path("logs/session.log", log_path, sizeof(log_path));
    log_init(log_path);

    LOG_INFO("initializing SDL");

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        LOG_ERROR("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "RayGrid - Editor",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        LOG_ERROR("failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    LOG_INFO("window created successfully");

    SDL_Surface *surface = SDL_GetWindowSurface(window);
    EditorState editor = {.currentSelection = WALL, .hasSpawn = false};

    LOG_INFO("loading map");
    map_load(&editor);

    bool running = true;
    SDL_Event event;
    char title[128];

    while (running) {
        LOG_DEBUG("frame start | running=%d", running);

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                LOG_INFO("quit event received");
                running = false;
            }

            if (event.type == SDL_KEYDOWN) {
                LOG_DEBUG("key pressed: %s", SDL_GetKeyName(event.key.keysym.sym));

                switch (event.key.keysym.sym) {
                    case SDLK_x:
                        LOG_INFO("exit requested (X pressed)");
                        running = false;
                        break;

                    case SDLK_RETURN:
                        if (editor.hasSpawn) {
                            LOG_INFO("saving map and exiting");
                            map_save(&editor);
                            running = false;
                        } else {
                            LOG_WARN("spawn not set, map not saved");
                        }
                        break;

                    case SDLK_1:
                        editor.currentSelection = EMPTY;
                        LOG_INFO("selection changed to EMPTY");
                        break;

                    case SDLK_2:
                        editor.currentSelection = WALL;
                        LOG_INFO("selection changed to WALL");
                        break;

                    case SDLK_3:
                        editor.currentSelection = SPAWN;
                        LOG_INFO("selection changed to SPAWN");
                        break;
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int gridX = event.button.x / TILE_SIZE;
                int gridY = event.button.y / TILE_SIZE;

                LOG_DEBUG("mouse click at (%d, %d) -> grid (%d, %d)",
                          event.button.x, event.button.y, gridX, gridY);

                if (worldMap[gridY][gridX] == LIMIT) {
                    LOG_WARN("attempt to modify LIMIT tile at (%d, %d)", gridX, gridY);
                    break;
                }

                if (editor.currentSelection == SPAWN) {
                    if (editor.hasSpawn) {
                        LOG_WARN("spawn already exists, ignoring new spawn");
                        break;
                    }
                    editor.hasSpawn = true;
                    editor.spawnX = gridX;
                    editor.spawnY = gridY;
                    LOG_INFO("spawn set at (%d, %d)", gridX, gridY);
                }

                if (editor.currentSelection == EMPTY &&
                    gridX == editor.spawnX && gridY == editor.spawnY) {
                    editor.hasSpawn = false;
                    LOG_INFO("spawn removed from (%d, %d)", gridX, gridY);
                }

                worldMap[gridY][gridX] = editor.currentSelection;
                LOG_DEBUG("tile updated at (%d, %d) -> type=%d",
                          gridX, gridY, editor.currentSelection);
            }
        }

        render_grid(surface);
        SDL_UpdateWindowSurface(window);

        const char *sel_name;
        switch (editor.currentSelection) {
            case WALL:  sel_name = "[2] Parede";  break;
            case SPAWN: sel_name = "[3] Spawn";   break;
            default:    sel_name = "[1] Apagar";  break;
        }

        SDL_snprintf(title, sizeof(title),
            "RayGrid Editor | Selecionado: %s | [1] Apagar  [2] Parede  [3] Spawn  %s  [X] Sair",
            sel_name, editor.hasSpawn ? "[Enter] Jogar" : "[Enter] Jogar (precisa de Spawn)");

        SDL_SetWindowTitle(window, title);
    }

    LOG_INFO("shutting down");

    SDL_DestroyWindow(window);
    SDL_Quit();

    LOG_INFO("SDL terminated");
    log_close();

    return 0;
}