#pragma once

class Texture {

public:
    Texture(SDL_Texture* t, std::string n, std::string d) {
        texture = t;
        name = n;
        description = d;
    }
    Texture() = default;
    SDL_Texture* texture;
    std::string name;
    std::string description;

};

class Textures {


public:
    std::vector<Texture> tex;

    Textures() = default;
    void addTexture(SDL_Texture* t, std::string n, std::string d) {
        Texture currentTex(t, n, d);
        tex.push_back(currentTex);
    }
    Texture getTextureByName(std::string name) {
        for (int i = 0; i < (int)tex.size(); i++)
        {
            if (tex[i].name == name) {
                return tex[i];
            }
        }
    }
    void destroyTextures() {
        for (int i = 0; i < (int)tex.size(); i++)
        {
            SDL_DestroyTexture(tex[i].texture);
        }
    }

    
};
