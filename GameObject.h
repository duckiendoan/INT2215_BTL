#pragma once
#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"

class GameObject
{
public:
    GameObject(float p_x, float p_y, SDL_Texture* p_tex);
    float x, y;
    SDL_Rect currentFrame;
    SDL_Texture* getTexture();
    void setScale(float new_scale);
    float getScale();
    int getWidth();
    int getHeight();
    bool shown;
private:
    SDL_Texture* texture;
    float scale;
};
