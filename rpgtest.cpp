#include "rpgtest.h"
#include <string>




int main(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    Textures textures;

    SDL_Window* mainWindow = SDL_CreateWindow("GAME",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1000, 1000, 0);

    // use hardware acceleration
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    // creates a renderer to render our images and creates a surface to load an image into the main memory
    SDL_Renderer* rend = SDL_CreateRenderer(mainWindow, -1, render_flags);

    initTextures(rend, &textures);
    SDL_Texture* currentTexture = textures.getTextureByName("texBase").texture;


    // define a rectangular object for our character and apply our texture to it
    SDL_Rect character;
    SDL_QueryTexture(textures.getTextureByName("texBase").texture, NULL, NULL, &character.w, &character.h);

    // initialize character
    character.x = (1000 - character.w) / 2;
    character.y = (1000 - character.h) / 2;
    int speed = 3;

    // initialize main loop variables
    bool close = false;
    Uint32 delay = 1000 / 165; // 165 FPS
    while (!close) {
        SDL_Event event;
        // Events management
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { close = true;  }
        }
        // handle keyboard interaction
        const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
        if (keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP]) {
            character.y -= speed;
        }
            if (keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT]) {
                character.x -= (speed);
            }
            if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN]) {
                character.y += speed;
            }
            if (keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT]) {
                character.x += (speed);
            }
            if (keyboardState[SDL_SCANCODE_RETURN])
                currentTexture = textures.getTextureByName("texSwing").texture;
            if (!keyboardState[SDL_SCANCODE_RETURN])
                currentTexture = textures.getTextureByName("texBase").texture;
                
                
                
            SDL_QueryTexture(currentTexture, NULL, NULL, &character.w, &character.h);


            // ensure character stays within screen boundaries
            if (character.x + character.w > 1000)
                character.x = 1000 - character.w;
            if (character.x < 0)
                character.x = 0;
            if (character.y + character.h > 1000)
                character.y = 1000 - character.h;
            if (character.y < 0)
                character.y = 0;

            // clear the screen
            SDL_RenderClear(rend);

            SDL_RenderCopy(rend, currentTexture, NULL, &character);

            SDL_RenderPresent(rend);  // update the screen with any buffered rendering
            SDL_Delay(delay);         // 165 fps
           
        }

        // destroy texture, renderer, and window, then quit
        
        textures.destroyTextures();
        SDL_DestroyTexture(currentTexture);
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(mainWindow);
        SDL_Quit();

        return 0;
}

void initTextures(SDL_Renderer* rend, Textures* textures)
{
    printf("loading textures");
    SDL_Texture* currentTexture = loadTexture(rend, "media/character/main_character.png");
    std::string name = "texBase";
    std::string description = "Character sprite facing left";
    textures->addTexture(currentTexture, name, description);
    printf("loading second texture");
    currentTexture = loadTexture(rend, "media/character/main_character_swing.png");
    name = "texSwing";
    description = "Character sprite facing left swinging sword";
    textures->addTexture(currentTexture, name, description);
}

SDL_Texture* loadTexture(SDL_Renderer* rend, const char* path) {
    SDL_Surface* surface;
    surface = IMG_Load(path);
    if (surface == NULL) {
        printf("Unable to load image");
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    printf("created texture");
    return tex;
}