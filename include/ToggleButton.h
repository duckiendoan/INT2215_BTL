#pragma once
#include "Button.h"

class ToggleButton: public Button{
public:
    ToggleButton(SDL_Rect srcRect, SDL_Texture* tex);
    void update() override;
    void swapState();
};