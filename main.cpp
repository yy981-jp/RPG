#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <string>

#include <yy981/dll.h>

constexpr int TILE_SIZE = 32;
constexpr int MAP_WIDTH = 20;
constexpr int MAP_HEIGHT = 15;
constexpr int WIN_WIDTH = 640;
constexpr int WIN_HEIGHT = 480;

#include "inc/callTile.h"
#include "IMGtoCSV.h"

class Game {
public:
	Game() : window(nullptr), renderer(nullptr), running(false), tileManager(nullptr) {}

	bool init(const char* title, int width, int height) {
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
			std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
			return false;
		}

		if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
			std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
			return false;
		}

		window = SDL_CreateWindow(title,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			width, height, SDL_WINDOW_SHOWN);
		if (!window) {
			std::cerr << "CreateWindow Error: " << SDL_GetError() << std::endl;
			return false;
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (!renderer) {
			std::cerr << "CreateRenderer Error: " << SDL_GetError() << std::endl;
			return false;
		}

		// タイルマネージャ初期化（tileinfo.jsonを読み込む）
		try {
			tileManager = new TileManager(renderer, "src/link/ID.json");
		} catch (const std::exception& e) {
			std::cerr << "TileManager Error: " << e.what() << std::endl;
			return false;
		}

		// テスト用マップ（文字列ID）
		// DLL<std::vector<std::vector<int>>, std::string, std::string> IMGtoCSV(LoadLibrary("IMGtoCSV.dll"),"IMGtoCSV");
		map = IMGtoCSV("id","test");

		running = true;
		return true;
	}

	void run() {
		while (running) {
			handleEvents();
			update();
			render();
			SDL_Delay(16); // 約60FPS
		}
	}

	void cleanup() {
		if (tileManager) delete tileManager;
		if (renderer) SDL_DestroyRenderer(renderer);
		if (window) SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
	}

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool running;
	TileManager* tileManager;

	std::vector<std::vector<int>> map;

	void handleEvents() {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
		}
	}

	void update() {
		// 今は何もしない
	}

	void render() {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		for (int y = 0; y < MAP_HEIGHT; ++y) {
			for (int x = 0; x < MAP_WIDTH; ++x) {
				const int tileID = map[y][x];
				const TileInfo& tile = tileManager->getTile(tileID);

				SDL_Rect dstRect = {
					x * TILE_SIZE,
					y * TILE_SIZE,
					tile.rect.w,
					tile.rect.h
				};

				SDL_RenderCopy(renderer, tile.texture, &tile.rect, &dstRect);
			}
		}

		SDL_RenderPresent(renderer);
	}
};

int main(int argc, char* argv[]) {
	Game game;
	if (game.init("Map Test", WIN_WIDTH, WIN_HEIGHT)) {
		game.run();
		game.cleanup();
	}
	return 0;
}
