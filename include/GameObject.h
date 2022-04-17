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
    static bool checkCollision(GameObject& obj1, GameObject& obj2);
private:
    float scale = 1;
protected:
    SDL_Texture* texture;
    GameObject();
};
