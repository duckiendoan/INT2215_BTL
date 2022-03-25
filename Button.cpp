#include "Button.h"

Button::Button(SDL_Rect p_srcRect, SDL_Texture *tex, int numBtn = 1) : srcRect(p_srcRect) {
    texture = tex;
    originalWidth = p_srcRect.w;
    state = BUTTON_STATE_DEFAULT;

    currentFrame.x = 0;
    currentFrame.y = 0;
    SDL_QueryTexture(tex, nullptr, nullptr, &currentFrame.w, &currentFrame.h);
    currentFrame.w /= 2;
    currentFrame.h /= numBtn;
    shown = true;
}

void Button::update()
{
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    if ((mouse_x >= x && mouse_x < x + getWidth()) && (mouse_y >= y && mouse_y < y + getHeight())) {
        state = BUTTON_STATE_HOVER;
        srcRect.x = originalWidth;
    } else {
        state = BUTTON_STATE_DEFAULT;
        if (srcRect.x == originalWidth) {
            srcRect.x -= originalWidth;
        }
    }

}

ButtonState Button::getState()
{
    return state;
}