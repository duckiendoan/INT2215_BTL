#include "Animator.h"

Animator::Animator(RenderWindow &pWindow) : window(pWindow) {
    fireworkTexture = window.loadTexture("assets\\Firework.png");
    fireworkSprite = new Sprite(5, 6, fireworkTexture);
}

void Animator::updateAnimation() {
    if (fireworkSprite->isPlaying)
        if (!fireworkSprite->getNextFrame())
            fireworkSprite->isPlaying = false;
}

void Animator::startAnimation(float &x, float &y) {
    fireworkSprite->isPlaying = true;
    fireworkSprite->x = x;
    fireworkSprite->y = y;
}

void Animator::renderAnimation() {
    if (fireworkSprite->isPlaying)
        window.renderSprite(*fireworkSprite);
}
