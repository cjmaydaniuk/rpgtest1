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
    Character character;
    initCharacter(&character);

    SDL_Texture* currentTexture = textures.getTextureByName("texBaseLeft").texture;
    SDL_QueryTexture(currentTexture, NULL, NULL, &character.object.w, &character.object.h);
    
    // initialize stamina bar
    SDL_Rect staminaBarMax;
    SDL_Rect staminaBarCurrent;
    staminaBarMax.x = 10; staminaBarMax.y = 10; staminaBarMax.h = 10; staminaBarMax.w = character.maxStamina;
    staminaBarCurrent.x = 10; staminaBarCurrent.y = 10; staminaBarCurrent.h = 10; staminaBarCurrent.w = character.curStamina;





    // initialize character
    
    // initialize main loop variables
    bool close = false;
    Uint32 delay = 1000 / 165; // 165 FPS
    while (!close) {
        SDL_Event event;
        // Events management
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { close = true;  }
        }
        currentTexture = monitorKeyboard(&character, &textures);
        SDL_QueryTexture(currentTexture, NULL, NULL, &character.object.w, &character.object.h);


        checkBoundaries(&character.object);


        SDL_RenderClear(rend);         // clear the screen

        SDL_RenderCopy(rend, currentTexture, NULL, &character.object); // add character sprite to rendering queue
        SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
        staminaBarMax.w = character.maxStamina;
        SDL_RenderDrawRect(rend, &staminaBarMax);
        staminaBarCurrent.w = character.curStamina;
        SDL_RenderFillRect(rend, &staminaBarCurrent);
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        
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
SDL_Texture* monitorKeyboard(Character* character, Textures* textures) {
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

    int speed = character->speed;
    if ((keyboardState[SDL_SCANCODE_RSHIFT] || keyboardState[SDL_SCANCODE_LSHIFT]) && character->curStamina > 10)    {
        speed *= 3;
        character->curStamina -= 3;
    }
    else {
        character->curStamina += 1.5;
        if (character->curStamina > character->maxStamina) { character->curStamina = character->maxStamina; }
    }

    
    if (keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP]) {
        character->move(0, -speed);
    }
    if (keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT]) {
        character->move(-speed, 0);
        character->direction = 0;
    }
    if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN]) {
        character->move(0, speed);
    }
    if (keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT]) {
        character->move(speed, 0);
        character->direction = 1;
    }
    
    if (keyboardState[SDL_SCANCODE_RETURN]) {
        if (character->direction == 0) { return textures->getTextureByName("texSwingLeft").texture; }
        if (character->direction == 1) { return textures->getTextureByName("texSwingRight").texture; }
    }
    if (!keyboardState[SDL_SCANCODE_RETURN]) {
        if (character->direction == 0) { return textures->getTextureByName("texBaseLeft").texture; }
        if (character->direction == 1) { return textures->getTextureByName("texBaseRight").texture; }
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

void initCharacter(Character* character) {
    character->maxStamina = 250; 
    character->curStamina = 250;
    character->setPosition((WINDOW_PROPERTIES::WIDTH - character->getWidth()) / 2, (WINDOW_PROPERTIES::HEIGHT - character->getHeight()) / 2);
    character->speed = 3; 
    character->direction = 0; //0 = left, 1 = right

}