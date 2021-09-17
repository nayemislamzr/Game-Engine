#include <iostream>

#include "Game.h"

Game* game = new Game(1280, 700, "Hello World");

void mouseMoveListener(GLFWwindow* window, double pos_X, double pos_Y)
{
	std::cout << pos_X << ' ' << pos_Y << std::endl;
	if (pos_Y == 1.0f)
	{
		l -= 0.1f;
		r += 0.1f;
		d += 0.1f;
		u -= 0.1f;
	}
	else if (pos_Y == -1.0f)
	{
		l += 0.1f;
		r -= 0.1f;
		d -= 0.1f;
		u += 0.1f;
	}
}

int main(int argc, char** argv)
{
	game->init();
	game->addEventListener(InputEvent::MOUSE_SCROLL, mouseMoveListener);
	while (game->running())
	{
		game->handleEvent();
		game->update();
		game->draw();
	}
	game->destroy();
	return 0;
}