#pragma once
#include "GameObject.h"

class Sprite : public GameObject
{
public:
    Sprite(int rows, int columns, SDL_Texture *tex);
    bool getNextFrame();
    bool getFrame(int n);
    int getFrameCount();
    bool isPlaying = false;
    SDL_Rect srcRect;
private:
    int currentFrameIndex;
    int maxFrameIndex;
    int columns;
    int spriteWidth;
    int spriteHeight;
};