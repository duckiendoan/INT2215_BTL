#include <string>
#include "Game.h"
#include <iostream>

Game::Game(int p_trials, int bricks_per_row, int num_rows)
    : trials(p_trials), bricksPerRow(bricks_per_row), numRows(num_rows), gameRunning(false), score(0)
{
    srand(time(nullptr));
}

void Game::run() {
    loadButtons();
    generateBricks();
    // Create paddle and ball
    GameObject paddle(0.4 * constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT - 0.03 * constants::SCREEN_WIDTH, paddleTex);
    paddle.setScale(0.2f * constants::SCREEN_WIDTH / paddle.getWidth());

    float ballX = paddle.x + paddle.getWidth() / 2 - paddle.getHeight() / 2;
    float ballY = paddle.y - paddle.getHeight();
    double ballAngle = constants::PI / 3;

    Ball ball(ballX, ballY, ballTex, ballAngle, constants::DEFAULT_BALL_SPEED);
    ball.setScale(paddle.getScale());

    background.setScale(float(constants::SCREEN_WIDTH) / background.getWidth());

    state = GAME_STATE_INITIAL;
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

        render(ball, paddle);
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
    //CEV_gifAnimFree(confettiGif.animation);
}

void Game::update(Ball& ball, GameObject &paddle, double dt) {
    bool mouseMoved = false;
    bool mouseClicked = false;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            gameRunning = false;
        if (event.type == SDL_MOUSEBUTTONUP) {
            //std::cout << "Mouse Button up!" << std::endl;
            if (pauseBtn.state == BUTTON_STATE_HOVER)
            {
                state = GAME_STATE_PAUSED;
                pauseBtn.state = BUTTON_STATE_DEFAULT;
            }
            else if (continueBtn.state == BUTTON_STATE_HOVER)
            {
                state = GAME_STATE_PLAYING;
                continueBtn.state = BUTTON_STATE_DEFAULT;
            }
            else if (resetBtn.state == BUTTON_STATE_HOVER)
            {
                reset(ball, paddle, false);
                resetBtn.state = BUTTON_STATE_DEFAULT;
            }
            else if (homeBtn.state == BUTTON_STATE_HOVER)
            {
                state = GAME_STATE_INITIAL;
                homeBtn.state = BUTTON_STATE_DEFAULT;
            }
            else if (startBtn.state == BUTTON_STATE_HOVER)
            {
                reset(ball, paddle, false);
                startBtn.state = BUTTON_STATE_DEFAULT;
            }
            else if (exitBtn.state == BUTTON_STATE_HOVER)
                gameRunning = false;

        }
        if (event.type == SDL_MOUSEMOTION)
            mouseMoved = true;
    }
    const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
    switch (state) {
        case GAME_STATE_INITIAL:
            startBtn.update();
            settingsBtn.update();
            helpBtn.update();
            exitBtn.update();
            break;
        case GAME_STATE_PLAYING: {
            if (currentKeyStates[SDL_SCANCODE_P]) {
                state = GAME_STATE_PAUSED;
            }
            pauseBtn.update();
            handlePaddleMovement(paddle, currentKeyStates, mouseMoved);
            ball.checkEdgeCollision(paddle, trials);
            ball.checkPaddleCollision(paddle);
            ball.checkBrickCollision(bricks, remainingBricks, score);
            ball.updatePosition(dt);

            if (remainingBricks == 0) state = GAME_STATE_WON;
            if (trials < 0)
            {
                trials++;
                state = GAME_STATE_LOST;
                std::cout << "You lost!" << std::endl;
            }
            break;
        }

        case GAME_STATE_PAUSED:
            //pauseBtn.update();
            continueBtn.update();
            homeBtn.update();
            resetBtn.update();
            break;
        case GAME_STATE_WON:
        case GAME_STATE_LOST:
        {
            if (currentKeyStates[SDL_SCANCODE_RETURN]) {
                reset(ball, paddle, state == GAME_STATE_WON);
            }
            break;
        }
    }
}
void Game::render(Ball &ball, GameObject &paddle) {
    std::string attempt = "Attempts: " + std::to_string(trials);
    std::string scoreTxt = "Score: " + std::to_string(score);

    switch (state) {
        case GAME_STATE_INITIAL:
            window.clearScreen();
            window.render(background);
            window.render(gameTitle);
            window.renderButton(startBtn);
            window.renderButton(settingsBtn);
            window.renderButton(helpBtn);
            window.renderButton(exitBtn);
            window.display();
            break;
        case GAME_STATE_PLAYING:
        case GAME_STATE_WON:
        case GAME_STATE_PAUSED:
        case GAME_STATE_LOST: {
            // Rendering
            window.clearScreen();
            window.render(background);
            //window.render(test);
            window.renderButton(pauseBtn);
            // Text rendering
            window.renderText(attempt.c_str(), Score, 10, 10);
            window.renderText(scoreTxt.c_str(), Score, 10, 30);
            if (state == GAME_STATE_WON)
                window.renderText("You won!", Score,
                                  [](int w, int h) { return constants::SCREEN_WIDTH / 2 - w / 2;},
                                  [](int w, int h) { return constants::SCREEN_HEIGHT / 2 - h / 2; });
            if (state == GAME_STATE_LOST)
                window.renderText("Game over", Score,
                                  [](int w, int h) { return constants::SCREEN_WIDTH / 2 - w / 2;},
                                  [](int w, int h) { return constants::SCREEN_HEIGHT / 2 - h / 2; });

            window.render(paddle);
            window.render(ball);

            for (auto brick: bricks)
                if (brick.shown)
                    window.renderSprite(brick);
            if (state == GAME_STATE_PAUSED)
            {
                window.render(pauseMenu);
                window.renderButton(homeBtn);
                window.renderButton(continueBtn);
                window.renderButton(resetBtn);
            }

            window.display();
            break;
        }
    }
}
void Game::generateBricks() {
    int brickHeight = brickSheet.spriteHeight, brickWidth = brickSheet.spriteWidth;

    bricks.clear();
    bricks.reserve(bricksPerRow * numRows);
    float gapPerBrick = float((bricksPerRow - 1) * constants::BRICK_GAP) / float(bricksPerRow);
    float newBrickWidth = (1 / float(bricksPerRow)) *
            (constants::SCREEN_WIDTH - constants::BRICK_MARGIN * 2) - gapPerBrick;
    float brickScale = newBrickWidth / float(brickWidth);
    float newBrickHeight = float(brickHeight) * brickScale;
    Sprite currentBrick = brickSheet.GetSprite(0);

    for (int i = 0; i < bricksPerRow * numRows; i++)
    {
        int row = i / bricksPerRow;
        int column = i % bricksPerRow;
        float brick_y = constants::BRICK_MARGIN + 50 + constants::BRICK_GAP * (row) + newBrickHeight * row;
        float brick_x = constants::BRICK_MARGIN + constants::BRICK_GAP * (column) + newBrickWidth * column;
        if (column == 0)
            currentBrick = brickSheet.GetSprite(rand() % brickSheet.spriteCount);
        currentBrick.x = brick_x;
        currentBrick.y = brick_y;
        bricks.push_back(currentBrick);
        //bricks.emplace_back(brick_x, brick_y, brickTex);
    }

    for (auto &brick : bricks)
        brick.setScale(brickScale);
    remainingBricks = bricksPerRow * numRows;
}

void Game::handlePaddleMovement(GameObject &paddle, const Uint8* currentKeyStates, bool mouse_moved) {
    float distance = 0;
    if (currentKeyStates[SDL_SCANCODE_LEFT]) {
        distance = -30;
    }
    else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
        distance = 30;
    }
    else if (mouse_moved){
        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        distance = mouse_x - paddle.x - paddle.getWidth() / 2;
    }

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
    state = GAME_STATE_PLAYING;
}

void Game::loadButtons() {
    pauseBtn.setScale(1);
    pauseBtn.x = constants::SCREEN_WIDTH - pauseBtn.getWidth() - 10;
    pauseBtn.y = 7;

    gameTitle.setScale(0.5 * constants::SCREEN_WIDTH / gameTitle.getWidth());
    gameTitle.x = constants::SCREEN_WIDTH * 0.5 - gameTitle.getWidth() * 0.5;
    gameTitle.y = constants::SCREEN_HEIGHT * 0.35 - gameTitle.getHeight() * 0.35;
    loadPauseMenuButtons();
    loadStartMenuButtons();

}

void Game::loadStartMenuButtons()
{
    startBtn.setScale(0.2 * constants::SCREEN_WIDTH / startBtn.srcRect.w);
    startBtn.x = constants::SCREEN_WIDTH * 0.5 - startBtn.getWidth() * 0.5;
    startBtn.y = constants::SCREEN_HEIGHT * 0.5 - startBtn.getHeight() * 0.5;

    settingsBtn.setScale(startBtn.getScale());
    settingsBtn.x = startBtn.x;
    settingsBtn.y = startBtn.y + startBtn.getHeight();

    helpBtn.setScale(startBtn.getScale());
    helpBtn.x = startBtn.x;
    helpBtn.y = settingsBtn.y + settingsBtn.getHeight();

    exitBtn.setScale(startBtn.getScale());
    exitBtn.x = startBtn.x;
    exitBtn.y = helpBtn.y + helpBtn.getHeight();
}

void Game::loadPauseMenuButtons() {
    pauseMenu.setScale(0.4 * constants::SCREEN_WIDTH / pauseMenu.getWidth());
    pauseMenu.x = 0.5f * constants::SCREEN_WIDTH - 0.5f * pauseMenu.getWidth();
    pauseMenu.y = 0.5f * constants::SCREEN_HEIGHT - 0.5f * pauseMenu.getHeight();

    int btnWidth = homeBtn.srcRect.w;
    homeBtn.setScale(0.09 * constants::SCREEN_WIDTH / btnWidth);
    continueBtn.setScale(homeBtn.getScale());
    resetBtn.setScale(homeBtn.getScale());

    float gap = float(pauseMenu.getWidth() - homeBtn.getWidth() * 4) / 2;
    homeBtn.x = pauseMenu.x + 0.5f * homeBtn.getWidth();
    homeBtn.y = pauseMenu.y + pauseMenu.getHeight() * 0.45f;

    continueBtn.x = homeBtn.x + homeBtn.getWidth() + gap;
    continueBtn.y = homeBtn.y;

    resetBtn.x = continueBtn.x + homeBtn.getWidth() + gap;
    resetBtn.y = homeBtn.y;
}