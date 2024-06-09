#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include "SDL\SDL.h"
#include "SDL\SDL_image.h"
#include "SDL\SDL_timer.h"
#include "texture_classes.h"
#include "character_classes.h"

void initTextures(SDL_Renderer* rend, Textures* textures);
void initCharacter(Character* character);
SDL_Texture* loadTexture(SDL_Renderer* rend, const char* path);
SDL_Texture* monitorKeyboard(Character* character, Textures* textures);
void checkBoundaries(SDL_Rect* character);

