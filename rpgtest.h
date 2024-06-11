#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include "SDL\SDL.h"
#include "SDL\SDL_image.h"
#include "SDL\SDL_timer.h"
#include "class_texture.h"
#include "class_character.h"
#include "class_map.h"
#include "map.h"

enum WINDOW_PROPERTIES { WIDTH = 1440, HEIGHT = 1080 };

void initTextures(SDL_Renderer* rend, Textures* textures);
void initCharacter(Character* character, Textures* textures);
SDL_Texture* loadTexture(SDL_Renderer* rend, const char* path);
void checkBoundaries(SDL_Rect* characterObject);
void drawFrame(SDL_Renderer* rend, Character* character, Textures* textures, Map* map);
void monitorKeyboard(Map* map, Character* character, Textures* textures);
void drawLayer(SDL_Renderer* rend, Map* map, Textures* textures, std::string layer);
SDL_Texture* getLayer(SDL_Renderer* rend, Map* map, Textures* textures, std::string layer);