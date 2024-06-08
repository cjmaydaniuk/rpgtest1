#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include "SDL\SDL.h"
#include "SDL\SDL_image.h"
#include "SDL\SDL_timer.h"
#include "texture_classes.h"

void initTextures(SDL_Renderer* rend, Textures* textures);
SDL_Texture* loadTexture(SDL_Renderer* rend, const char* path);