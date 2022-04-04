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
        float hit_x = (paddle.x + paddle.getWidth() / 2.0f) - ball_center_x;
        double maxBounceAngle = 5 * constants::PI / 12;
        // https://gamedev.stackexchange.com/questions/4253/in-pong-how-do-you-calculate-the-balls-direction-when-it-bounces-off-the-paddl
        float normalizedHitX = hit_x / (paddle.getWidth() / 2.0f);
        ballAngle = normalizedHitX * maxBounceAngle + constants::PI / 2;
    }
}

void Ball::updatePosition(double dt) {
    x += (ballSpeed * dt) * cos(ballAngle);
    y -= (ballSpeed * dt) * sin(ballAngle);
}

void Ball::resetPosition(GameObject &paddle) {
    x = paddle.x + paddle.getWidth() / 2 - paddle.getHeight() / 2;
    y = paddle.y - paddle.getHeight();
}