#pragma once
#include "RenderWindow.h"

class Animator {
public:
    explicit Animator(RenderWindow& window);
    void updateAnimation();
    void startAnimation(float& x, float& y);
    void renderAnimation();
private:
    RenderWindow& window;
    SDL_Texture* fireworkTexture;
    Sprite* fireworkSprite;
};