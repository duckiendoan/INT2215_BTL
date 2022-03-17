#include <string>
#include "Game.h"
#include <iostream>

Game::Game(int p_trials, int bricks_per_row, int num_rows)
    : trials(p_trials), bricksPerRow(bricks_per_row), numRows(num_rows), gameRunning(false), score(0)
{

}

void Game::run() {
    loadBackground();
    generateBricks();
    // Create paddle and ball
    GameObject paddle(0.4 * constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT - 0.03 * constants::SCREEN_WIDTH, paddleTex);
    paddle.setScale(0.2f * constants::SCREEN_WIDTH / paddle.getWidth());

    float ballX = paddle.x + paddle.getWidth() / 2 - paddle.getHeight() / 2;
    float ballY = paddle.y - paddle.getHeight();
    double ballAngle = constants::PI / 3;

    Ball ball(ballX, ballY, ballTex, ballAngle, constants::DEFAULT_BALL_SPEED);
    ball.setScale(paddle.getScale());

    GameObject background(0, 0, backgroundTex[0]);
    background.setScale(float(constants::SCREEN_WIDTH) / background.getWidth());

    SDL_Event event;

    state = Playing;
    gameRunning = true;
    auto lastTime = SDL_GetTicks64();
    // Update the game 60 times per second
    const double delta = 1000.0 / 60;
    double accumulator = 0;
    int updates = 0, fps = 0;
    long long timer = 0;

    while (gameRunning)
    {
        auto currentTime = SDL_GetTicks64();
        auto frameTime = currentTime - lastTime;
        lastTime = currentTime;
        accumulator += double(frameTime) / delta;

        while (accumulator >= 1) {
            update(ball, paddle, delta / 1000);
            updates++;
            accumulator--;
        }

        render(ball, paddle, background);
        fps++;
        if (SDL_GetTicks64() - timer > 1000) {
            timer += 1000;
            std::cout << updates << " updates, " << fps << " fps" << "\n";
            updates = 0;
            fps = 0;
        }
        //SDL_Delay(5);
    }

    window.cleanUp();
    SDL_DestroyTexture(ballTex);
    SDL_DestroyTexture(brickTex);
    SDL_DestroyTexture(paddleTex);
}

void Game::update(Ball& ball, GameObject &paddle, double dt) {

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            gameRunning = false;
    }

    switch (state) {
        case Initial:
            break;
        case Playing: {
            handlePaddleMovement(paddle);
            ball.checkEdgeCollision(paddle, trials);
            ball.checkPaddleCollision(paddle);
            ball.checkBrickCollision(bricks, remainingBricks, score);
            ball.updatePosition(dt);

            if (remainingBricks == 0) state = Won;
            if (trials < 0)
            {
                trials++;
                state = Lost;
                std::cout << "You lost!" << std::endl;
            }
            break;
        }

        case Paused:
            break;
        case Won: {
            const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
            if (currentKeyStates[SDL_SCANCODE_RETURN]) {
                reset(ball, paddle, true);
            }
            break;
        }

        case Lost:
            const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
            if (currentKeyStates[SDL_SCANCODE_RETURN]) {
                reset(ball, paddle, false);
            }
            break;
    }
}
void Game::render(Ball &ball, GameObject &paddle, GameObject &background) {
    std::string attemptText = "Attempts: " + std::to_string(trials);
    std::string scoreText = "Score: " + std::to_string(score);
    switch (state) {
        case Initial:
            break;
        case Playing:
        case Won:
        case Lost: {
            // Rendering
            window.clearScreen();
            window.render(background);
            window.renderText(attemptText.c_str(), Score, 10, 10);
            window.renderText(scoreText.c_str(), Score, 10, 30);
            if (state == Won)
                window.renderText("You won!", Score, constants::SCREEN_WIDTH / 2, constants::SCREEN_HEIGHT / 2);
            if (state == Lost)
                window.renderText("Game over!", Score, constants::SCREEN_WIDTH / 2, constants::SCREEN_HEIGHT / 2);

            window.render(paddle);
            window.render(ball);

            for (auto brick: bricks)
                if (brick.shown)
                    window.render(brick);

            window.display();
            break;
        }

        case Paused:
            break;
    }
}
void Game::generateBricks() {
    int brickHeight = 0, brickWidth = 0;
    SDL_QueryTexture(brickTex, nullptr, nullptr, &brickWidth, &brickHeight);

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
        float brick_y = constants::BRICK_MARGIN + 30 + constants::BRICK_GAP * (row) + newBrickHeight * row;
        float brick_x = constants::BRICK_MARGIN + constants::BRICK_GAP * (column) + newBrickWidth * column;

        bricks.emplace_back(brick_x, brick_y, brickTex);
    }

    for (auto &brick : bricks)
        brick.setScale(brickScale);
    remainingBricks = bricksPerRow * numRows;
}

void Game::handlePaddleMovement(GameObject &paddle) {
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    float distance = mouse_x - paddle.x - paddle.getWidth() / 2;
    float paddle_vel = distance / 3;

    // Distance is too small or the paddle is going out of screen
    if (abs(distance) < 5 || (paddle.x + paddle.getWidth() >= constants::SCREEN_WIDTH && paddle_vel > 0)
        || (paddle.x <= 0 && paddle_vel < 0))
        paddle_vel = 0;
    paddle.x += paddle_vel;
}

void Game::reset(Ball& ball, GameObject& paddle, bool nextLevel) {
    if (nextLevel) {
        numRows++;
        bricksPerRow++;
    }
    generateBricks();
    paddle.x = 0.4 * constants::SCREEN_WIDTH;
    paddle.y = constants::SCREEN_HEIGHT - 0.03 * constants::SCREEN_WIDTH;
    ball.resetPosition(paddle);
    trials = 3;
    score = 0;
    state = Playing;
}
void Game::loadBackground() {
    backgroundTex.push_back(window.loadTexture("assets\\background1.png"));
    backgroundTex.push_back(window.loadTexture("assets\\background2.png"));
}