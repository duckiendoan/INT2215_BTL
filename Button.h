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
    void update();
    ButtonState state;
    SDL_Rect srcRect;
private:
    int originalWidth;
};