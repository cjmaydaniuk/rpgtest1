#pragma once
#include <iostream>
#include <fstream>
#include <string>
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

Map loadMap(std::string name) {

	Map currentMap;

	std::string path;
	std::ifstream mapFile;
	std::string line;

	path = "mapdata/" + name + ".xml";
	std::cout << "loading map at path " << path << "\n";

	mapFile.open(path);

	if (mapFile.is_open()) {

		std::getline(mapFile, line);
		std::cout << "map file " << path << " is open.\n";
		std::cout << line << "\n";
		// at least confirm that the file contains the XML header before running the main loop
		if (line == "<?xml version \"1.0\" encoding=\"UTF-8\"?>") {
			std::cout << "detected XML file\n";
			while (std::getline(mapFile, line)) {

				std::string tag = getTag(line);
				
				if (tag == "map") {
					std::getline(mapFile, line); currentMap.name = getTagContents(line);
					std::getline(mapFile, line); currentMap.w = std::stoi(getTagContents(line));
					std::getline(mapFile, line); currentMap.h = std::stoi(getTagContents(line));
					currentMap.initTiles(currentMap.w, currentMap.h);
					std::cout << "Tiles initialized\n";
				}
			
				




				if (tag.find("tile") != std::string::npos && tag.find("/tile") == std::string::npos) {

					//first we need to get the x,y positions
					size_t pos1 = line.find("x=");
					size_t pos2 = line.find("y=");
					size_t pos3 = line.find(">");

					int x = std::stoi(line.substr(pos1 + 3, pos2 - pos1 - 5));
					int y = std::stoi(line.substr(pos2 + 3, pos3 - pos2 - 4));
					
					while (getTag(line) != "/tile") {
						std::getline(mapFile, line);
						tag = getTag(line);

						if (tag == "terrain_texture") { currentMap.tiles[x][y].terrainTexture = getTagContents(line); }
						if (tag == "terrain_mask") { currentMap.tiles[x][y].terrainMask = getTagContents(line); }
						if (tag == "upper_texture") { currentMap.tiles[x][y].upperTexture = getTagContents(line); }
						if (tag == "upper_mask") { currentMap.tiles[x][y].upperMask = getTagContents(line); }
						if (tag == "flags") { currentMap.tiles[x][y].flags = std::stol(getTagContents(line)); }
						if (tag == "script") { currentMap.tiles[x][y].script = getTagContents(line); }

					}
					
					

				}



			}

		}
		else { 
			std::cout << "error! contents do not match XML header\n"; 
		}
	}
	else { std::cout << "unable to open map at path " << path << ".\n"; }


	return currentMap;


}

std::string getTag(std::string line) {

	size_t pos1 = line.find("<");
	size_t pos2 = line.find(">");
	return line.substr(pos1+1, pos2 - pos1 -1);

}

std::string getTagContents(std::string line) {

	//as I don't use any multi-line tags, I only need to be able to find the contents between > and </

	size_t pos1 = line.find(">");
	size_t pos2 = line.find("</");
	return line.substr(pos1 + 1, pos2 - pos1 - 1);

}