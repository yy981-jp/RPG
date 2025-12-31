#define SDL_MAIN_HANDLED
#include <iostream>
#include <vector>
#include <string>

#include <yy981/dll.h>

#include "game.h"

int main(int argc, char* argv[]) {
	Game game;
	if (game.init("Map Test", cst::winWidth, cst::winHeight)) {
		game.run();
		game.cleanup();
	}
	return 0;
}
