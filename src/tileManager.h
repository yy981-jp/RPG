#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct TileInfo {
	SDL_Texture* texture = nullptr;
	SDL_Rect rect{};
};

class TileManager {
public:
	TileManager(SDL_Renderer* renderer, const std::string& jsonPath): renderer(renderer) {
		load(jsonPath);
	}

	~TileManager() {
		for (auto& [_, tex] : sheetCache) {
			SDL_DestroyTexture(tex);
		}
	}

	const TileInfo& getTile(const int& tileID) const {
		return tileMap.at(tileID);
	}

private:
	SDL_Renderer* renderer;
	std::unordered_map<std::string, SDL_Texture*> sheetCache;
	std::unordered_map<int, TileInfo> tileMap;

	void load(const std::string& path) {
		std::ifstream ifs(path);
		if (!ifs) throw std::runtime_error("タイルJSONが開けません");
		json j;
		ifs >> j;

		for (auto& [tileID, entry] : j.items()) {
			std::string sheetPath = "assets/tiles/" + std::string(entry["sheet"]);
			std::vector<int> index = entry["index"];
			std::vector<int> size = entry.value("size", std::vector<int>{1,1});

			SDL_Texture* tex = loadTexture(sheetPath);
			SDL_Rect srcRect = {
				index[0] * cst::tileSize,
				index[1] * cst::tileSize,
				size[0] * cst::tileSize,
				size[1] * cst::tileSize
			};

			tileMap[std::stoi(tileID)] = TileInfo{ tex, srcRect };
		}
	}

	SDL_Texture* loadTexture(const std::string& path) {
		if (sheetCache.count(path)) return sheetCache[path];

		SDL_Surface* surface = IMG_Load(path.c_str());
		if (!surface) throw std::runtime_error("画像の読み込みに失敗: " + path);

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		if (!texture) throw std::runtime_error("テクスチャ作成失敗: " + path);

		sheetCache[path] = texture;
		return texture;
	}
};
