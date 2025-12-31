#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace cst {
    constexpr int tileSize =    32,
                  mapWidth =    20,
                  mapHeight =   15,
                  winWidth =    640,
                  winHeight =   480;
}