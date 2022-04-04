#pragma once
#include <vector>
#include "Sprite.h"
#include "RenderWindow.h"
#include "Ball.h"
class BrickManager
{
public:
    BrickManager(int bricksPerRow, int numRows, Sprite& brickSprite);
    void generateBricks();
    void nextLevel();
    void renderBricks(RenderWindow& window);
    int checkBallBricksCollision(Ball& ball);
    int getRemainingBricks() const;
    float lastHitX, lastHitY;
private:
    std::vector<Sprite> bricks;
    Sprite& brickSprite;
    int remainingBricks;
    int bricksPerRow;
    int numRows;
};