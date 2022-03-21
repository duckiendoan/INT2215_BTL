#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "RenderWindow.h"

RenderWindow::RenderWindow(const char *title, int w, int h) : window(nullptr), renderer(nullptr)
{
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              w, h, SDL_WINDOW_SHOWN);
    if (window == nullptr)
        std::cout << "Failed to initialize window: " << SDL_GetError() << std::endl;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr)
        std::cout << "Failed to initialize renderer: " << SDL_GetError() << std::endl;

    // Load font
    scoreFont = TTF_OpenFont("assets/fonts/VCR_OSD_MONO_1.001.ttf", 24);
}

void RenderWindow::cleanUp()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(scoreFont);
}

SDL_Texture *RenderWindow::loadTexture(const char *filePath)
{
    SDL_Texture* tex = nullptr;
    tex = IMG_LoadTexture(renderer, filePath);
    if (tex == nullptr)
        std::cout << "Failed to load texture: " << SDL_GetError() << std::endl;
    return tex;
}

void RenderWindow::clearScreen()
{
    SDL_RenderClear(renderer);
}

void RenderWindow::render(GameObject& entity)
{
    entity.currentFrame.x = (int)entity.x;
    entity.currentFrame.y = (int)entity.y;

    SDL_RenderCopy(renderer, entity.getTexture(), nullptr, &entity.currentFrame);
}

void RenderWindow::renderButton(Button &entity)
{
    entity.currentFrame.x = (int)entity.x;
    entity.currentFrame.y = (int)entity.y;

    SDL_RenderCopy(renderer, entity.getTexture(), &entity.srcRect, &entity.currentFrame);
}

void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}

void RenderWindow::renderText(const char *text, FontType type, int x, int y)
{
    SDL_Surface* textSurface;
    // Set color to black
    switch (type) {
        case Score:
            textSurface = TTF_RenderText_Solid(scoreFont, text, {255, 255, 255});
            break;
        default:
            break;
    }

    if (!textSurface) {
        std::cout << "Failed to render text: " << SDL_GetError() << std::endl;
    }

    SDL_Texture* text_texture= SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect dst = { x, y, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, text_texture, nullptr, &dst);

    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(textSurface);
}

void RenderWindow::renderText(const char *text, FontType type, int(*x)(int w, int h), int(*y)(int w, int h))
{
    SDL_Surface* textSurface;
    // Set color to black
    switch (type) {
        case Score:
            textSurface = TTF_RenderText_Solid(scoreFont, text, {255, 255, 255});
            break;
        default:
            break;
    }

    if (!textSurface) {
        std::cout << "Failed to render text: " << SDL_GetError() << std::endl;
    }

    SDL_Texture* text_texture= SDL_CreateTextureFromSurface(renderer, textSurface);
    int w = textSurface->w;
    int h = textSurface->h;
    SDL_Rect dst = { x(w, h), y(w, h), w, h };
    SDL_RenderCopy(renderer, text_texture, nullptr, &dst);

    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(textSurface);
}

void RenderWindow::renderText(Text &textEntity)
{
    textEntity.initTexture(renderer);
    render(textEntity);
}