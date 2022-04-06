#include "BrickManager.h"

BrickManager::BrickManager(int bricksPerRow, int numRows, Sprite &brickSprite) :
    bricksPerRow(bricksPerRow), numRows(numRows), brickSprite(brickSprite) {
    lastHitX = 0;
    lastHitY = 0;
    lastHitTime = 0;
    remainingBricks = bricksPerRow * numRows;
}

void BrickManager::generateBricks() {
    int brickHeight = brickSprite.srcRect.h, brickWidth = brickSprite.srcRect.w;

    bricks.clear();
    bricks.reserve(bricksPerRow * numRows);
    float gapPerBrick = float((bricksPerRow - 1) * constants::BRICK_GAP) / float(bricksPerRow);
    float newBrickWidth = (1 / float(bricksPerRow)) *
                          (constants::SCREEN_WIDTH - constants::BRICK_MARGIN * 2) - gapPerBrick;
    float brickScale = newBrickWidth / float(brickWidth);
    float newBrickHeight = float(brickHeight) * brickScale;

    for (int i = 0; i < bricksPerRow * numRows; i++)
    {
        int row = i / bricksPerRow;
        int column = i % bricksPerRow;
        float brick_y = constants::BRICK_MARGIN + 50 + constants::BRICK_GAP * (row) + newBrickHeight * row;
        float brick_x = constants::BRICK_MARGIN + constants::BRICK_GAP * (column) + newBrickWidth * column;
        if (column == 0)
            brickSprite.getFrame(rand() % brickSprite.getFrameCount());
        brickSprite.x = brick_x;
        brickSprite.y = brick_y;
        bricks.push_back(brickSprite);
    }

    for (auto &brick : bricks)
        brick.setScale(brickScale);
    remainingBricks = bricksPerRow * numRows;
}

void BrickManager::renderBricks(RenderWindow &window) {
    for (auto brick: bricks)
        if (brick.shown)
            window.renderSprite(brick);
}

void BrickManager::nextLevel() {
    bricksPerRow++;
    numRows++;
}

int BrickManager::checkBallBricksCollision(Ball &ball) {
    int score = 0;
    for (auto& brick: bricks) {
        if (brick.shown) {
            if (GameObject::checkCollision(ball, brick)) {
                brick.shown = false;
                remainingBricks--;
                score += 30;
                lastHitX = brick.x;
                lastHitY = brick.y;

                float ballcenterx = ball.x + 0.5f * ball.getWidth();
                float ballcentery = ball.y + 0.5f * ball.getHeight();

                // Center of the brick x and y coordinates
                float brickcenterx = brick.x + 0.5f * brick.getWidth();
                float brickcentery = brick.y + 0.5f * brick.getHeight();

                float x_min = std::max(ball.x, brick.x);
                float x_max = std::min(ball.x + ball.getWidth(), brick.x + brick.getWidth());

                float y_min = std::max(ball.y, brick.y);
                float y_max = std::min(ball.y + ball.getHeight(), brick.y + brick.getHeight());

                float x_size = x_max - x_min;
                float y_size = y_max - y_min;

                if (y_size > x_size) {
                    // Left
                    if (ballcenterx < brickcenterx) {
                        ball.x -= x_size + 0.01f; // Move out of collision
                    } else {
                        ball.x += x_size + 0.01f; // Move out of collision
                    }
                    ball.ballAngle = -ball.ballAngle - constants::PI;
                } else {
                    if (ballcentery > brickcentery) {
                        // Bottom
                        ball.y += y_size + 0.01f; // Move out of collision
                    } else {
                        // Top
                        ball.y -= y_size + 0.01f; // Move out of collision
                    }
                    ball.ballAngle = -ball.ballAngle;
                }

            }

        }
    }
    return score;
}

int BrickManager::getRemainingBricks() const {
    return remainingBricks;
}
