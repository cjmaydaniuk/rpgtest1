#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>




class Tile {

public:
	Tile() = default;
	std::string terrainTexture;
	std::string terrainMask;
	std::string upperTexture;
	std::string upperMask;
	unsigned long flags;
	std::string script;
};

class Map {

public:
	Map(std::string mapName, int width, int height) {
		name = mapName;
		w = width;
		h = height;
		initTiles(w, h);
	}
	Map() = default;


	std::vector <std::vector <Tile>> tiles;
	std::string name;
	int w; int h;

	Tile getTile(int x, int y) {
		if (x <= w && y <= h) { return tiles[x][y]; }
		printf("[ERROR] : Tile out of range \n");
	}

	int setTile(int x, int y, std::string tTex, std::string tMask, std::string uTex, std::string uMask, unsigned long f, std::string s) {
		if (x <= w && y <= h) {
			tiles[x][y].terrainTexture = tTex;
			tiles[x][y].terrainMask = tTex;
			tiles[x][y].upperTexture = tTex;
			tiles[x][y].upperMask = tTex;
			tiles[x][y].flags = f;
			tiles[x][y].script = s;
			return 0;
		}
		printf("[ERROR] : Tile out of range \n");
		return 1;
	}

	void initTiles(int width, int height) {
		w = width; h = height;
		
		tiles.resize(w, std::vector<Tile>(h));
		
		//initialize all tiles to grass
		for (int x = 0; x < w; x++)	{
			for (int y = 0; y < h; y++) {
				setTile(x, y, "texGrass1", "", "", "", 0, "");
			}
		}
	}

};