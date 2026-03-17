#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

int main(int argc, char** args) {
	
	// Ponteiros usados
	SDL_Surface* winSurface = NULL;
	SDL_Window* window = NULL;

	// Inicializando o SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Erro Initializing SDL: %s\n", SDL_GetError());
		system("Pause");
		return 1;
	}

	// Criando uma janela
	window = SDL_CreateWindow( 
		"Teste", 
		SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED, 
		640, 
		480, 
		SDL_WINDOW_SHOWN
	);

	// Verifica que a janela foi criada
	if (!window) {
        printf("Erro ao criar janela: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

	// Pega o surface da janela
	winSurface = SDL_GetWindowSurface(window);
	
	// Preenche com uma cor (cinza) 128, 128, 128
	Uint32 color = SDL_MapRGB(winSurface->format, 128, 128, 128);
	SDL_UpdateWindowSurface(window);

	// Atualiza janela
	SDL_UpdateWindowSurface(window);

	// Loop de exibição da janela
	bool running = true;
	SDL_Event event;

	while (running) {
		while (SDL_PollEvent(&event)) {

			// Fecha clicando no botão da janela
			if (event.type == SDL_QUIT) {
				running = false;
			}

			// Teclado
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_x) {
					running = false;
				}
			}
		}
	}

	// Limpeza
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

