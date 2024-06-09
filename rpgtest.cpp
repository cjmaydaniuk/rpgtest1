#include "rpgtest.h"

enum WINDOW_PROPERTIES { WIDTH = 1440, HEIGHT = 1080 };

int main(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    Textures textures;

    SDL_Window* mainWindow = SDL_CreateWindow("GAME",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_PROPERTIES::WIDTH, WINDOW_PROPERTIES::HEIGHT, 0);

    // use hardware acceleration
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;



    // creates a renderer to render our images and creates a surface to load an image into the main memory
    SDL_Renderer* rend = SDL_CreateRenderer(mainWindow, -1, render_flags);
    initTextures(rend, &textures);

    // define a rectangular object for our character and apply our texture to it
    SDL_Rect character;
    SDL_Texture* currentTexture = textures.getTextureByName("texBaseLeft").texture;
    SDL_QueryTexture(currentTexture, NULL, NULL, &character.w, &character.h);





    // initialize character
    character.x = (WINDOW_PROPERTIES::WIDTH - character.w) / 2;
    character.y = (WINDOW_PROPERTIES::HEIGHT - character.h) / 2;
    int speed = 3; int horizontalDirection = 0; //0 = left, 1 = right

    // initialize main loop variables
    bool close = false;
    Uint32 delay = 1000 / 165; // 165 FPS
    while (!close) {
        SDL_Event event;
        // Events management
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { close = true;  }
        }
        currentTexture = monitorKeyboard(&character, &textures, speed, horizontalDirection);
        SDL_QueryTexture(currentTexture, NULL, NULL, &character.w, &character.h);


        checkBoundaries(&character);


        SDL_RenderClear(rend);         // clear the screen

        SDL_RenderCopy(rend, currentTexture, NULL, &character); // add character sprite to rendering queue

        SDL_RenderPresent(rend);  // update the screen with any queued rendering
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

//loads all textures into the textures vector
void initTextures(SDL_Renderer* rend, Textures* textures)
{
    //manual texture loading until I get to storing the list of textures in a data file, use xml?
    SDL_Texture* currentTexture = loadTexture(rend, "media/character/main_character_left.png");
    std::string name = "texBaseLeft";
    std::string description = "Character sprite facing left";
    textures->addTexture(currentTexture, name, description);

    currentTexture = loadTexture(rend, "media/character/main_character_right.png");
    name = "texBaseRight";
    description = "Character sprite facing left";
    textures->addTexture(currentTexture, name, description);

    currentTexture = loadTexture(rend, "media/character/main_character_swing_left.png");
    name = "texSwingLeft";
    description = "Character sprite facing left swinging sword";
    textures->addTexture(currentTexture, name, description);

    currentTexture = loadTexture(rend, "media/character/main_character_swing_right.png");
    name = "texSwingRight";
    description = "Character sprite facing left swinging sword";
    textures->addTexture(currentTexture, name, description);
}

// returns an SDL_Texture* object using an image file at path
SDL_Texture* loadTexture(SDL_Renderer* rend, const char* path) {
    SDL_Surface* surface;
    surface = IMG_Load(path);
    if (surface == NULL) {
        printf("Unable to load image");
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    return tex;
}

//handle keyboard interactions
SDL_Texture* monitorKeyboard(SDL_Rect* character, Textures* textures, int speed, int horizontalDirection) {
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
    int* h = &horizontalDirection;
    if (keyboardState[SDL_SCANCODE_RSHIFT] || keyboardState[SDL_SCANCODE_LSHIFT])    {
        speed *= 2;
    }

    
    if (keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP]) {
        character->y -= speed;
    }
    if (keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT]) {
        character->x -= (speed);
        *h = 0;
    }
    if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN]) {
        character->y += speed;
    }
    if (keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT]) {
        character->x += (speed);
        *h = 1;
    }
    
    if (keyboardState[SDL_SCANCODE_RETURN]) {
        if (*h == 0) { return textures->getTextureByName("texSwingLeft").texture; }
        if (*h == 1) { return textures->getTextureByName("texSwingRight").texture; }
    }
    if (!keyboardState[SDL_SCANCODE_RETURN]) {
        if (*h == 0) { return textures->getTextureByName("texBaseLeft").texture; }
        if (*h == 1) { return textures->getTextureByName("texBaseRight").texture; }
    }
}

// ensure character stays within screen boundaries
void checkBoundaries(SDL_Rect* character) {
    if (character->x + character->w > WINDOW_PROPERTIES::WIDTH)
        character->x = WINDOW_PROPERTIES::WIDTH - character->w;
    if (character->x < 0)
        character->x = 0;
    if (character->y + character->h > WINDOW_PROPERTIES::HEIGHT)
        character->y = WINDOW_PROPERTIES::HEIGHT - character->h;
    if (character->y < 0)
        character->y = 0;
}