#include "ToggleButton.h"

void ToggleButton::update()
{
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    if ((mouse_x >= x && mouse_x < x + getWidth()) && (mouse_y >= y && mouse_y < y + getHeight())) {
        state = BUTTON_STATE_HOVER;

    } else {
        state = BUTTON_STATE_DEFAULT;
    }
}

void ToggleButton::swapState()
{
    if (srcRect.x == 0)
    {
        srcRect.x = originalWidth;
    }
    else if (srcRect.x == originalWidth)
    {
        srcRect.x -= originalWidth;
    }
}

ToggleButton::ToggleButton(SDL_Rect p_srcRect, SDL_Texture *tex, int numBtn) : Button(p_srcRect, tex, numBtn) {

}
