#pragma once
#include <iostream>
#include <fstream>
#include "map.h"




int saveMap(Map* map, std::string name) {

	std::string path;
	std::ofstream mapFile;
	
	path = "mapdata/" + name + ".xml";
	std::cout << "saving map under path " << path << "\n";
	
	mapFile.open(path);

	mapFile << "<?xml version \"1.0\" encoding=\"UTF-8\"?>\n";
	mapFile << "<map>\n";
	mapFile << "<name>" << map->name << "</name>\n";
	mapFile << "<width>" << std::to_string(map->w) << "</width>\n";
	mapFile << "<height>" << std::to_string(map->h) << "</height>\n";

	for (int x = 0; x < map->w; x++) {
		for (int y = 0; y < map->h; y++) {

			Tile currentTile = map->getTile(x, y);
			
			mapFile << "<tile x=\"" << std::to_string(x) << "\" y=\"" << std::to_string(y) << "\">\n";
			mapFile << "\t<terrain_texture>" << currentTile.terrainTexture << "</terrain_texture>\n";
			mapFile << "\t<terrain_mask>" << currentTile.terrainTexture << "</terrain_mask>\n";
			mapFile << "\t<upper_texture>" << currentTile.terrainTexture << "</upper_texture>\n";
			mapFile << "\t<upper_mask>" << currentTile.terrainTexture << "</upper_mask>\n";
			mapFile << "\t<flags>" << std::to_string(currentTile.flags) << "</flags>\n";
			mapFile << "\t<script>" << currentTile.script << "</script>\n";
			mapFile << "</tile>\n";
		}
	}

	mapFile << "</map>\n";

	mapFile.close();
	return 0;

}

Map loadMap(std::string path) {
	
}
