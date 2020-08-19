#include "Game.hpp"
#include "InputController.h"

Game* game = nullptr;
InputController* input = nullptr;

int main(int argc, char* argv[]) {
	const int FPS = 120;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	game = new Game();
	input = new InputController(game);
	game->init("Sand Sim", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, false);

	// Game Loop
	while (game->running()) {
		frameStart = SDL_GetTicks();
		input->update();
		game->handleEvents();
		game->render();
		game->update();
		
		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game->clean();
	return 0;
}