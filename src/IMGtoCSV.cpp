#include "IMGtoCSV.h"

#define SDL_MAIN_HANDLED
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include <_/stb_image.h>

#include "def.h"

// HEX文字列に変換 (#RRGGBB)
std::string to_hex(int r, int g, int b) {
	std::ostringstream oss;
	oss << "#" << std::uppercase << std::setfill('0') << std::hex
		<< std::setw(2) << r
		<< std::setw(2) << g
		<< std::setw(2) << b;
	return oss.str();
}


std::vector<std::vector<int>> IMGtoCSV(std::string json_path, std::string image_path) {
	image_path = "assets/map/" + image_path + ".png";
	json_path = "assets/link/" + json_path + ".json";
	// JSON読み込み
	std::ifstream ifs(json_path);
	if (!ifs) {
		std::cerr << "Failed to open JSON: " << json_path << "\n";
		return {{}};
	}
	json j;
	ifs >> j;

	// HEX -> IDのマップ作成
	std::unordered_map<std::string, int> color_to_id;
	for (auto& [key, value] : j.items()) {
		// if (!value.contains("id")) continue;
		color_to_id[value["color"]] = std::stoi(key);
	}
	std::cout << "color_to_id.size: " << color_to_id.size() << "\n";

	// PNG読み込み
	int w, h, channels;
	unsigned char* data = stbi_load(image_path.c_str(), &w, &h, &channels, 3); // RGB
	if (!data) {
		std::cerr << "Failed to load image: " << image_path << "\n";
		return {{}};
	}

	// CSV出力
	std::vector<std::vector<int>> result(h);

	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			int idx = (y * w + x) * 3;
			int r = data[idx];
			int g = data[idx + 1];
			int b = data[idx + 2];
			std::string hex = to_hex(r, g, b);

			if (color_to_id.find(hex) != color_to_id.end()) {
				result[y].emplace_back(color_to_id[hex]);
			} else throw std::runtime_error("IDリストに登録されていない色が変換元画像に使用されています HEX:" + hex);
			// if (x < w - 1) ofs << ",";
		}
	}
	stbi_image_free(data);
	return result;
}
