#include "rpgtest.h"

enum WINDOW_PROPERTIES { WIDTH = 1440, HEIGHT = 1080 };

int main(int argc, char* argv[]) {

    Uint32 render_flags = SDL_RENDERER_ACCELERATED;     // use hardware acceleration
    Uint32 delay = 1000 / 165;
    Textures textures;
    Character character;
    bool close = false;

    //initialize window, renderer, and textures
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* mainWindow = SDL_CreateWindow("RPG TEST",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_PROPERTIES::WIDTH, WINDOW_PROPERTIES::HEIGHT, 0);
    SDL_Renderer* rend = SDL_CreateRenderer(mainWindow, -1, render_flags);

    initTextures(rend, &textures);
    initCharacter(&character, &textures);
            
    // main loop
    while (!close) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { close = true;  }
        }
        monitorKeyboard(&character, &textures);

        checkBoundaries(&character.object);

        drawFrame(rend, &character, &textures);
        SDL_Delay(delay);         // delay until next frame
           
    }

    // destroy texture, renderer, and window, then quit
    textures.destroyTextures();
    SDL_DestroyTexture(character.currentTexture);
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
void monitorKeyboard(Character* character, Textures* textures) {
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
    int speed = character->speed;

    //sprint
    if ((keyboardState[SDL_SCANCODE_RSHIFT] || keyboardState[SDL_SCANCODE_LSHIFT]))    {
        if (character->curStamina > 3) {
            speed *= 3;
            character->curStamina -= 3;
        }
    }
    //regain stamina if not sprinting
    else {
        character->curStamina += 1.5;
        if (character->curStamina > character->maxStamina) { character->curStamina = character->maxStamina; }
    }

    //movement related
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
    
    //attack
    if (keyboardState[SDL_SCANCODE_RETURN]) {
        if (character->direction == 0) { character->currentTexture = textures->getTextureByName("texSwingLeft").texture; }
        if (character->direction == 1) { character->currentTexture = textures->getTextureByName("texSwingRight").texture; }
    }
    if (!keyboardState[SDL_SCANCODE_RETURN]) {
        if (character->direction == 0) { character->currentTexture = textures->getTextureByName("texBaseLeft").texture; }
        if (character->direction == 1) { character->currentTexture = textures->getTextureByName("texBaseRight").texture; }
    }
    
    SDL_QueryTexture(character->currentTexture, NULL, NULL, character->getWidthPtr(), character->getHeightPtr());
}

// ensure character stays within screen boundaries
void checkBoundaries(SDL_Rect* characterObject) {
    if (characterObject->x + characterObject->w > WINDOW_PROPERTIES::WIDTH)
        characterObject->x = WINDOW_PROPERTIES::WIDTH - characterObject->w;
    if (characterObject->x < 0)
        characterObject->x = 0;
    if (characterObject->y + characterObject->h > WINDOW_PROPERTIES::HEIGHT)
        characterObject->y = WINDOW_PROPERTIES::HEIGHT - characterObject->h;
    if (characterObject->y < 0)
        characterObject->y = 0;
}

void initCharacter(Character* character, Textures* textures) {
    character->currentTexture = textures->getTextureByName("texBaseLeft").texture;
    character->maxHP = 250;
    character->curHP = 250;
    character->maxMP = 250;
    character->curMP = 250;
    character->maxStamina = 250; 
    character->curStamina = 250;
    character->setPosition((WINDOW_PROPERTIES::WIDTH - character->getWidth()) / 2, (WINDOW_PROPERTIES::HEIGHT - character->getHeight()) / 2);
    character->speed = 3; 
    character->direction = 0; //0 = left, 1 = right
}

void drawFrame(SDL_Renderer* rend, Character* character, Textures* textures) {
    
    SDL_Rect hpBarMax;  SDL_Rect hpBarCurrent; SDL_Rect hpBarBackground;
    SDL_Rect mpBarMax;  SDL_Rect mpBarCurrent; SDL_Rect mpBarBackground;
    SDL_Rect staminaBarMax; SDL_Rect staminaBarCurrent; SDL_Rect staminaBarBackground;
    
    //initialize hp bar
    hpBarBackground.x = 10; hpBarBackground.y = 10; hpBarBackground.h = 10; hpBarBackground.w = 150;
    hpBarMax.x = 10; hpBarMax.y = 10; hpBarMax.h = 10; hpBarMax.w = 150;
    hpBarCurrent.x = 10; hpBarCurrent.y = 10; hpBarCurrent.h = 10; hpBarCurrent.w = (int)((character->curHP/character->maxHP)*150);
    //initialize mp bar
    mpBarBackground.x = 10; mpBarBackground.y = 25; mpBarBackground.h = 10; mpBarBackground.w = 150;
    mpBarMax.x = 10; mpBarMax.y = 25; mpBarMax.h = 10; mpBarMax.w = 150;
    mpBarCurrent.x = 10; mpBarCurrent.y = 25; mpBarCurrent.h = 10; mpBarCurrent.w = (int)((character->curMP/character->maxMP)*150);
    //initialize stamina bar
    staminaBarBackground.x = 10; staminaBarBackground.y = 40; staminaBarBackground.h = 10; staminaBarBackground.w = 150;    staminaBarMax.x = 10; staminaBarMax.y = 40; staminaBarMax.h = 10; staminaBarMax.w = 150;
    staminaBarCurrent.x = 10; staminaBarCurrent.y = 40; staminaBarCurrent.h = 10; staminaBarCurrent.w = (int)((character->curStamina/character->maxStamina)*150);    

    SDL_RenderClear(rend);         // clear the screen

    //terrain layer goes here
    //under layer goes here

    SDL_RenderCopy(rend, character->currentTexture, NULL, character->getObjectPtr()); // player layer

    //upper layer goes here
    //potential second upper layer
    
    //UI elements
    //HP bar
    SDL_SetRenderDrawColor(rend, 75, 0, 0, 255);
    SDL_RenderFillRect(rend, &hpBarBackground);
    SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
    SDL_RenderDrawRect(rend, &hpBarMax);
    SDL_RenderFillRect(rend, &hpBarCurrent);
    //MP bar
    SDL_SetRenderDrawColor(rend, 0, 0, 75, 255);
    SDL_RenderFillRect(rend, &mpBarBackground);
    SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
    SDL_RenderDrawRect(rend, &mpBarMax);
    SDL_RenderFillRect(rend, &mpBarCurrent);
    //stamina bar
    SDL_SetRenderDrawColor(rend, 0, 75, 0, 255);
    SDL_RenderFillRect(rend, &staminaBarBackground);
    SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
    SDL_RenderDrawRect(rend, &staminaBarMax);
    SDL_RenderFillRect(rend, &staminaBarCurrent);
    //return render draw color to black
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);

    SDL_RenderPresent(rend);  // update the screen with any queued rendering
}