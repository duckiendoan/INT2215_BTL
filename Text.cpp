#include "Text.h"
#include <iostream>

Text::Text(TTF_Font* font, std::string& text, SDL_Color color) {
    textSurface = TTF_RenderText_Solid(font, text.c_str(), color);

    if (!textSurface) {
        std::cout << "Failed to create text surface: " << SDL_GetError() << std::endl;
    }

    currentFrame.w = textSurface->w;
    currentFrame.h = textSurface->h;
    texture = nullptr;
}

Text::~Text()
{
    if (texture)
        SDL_DestroyTexture(texture);
    else
        SDL_FreeSurface(textSurface);
}

void Text::initTexture(SDL_Renderer *renderer) {
    if (texture) return;
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
}