#pragma once
#include "GameObject.h"
#include "vector"

class Ball: public GameObject
{
public:
    Ball(float pX, float pY, SDL_Texture *pTex, double angle, float ballSpeed);
    void checkEdgeCollision(GameObject& paddle, int& trials);
    void checkPaddleCollision(GameObject& paddle);
    void checkBrickCollision(std::vector<GameObject>& bricks);
    void updatePosition(double dt);
    void resetPosition(GameObject& paddle);
private:
    double ballAngle;
    double ballSpeed;
};