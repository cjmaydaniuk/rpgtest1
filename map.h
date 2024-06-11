#pragma once
#include <string>
#include "class_map.h"

int saveMap(Map* map, std::string name);
Map loadMap(std::string path);
std::string getTag(std::string line);
std::string getTagContents(std::string line);