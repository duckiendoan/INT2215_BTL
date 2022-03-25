#pragma once
#include "GameObject.h"

enum ButtonState
{
    BUTTON_STATE_DEFAULT,
    BUTTON_STATE_HOVER,
    BUTTON_STATE_CLICKED
};

class Button: public GameObject
{
public:
    Button(SDL_Rect srcRect, SDL_Texture* tex, int numBtn);
    void update();
    ButtonState getState();
    SDL_Rect srcRect;
private:
    ButtonState state;
    int originalWidth;
};