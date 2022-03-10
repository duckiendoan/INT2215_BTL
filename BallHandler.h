#pragma once
#include <iostream>
#include "Common.h"
#include "GameObject.h"
#include <vector>

class BallHandler
{
public:
    GameObject& ball;
    BallHandler(GameObject& ball, double angle, float ballSpeed);
    void checkEdgeCollision(GameObject& paddle, int& trials);
    void checkPaddleCollision(GameObject& paddle);
    void checkBrickCollision(std::vector<GameObject>& bricks);
    void updatePosition();
    void resetBallPosition(GameObject& paddle);
private:
    double ballAngle;
    float ballSpeed;
};