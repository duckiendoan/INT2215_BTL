#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "RenderWindow.h"
#include "GameObject.h"

RenderWindow::RenderWindow(const char *title, int w, int h) : window(nullptr), renderer(nullptr)
{
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              w, h, SDL_WINDOW_SHOWN);
    if (window == nullptr)
        std::cout << "Failed to initialize window: " << SDL_GetError() << std::endl;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == nullptr)
        std::cout << "Failed to initialize renderer: " << SDL_GetError() << std::endl;

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
    /*
    SDL_Rect src;
    //SDL_QueryTexture(texture, nullptr, nullptr, &src.w, &src.h);
    src.x = entity.getCurrentFrame().x;
    src.y = entity.getCurrentFrame().y;
    src.w = entity.getCurrentFrame().w;
    src.h = entity.getCurrentFrame().h;
    */
    /*
    SDL_Rect dst;
    dst.x = entity.getX();
    dst.y = entity.getY();
    dst.w = entity.getCurrentFrame().w * scale;
    dst.h = entity.getCurrentFrame().h * scale;
    */
    entity.currentFrame.x = (int)entity.x;
    entity.currentFrame.y = (int)entity.y;

    SDL_RenderCopy(renderer, entity.getTexture(), nullptr, &entity.currentFrame);
}

void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}

void RenderWindow::renderText(const char *text, FontType type, int x, int y)
{
    SDL_Surface* textSurface;
    // Set color to black
    SDL_Color color = { 255, 255, 255 };

    switch (type) {
        case Score:
            textSurface = TTF_RenderText_Solid(scoreFont, text, color);
            break;
        default:
            break;
    }

    if (!textSurface) {
        std::cout << "Failed to render text: " << SDL_GetError() << std::endl;
    }

    SDL_Texture* text_texture= SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect dest = { x, y, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, text_texture, nullptr, &dest);

    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(textSurface);
}