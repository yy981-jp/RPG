#include "def.h"
#include "tileManager.h"


class Game {
public:
	Game() : window(nullptr), renderer(nullptr), running(false), tileManager(nullptr) {}

	bool init(const char* title, int width, int height); 
	void run();
	void cleanup();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool running;
	TileManager* tileManager;
	std::vector<std::vector<int>> map;

	void handleEvents();

	void update() {
		// 今は何もしない
	}

	void render();
};
