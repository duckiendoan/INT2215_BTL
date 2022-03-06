#include <iostream>
#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include "GameObject.h"

GameObject::GameObject(float p_x, float p_y, SDL_Texture *p_tex) : x(p_x), y(p_y), texture(p_tex)
{
    currentFrame.x = p_x;
    currentFrame.y = p_y;
    SDL_QueryTexture(p_tex, nullptr, nullptr, &currentFrame.w, &currentFrame.h);
    shown = true;
}

SDL_Texture *GameObject::getTexture() {
    return texture;
}

void GameObject::setScale(float new_scale) {
    scale = new_scale;
    currentFrame.w *= scale;
    currentFrame.h *= scale;
}

int GameObject::getWidth() {
    return currentFrame.w;
}

int GameObject::getHeight() {
    return currentFrame.h;
}

float GameObject::getScale() {
    return scale;
}