#include "Spritesheet.h"

Spritesheet::Spritesheet(int rows, int columns, SDL_Texture *tex) {
    spriteCount = rows * columns;
    sprites.reserve(rows * columns);
    //int width, height;
    SDL_QueryTexture(tex, nullptr, nullptr, &spriteWidth, &spriteHeight);
    spriteWidth /= columns;
    spriteHeight /= rows;

    for (int i = 0; i < rows * columns; i++)
    {
        int nRow = i / columns;
        int nCol = i % columns;
        SDL_Rect src = {nCol * spriteWidth, nRow * spriteHeight, spriteWidth, spriteHeight};
        sprites.emplace_back(src, tex);
    }
}

Sprite &Spritesheet::GetSprite(int n) {
    return sprites[n];
}
