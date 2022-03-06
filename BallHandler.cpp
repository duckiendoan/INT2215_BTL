#include <iostream>
#include <cmath>
#include <vector>
#include "GameObject.h"
#include "BallHandler.h"

BallHandler::BallHandler(GameObject &p_ball, double p_angle, float p_ballSpeed)
        : ball(p_ball), ballAngle(p_angle), ballSpeed(p_ballSpeed) {

}

void BallHandler::checkEdgeCollision(GameObject &paddle, int& trials) {
    if (ball.x <= 0) {
        ball.x = 0;
        ballAngle = -ballAngle - constants::PI;
    }
    if (ball.y <= 0) {
        ball.y = 0;
        ballAngle = -ballAngle;
    }
    if (ball.x >= constants::SCREEN_WIDTH - ball.getWidth()) {
        ball.x = constants::SCREEN_WIDTH - ball.getWidth();
        ballAngle = -ballAngle - constants::PI;
    }
    if (ball.y >= constants::SCREEN_HEIGHT - ball.getHeight()) {
        trials--;
        if (trials >= 0)
            resetBallPosition(paddle);
        //ball.y = SCREEN_HEIGHT - ball.getHeight();
        //ballAngle = -ballAngle;
    }
}

void BallHandler::checkPaddleCollision(GameObject &paddle) {
    if (ball.x + ball.getWidth() > paddle.x && ball.x < paddle.x + paddle.getWidth() &&
        ball.y + ball.getHeight() > paddle.y && ball.y < paddle.y + paddle.getHeight()) {
        float ball_center_x = ball.x + ball.getWidth() / 2.0f;
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

void BallHandler::updatePosition() {
    ball.x += (ballSpeed / 60) * cos(ballAngle);
    ball.y -= (ballSpeed / 60) * sin(ballAngle);
}

void BallHandler::checkBrickCollision(std::vector<GameObject> &bricks) {
    for (int i = 0; i < bricks.size(); i++) {
        GameObject brick = bricks[i];
        if (brick.shown) {
            if (ball.x + ball.getWidth() > brick.x && ball.x < brick.x + brick.getWidth() &&
                ball.y + ball.getHeight() > brick.y && ball.y < brick.y + brick.getHeight()) {
                bricks[i].shown = false;

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
                    ballAngle = -ballAngle - constants::PI;
                } else {
                    if (ballcentery > brickcentery) {
                        // Bottom
                        ball.y += y_size + 0.01f; // Move out of collision
                    } else {
                        // Top
                        ball.y -= y_size + 0.01f; // Move out of collision
                    }
                    ballAngle = -ballAngle;
                }

            }

        }
    }
}

void BallHandler::resetBallPosition(GameObject &paddle) {
    ball.x = paddle.x + paddle.getWidth() / 2 - paddle.getHeight() / 2;
    ball.y = paddle.y - paddle.getHeight();
}