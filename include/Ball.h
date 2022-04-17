#pragma once
#include "Sprite.h"
#include "vector"

class Ball: public GameObject
{
public:
    Ball(float pX, float pY, SDL_Texture *pTex, double angle, float ballSpeed);
    void checkEdgeCollision(GameObject& paddle, int& trials);
    void checkPaddleCollision(GameObject& paddle);
    void checkBallCollision(Ball& other);
    void updatePosition(double dt);
    void resetPosition(GameObject& paddle);
    double ballAngle;
private:
    double ballSpeed;
};