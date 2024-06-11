#pragma once
#include <string>
#include "class_map.h"

int saveMap(Map* map, std::string name);
Map loadMap(std::string path);