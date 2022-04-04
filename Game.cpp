#include <string>
#include "Game.h"
#include <iostream>

Game::Game(int p_trials, int bricks_per_row, int num_rows)
    : attempts(p_trials), gameRunning(false), score(0)
{
    srand(time(nullptr));
    brickManager = new BrickManager(bricks_per_row, num_rows, brickSprite);
}

void Game::run() {
    loadButtons();
    brickManager->generateBricks();
    // Create paddle and ball
    GameObject paddle(0.4 * constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT - 0.03 * constants::SCREEN_WIDTH, paddleTex);
    paddle.setScale(0.2f * constants::SCREEN_WIDTH / paddle.getWidth());

    float ballX = paddle.x + paddle.getWidth() * 0.5f - paddle.getHeight() * 0.5f;
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
    soundPlayer.playMusic();

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
    }

    window.cleanUp();
    SDL_DestroyTexture(ballTex);
    SDL_DestroyTexture(brickTex);
    SDL_DestroyTexture(paddleTex);
    SDL_DestroyTexture(pauseBtnTex);
    SDL_DestroyTexture(pauseMenuBtnTex);
    SDL_DestroyTexture(pauseMenuTex);
    SDL_DestroyTexture(startMenuBtnTex);
    SDL_DestroyTexture(gameTitleTex);
    SDL_DestroyTexture(backgroundTex);
    delete brickManager;
}

void Game::update(Ball& ball, GameObject &paddle, double dt) {
    bool mouseMoved = false;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            gameRunning = false;
        if (event.type == SDL_MOUSEBUTTONUP) {
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
            else if (settingsBtn.state == BUTTON_STATE_HOVER)
            {
                settingsMenu = true;
                settingsBtn.state = BUTTON_STATE_DEFAULT;
            }
            else if (helpBtn.state == BUTTON_STATE_HOVER)
            {
                helpMenu = true;
                helpBtn.state = BUTTON_STATE_DEFAULT;
            }
            else if (okButton.state == BUTTON_STATE_HOVER)
            {
                if (settingsMenu) settingsMenu = false;
                if (helpMenu) helpMenu = false;
                okButton.state = BUTTON_STATE_DEFAULT;
            }
            else if (musicCheckboxBtn.state == BUTTON_STATE_HOVER)
            {
                musicCheckboxBtn.swapState();
                if (soundPlayer.musicPlaying)
                {
                    soundPlayer.pauseMusic();
                    soundPlayer.musicPlaying = false;
                } else {
                    soundPlayer.resumeMusic();
                    soundPlayer.musicPlaying = true;
                }
            }
            else if (sfxCheckBoxBtn.state == BUTTON_STATE_HOVER)
            {
                sfxCheckBoxBtn.swapState();
                if (soundPlayer.sfxPlaying)
                {
                    soundPlayer.sfxPlaying = false;
                } else {
                    soundPlayer.sfxPlaying = true;
                }
            }
            else if (exitBtn.state == BUTTON_STATE_HOVER)
                gameRunning = false;
            soundPlayer.playSfx("button_click");
        }
        if (event.type == SDL_MOUSEMOTION)
            mouseMoved = true;
    }
    const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
    switch (state) {
        case GAME_STATE_INITIAL:
            if (!settingsMenu && !helpMenu)
            {
                startBtn.update();
                settingsBtn.update();
                helpBtn.update();
                exitBtn.update();
            }
            else
            {
                okButton.update();
                if (settingsMenu)
                {
                    musicCheckboxBtn.update();
                    sfxCheckBoxBtn.update();
                }
            }
            break;
        case GAME_STATE_PLAYING: {
            if (currentKeyStates[SDL_SCANCODE_P]) {
                state = GAME_STATE_PAUSED;
            }
            animator.updateAnimation();
            pauseBtn.update();
            handlePaddleMovement(paddle, currentKeyStates, mouseMoved);
            ball.checkEdgeCollision(paddle, attempts);
            ball.checkPaddleCollision(paddle);
            int newScore = brickManager->checkBallBricksCollision(ball);
            score += newScore;

            if (newScore > 0 && rand() % 10 > 4)
            {
                animator.startAnimation(brickManager->lastHitX, brickManager->lastHitY);
            }
            ball.updatePosition(dt);

            if (brickManager->getRemainingBricks() == 0) state = GAME_STATE_WON;
            if (attempts < 0)
            {
                attempts++;
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
    std::string attempt = "Attempts: " + std::to_string(attempts);
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
            if (settingsMenu)
            {
                menuSprite.getFrame(1);
                window.renderSprite(menuSprite);
                window.renderButton(musicCheckboxBtn);
                window.renderButton(sfxCheckBoxBtn);
                window.renderButton(okButton);
            } else if (helpMenu)
            {
                menuSprite.getFrame(0);
                window.renderSprite(menuSprite);
                window.renderButton(okButton);
            }
            window.display();
            break;
        case GAME_STATE_PLAYING:
        case GAME_STATE_WON:
        case GAME_STATE_PAUSED:
        case GAME_STATE_LOST: {
            // Rendering
            window.clearScreen();
            window.render(background);
            animator.renderAnimation();
            window.renderButton(pauseBtn);
            // Text rendering
            window.renderText(attempt.c_str(), Score, 10, 10);
            window.renderText(scoreTxt.c_str(), Score, 10, 30);
            if (state == GAME_STATE_WON)
                window.renderText("You won! Press [Enter] to enter the next level!", Score,
                                  [](int w, int h) { return constants::SCREEN_WIDTH / 2 - w / 2;},
                                  [](int w, int h) { return constants::SCREEN_HEIGHT / 2 - h / 2; });
            if (state == GAME_STATE_LOST)
                window.renderText("Game over! Press [Enter] to try again!", Score,
                                  [](int w, int h) { return constants::SCREEN_WIDTH / 2 - w / 2;},
                                  [](int w, int h) { return constants::SCREEN_HEIGHT / 2 - h / 2; });

            window.render(paddle);
            window.render(ball);
            brickManager->renderBricks(window);

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
        distance = mouse_x - paddle.x - paddle.getWidth() * 0.5f;
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
        brickManager->nextLevel();
    }
    brickManager->generateBricks();
    paddle.x = 0.4 * constants::SCREEN_WIDTH;
    paddle.y = constants::SCREEN_HEIGHT - 0.03 * constants::SCREEN_WIDTH;
    ball.resetPosition(paddle);
    attempts = 3;
    score = 0;
    state = GAME_STATE_PLAYING;
}

void Game::loadButtons()
{
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

    menuSprite.setScale(startBtn.getScale());
    menuSprite.x = constants::SCREEN_WIDTH * 0.5 - menuSprite.getWidth() * 0.5;
    menuSprite.y = constants::SCREEN_HEIGHT * 0.5 - menuSprite.getHeight() * 0.5;

    okButton.setScale(startBtn.getScale());
    okButton.x = menuSprite.x + menuSprite.getWidth() * 0.5f - 0.5f * okButton.getWidth();
    okButton.y = menuSprite.y + menuSprite.getHeight() * 0.82f;

    musicCheckboxBtn.setScale(startBtn.getScale() + 0.3);
    sfxCheckBoxBtn.setScale(startBtn.getScale() + 0.3);
    musicCheckboxBtn.x = menuSprite.x + menuSprite.getWidth() * 0.75f;
    sfxCheckBoxBtn.x = musicCheckboxBtn.x;

    musicCheckboxBtn.y = menuSprite.y + menuSprite.getHeight() * 0.5f - musicCheckboxBtn.getHeight() * 0.7f;
    sfxCheckBoxBtn.y = musicCheckboxBtn.y + 1.1f * musicCheckboxBtn.getHeight();
}

void Game::loadPauseMenuButtons()
{
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