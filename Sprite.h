#pragma once
#include "GameObject.h"

class Sprite : public GameObject
{
public:
    Sprite(SDL_Rect pSrcRect, SDL_Texture* tex);
    SDL_Rect srcRect;
};