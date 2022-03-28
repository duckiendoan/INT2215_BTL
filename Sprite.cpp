#include "Sprite.h"

Sprite::Sprite(SDL_Rect pSrcRect, SDL_Texture *tex) : srcRect(pSrcRect) {
    texture = tex;
    currentFrame.x = 0;
    currentFrame.y = 0;
    //SDL_QueryTexture(tex, nullptr, nullptr, &currentFrame.w, &currentFrame.h);
    currentFrame.w = srcRect.w;
    currentFrame.h = srcRect.h;
    shown = true;
}
