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
}

void RenderWindow::cleanUp()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
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