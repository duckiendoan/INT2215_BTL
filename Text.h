#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Common.h"
#include "GameObject.h"

class Text : public GameObject
{
public:
    Text(TTF_Font* font, std::string& text, SDL_Color color);
    ~Text();
    void initTexture(SDL_Renderer* renderer);
private:
    SDL_Surface* textSurface;
};