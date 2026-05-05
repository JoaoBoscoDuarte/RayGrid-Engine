#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "config.h"
#include "map.h"

static void render_grid(SDL_Surface *surface) {
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
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erro SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "RayGrid - Editor",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        printf("Erro ao criar janela: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface *surface = SDL_GetWindowSurface(window);
    EditorState editor = { .currentSelection = WALL, .hasSpawn = false };
    map_load(&editor);

    bool running = true;
    SDL_Event event;
    char title[128];

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_x:     running = false; break;
                    case SDLK_RETURN:
                        if (editor.hasSpawn) {
                            map_save(&editor);
                            running = false; // TODO: iniciar jogo
                        }
                        break;
                    case SDLK_1: editor.currentSelection = EMPTY; break;
                    case SDLK_2: editor.currentSelection = WALL;  break;
                    case SDLK_3: editor.currentSelection = SPAWN; break;
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int gridX = event.button.x / TILE_SIZE;
                int gridY = event.button.y / TILE_SIZE;

                // Impede sobrescrever as bordas
                if (worldMap[gridY][gridX] == LIMIT) break;

                // SPAWN: só um permitido
                if (editor.currentSelection == SPAWN) {
                    if (editor.hasSpawn) break;
                    editor.hasSpawn = true;
                    editor.spawnX = gridX;
                    editor.spawnY = gridY;
                }

                // Remove spawn anterior se estiver apagando a célula
                if (editor.currentSelection == EMPTY &&
                    gridX == editor.spawnX && gridY == editor.spawnY) {
                    editor.hasSpawn = false;
                }

                worldMap[gridY][gridX] = editor.currentSelection;
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

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
