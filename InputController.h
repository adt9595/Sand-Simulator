#pragma once

#include "Game.hpp"

class InputController{
public:
	InputController(Game* game);

	void update();
	void handleClick();
	bool isLeftMouseDown() { return leftMouseDown; };
	bool isRightMouseDown() { return rightMouseDown; };
	

private:
	Game* game;
	static bool leftMouseDown;
	static bool rightMouseDown;
};