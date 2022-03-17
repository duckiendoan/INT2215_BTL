#include "Ball.h"
#include "Common.h"
#include <cmath>

Ball::Ball(float pX, float pY, SDL_Texture *pTex, double angle,
           float speed) : GameObject(pX, pY, pTex), ballAngle(angle), ballSpeed(speed) {

}

void Ball::checkEdgeCollision(GameObject &paddle, int& trials) {
    if (x <= 0) {
        x = 0;
        ballAngle = -ballAngle - constants::PI;
    }
    if (y <= 0) {
        y = 0;
        ballAngle = -ballAngle;
    }
    if (x >= constants::SCREEN_WIDTH - getWidth()) {
        x = constants::SCREEN_WIDTH - getWidth();
        ballAngle = -ballAngle - constants::PI;
    }
    if (y >= constants::SCREEN_HEIGHT - getHeight()) {
        trials--;
        if (trials >= 0)
            resetPosition(paddle);
        //ball.y = SCREEN_HEIGHT - ball.getHeight();
        //ballAngle = -ballAngle;
    }
}

void Ball::checkPaddleCollision(GameObject &paddle) {
    if (GameObject::checkCollision(*this, paddle)) {
        float ball_center_x = x + getWidth() / 2.0f;
        float hit_x = ball_center_x - paddle.x;
        float k = 0;

        if (hit_x < paddle.getWidth() / 2) {
            k = 2 * hit_x / paddle.getWidth();
            ballAngle = constants::PI - k * constants::PI / 2 - 0.3;
        } else {
            k = 2 * (paddle.getWidth() - hit_x) / paddle.getWidth();
            ballAngle = k * constants::PI / 2 + 0.3;
        }

    }
}

void Ball::updatePosition(double dt) {
    x += (ballSpeed * dt) * cos(ballAngle);
    y -= (ballSpeed * dt) * sin(ballAngle);
}

void Ball::checkBrickCollision(std::vector<GameObject> &bricks, int& remainingBricks, int& score) {
    for (int i = 0; i < bricks.size(); i++) {
        GameObject brick = bricks[i];
        if (brick.shown) {
            if (GameObject::checkCollision(*this, brick)) {
                bricks[i].shown = false;
                remainingBricks--;
                score += 30;

                float ballcenterx = x + 0.5f * getWidth();
                float ballcentery = y + 0.5f * getHeight();

                // Center of the brick x and y coordinates
                float brickcenterx = brick.x + 0.5f * brick.getWidth();
                float brickcentery = brick.y + 0.5f * brick.getHeight();

                float x_min = std::max(x, brick.x);
                float x_max = std::min(x + getWidth(), brick.x + brick.getWidth());

                float y_min = std::max(y, brick.y);
                float y_max = std::min(y + getHeight(), brick.y + brick.getHeight());

                float x_size = x_max - x_min;
                float y_size = y_max - y_min;

                if (y_size > x_size) {
                    // Left
                    if (ballcenterx < brickcenterx) {
                        x -= x_size + 0.01f; // Move out of collision
                    } else {
                        x += x_size + 0.01f; // Move out of collision
                    }
                    ballAngle = -ballAngle - constants::PI;
                } else {
                    if (ballcentery > brickcentery) {
                        // Bottom
                        y += y_size + 0.01f; // Move out of collision
                    } else {
                        // Top
                        y -= y_size + 0.01f; // Move out of collision
                    }
                    ballAngle = -ballAngle;
                }

            }

        }
    }
}

void Ball::resetPosition(GameObject &paddle) {
    x = paddle.x + paddle.getWidth() / 2 - paddle.getHeight() / 2;
    y = paddle.y - paddle.getHeight();
}