#pragma once
#include "Sprite.h"
#include <vector>

class Spritesheet
{
public:
    Spritesheet(int rows, int columns, SDL_Texture* tex);
    Sprite& GetSprite(int n);
    int spriteWidth, spriteHeight;
    int spriteCount;
private:
    std::vector<Sprite> sprites;
};