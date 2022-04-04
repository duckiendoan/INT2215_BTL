#pragma once
#include "Button.h"

class ToggleButton: public Button{
public:
    ToggleButton(SDL_Rect srcRect, SDL_Texture* tex, int numBtn);
    void update() override;
    void swapState();
};