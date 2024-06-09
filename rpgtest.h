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
void initCharacter(Character* character, Textures* textures);
SDL_Texture* loadTexture(SDL_Renderer* rend, const char* path);
void checkBoundaries(SDL_Rect* characterObject);
void drawFrame(SDL_Renderer* rend, Character* character, Textures* textures);
void monitorKeyboard(Character* character, Textures* textures);