#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "GameObject.h"
#include "Common.h"
#include "Text.h"
#include "Button.h"

class RenderWindow
{
public:
    RenderWindow(const char* title, int w, int h);
    void cleanUp();
    void clearScreen();
    SDL_Texture* loadTexture(const char* filePath);
    void render(GameObject& entity);
    void renderButton(Button &entity);
    void renderText(const char* text, FontType type, int x, int y);
    void renderText(const char *text, FontType type, int(*x)(int w, int h), int(*y)(int w, int h));
    void renderText(Text& textEntity);
    void display();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* scoreFont;
};