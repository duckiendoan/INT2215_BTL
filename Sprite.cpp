#include "Sprite.h"

Sprite::Sprite(int rows, int columns, SDL_Texture *tex) : columns(columns) {
    SDL_QueryTexture(tex, nullptr, nullptr, &spriteWidth, &spriteHeight);
    spriteWidth /= columns;
    spriteHeight /= rows;
    currentFrameIndex = -1;
    maxFrameIndex = columns * rows - 1;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = spriteWidth;
    srcRect.h = spriteHeight;

    texture = tex;
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = srcRect.w;
    currentFrame.h = srcRect.h;
    shown = true;
}

bool Sprite::getNextFrame() {
    currentFrameIndex++;
    if (currentFrameIndex > maxFrameIndex) {
        currentFrameIndex = 0;
        return false;
    }
    int nRow = currentFrameIndex / columns;
    int nCol = currentFrameIndex % columns;
    srcRect.x = nCol * spriteWidth;
    srcRect.y = nRow * spriteHeight;
    return true;
}

bool Sprite::getFrame(int frameIndex) {
    if (frameIndex < 0 || frameIndex > maxFrameIndex) return false;
    if (frameIndex == currentFrameIndex) return true;
    currentFrameIndex = frameIndex;
    int nRow = frameIndex / columns;
    int nCol = frameIndex % columns;
    srcRect.x = nCol * spriteWidth;
    srcRect.y = nRow * spriteHeight;
    return true;
}

int Sprite::getFrameCount() {
    return maxFrameIndex + 1;
}
