#pragma once

class Character {

public:
    SDL_Rect object;
    SDL_Texture* currentTexture;
    int speed;
    int direction;
    double curHP;    double maxHP;
    double curMP;    double maxMP;
    double curStamina;    double maxStamina;

    Character(SDL_Rect obj, int s, int d) {
        object = obj;
        speed = s;
        direction = d;
    }
    Character() = default;
    void move(int xSpeed, int ySpeed) {
        object.x += xSpeed;
        object.y += ySpeed;
    }
    void setPosition(int x, int y) {
        object.x = x;
        object.y = y;
    }
    int getWidth() {
        return object.w;
    }
    int* getWidthPtr() {
        return &object.w;
    }
    int getHeight() {
        return object.h;
    }
    int* getHeightPtr() {
        return &object.h;
    }
    SDL_Rect* getObjectPtr() {
        return &object;
    }
    SDL_Rect getObject() {
        return object;
    }

};