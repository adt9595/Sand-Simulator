#include "InputController.h"

bool InputController::leftMouseDown = false;
bool InputController::rightMouseDown = false;

InputController::InputController(Game* game) :game(game) {};

void InputController::update() {
	handleClick();

	if (game->event.type == SDL_KEYDOWN) {
		switch (game->event.key.keysym.sym) {
			case SDLK_0:
				game->changeCurrentElement(0);
				std::cout << game->getCurrentElementName() << std::endl;
				break;
			case SDLK_1:
				game->changeCurrentElement(1);
				std::cout << game->getCurrentElementName() << std::endl;
				break;
			case SDLK_2:
				game->changeCurrentElement(2);
				std::cout << game->getCurrentElementName() << std::endl;
				break;
			case SDLK_3:
				game->changeCurrentElement(3);
				std::cout << game->getCurrentElementName() << std::endl;
				break;
			case SDLK_4:
				game->changeCurrentElement(4);
				std::cout << game->getCurrentElementName() << std::endl;
				break;
			case SDLK_c:
				game->clearScreen();
				break;
			case SDLK_o:
				game->changeBrushSize(-1);
				break;
			case SDLK_p:
				game->changeBrushSize(1);
				break;
			default:
				break;
		}
	}

	/*
	if (game->event.type == SDL_KEYUP) {

	}*/

	if (game->event.type == SDL_MOUSEBUTTONDOWN) {
		if (game->event.button.button == SDL_BUTTON_LEFT) {
			rightMouseDown = false;
			leftMouseDown = true;
			game->erasing = false;
		}
		else if (game->event.button.button == SDL_BUTTON_RIGHT) {
			leftMouseDown = false;
			rightMouseDown = true;
			game->erasing = true;
		}
	}

	if (game->event.type == SDL_MOUSEBUTTONUP) {
		if (game->event.button.button == SDL_BUTTON_LEFT) {
			leftMouseDown = false;
			game->resetBrushPoints();
		}
		else if (game->event.button.button == SDL_BUTTON_RIGHT) {
			rightMouseDown = false;
			game->erasing = false;
			game->resetBrushPoints();
		}
	}
}



void InputController::handleClick() {
	int x, y;
	SDL_GetMouseState(&x, &y);

	if (leftMouseDown) {
		game->placeElement(x, y);
	}
	else if (rightMouseDown) {
		game->placeElement(x, y);
	}
	
}



