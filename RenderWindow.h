#pragma once
#include "SDL2/SDL.h"
#include "GameObject.h"

class RenderWindow
{
public:
    RenderWindow(const char* title, int w, int h);
    void cleanUp();
    void clearScreen();
    SDL_Texture* loadTexture(const char* filePath);
    void render(GameObject& entity);
    void display();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};