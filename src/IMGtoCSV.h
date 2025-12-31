#include <string>
#include <vector>

#include "def.h"

std::string to_hex(int r, int g, int b);
std::vector<std::vector<int>> IMGtoCSV(std::string json_path, std::string image_path);
