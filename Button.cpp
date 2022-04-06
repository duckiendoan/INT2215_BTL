#include "Button.h"

Button::Button(SDL_Rect p_srcRect, SDL_Texture *tex) : srcRect(p_srcRect) {
    texture = tex;
    originalWidth = p_srcRect.w;
    state = BUTTON_STATE_DEFAULT;

    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = srcRect.w;
    currentFrame.h = srcRect.h;
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