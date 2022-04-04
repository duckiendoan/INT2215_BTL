#pragma once
#include "GameObject.h"

enum ButtonState
{
    BUTTON_STATE_DEFAULT,
    BUTTON_STATE_HOVER
};

class Button: public GameObject
{
public:
    Button(SDL_Rect srcRect, SDL_Texture* tex, int numBtn);

    virtual void update();
    ButtonState state;
    SDL_Rect srcRect;
protected:
    int originalWidth;
};