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
    //brickManager->generateBricks();
    paddle.setScale(0.2f * constants::SCREEN_WIDTH / paddle.getWidth());

    float ballX = paddle.x + paddle.getWidth() * 0.5f - paddle.getHeight() * 0.5f;
    float ballY = paddle.y - paddle.getHeight();
    double ballAngle = constants::PI / (rand() % 6 + 2);

    ball.x = ballX;
    ball.y = ballY;
    ball2.x = ballX + ball.getWidth() + 15;
    ball2.y = ballY;
    ball.ballAngle = ballAngle;
    ball2.ballAngle = ballAngle + constants::PI / 2;

    ball.setScale(paddle.getScale());
    ball2.setScale(paddle.getScale());
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
            update(delta / 1000);
            updates++;
            accumulator--;
        }

        render();
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

void Game::update(double dt) {
    bool mouseMoved = false;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            gameRunning = false;
        if (event.type == SDL_MOUSEBUTTONUP) {
            handleMouseButtonUp();
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
                    twoBallCheckBoxBtn.update();
                }
            }
            break;
        case GAME_STATE_PLAYING: {
            if (currentKeyStates[SDL_SCANCODE_P]) {
                state = GAME_STATE_PAUSED;
            }
            animator.updateAnimation();
            pauseBtn.update();

            handlePaddleMovement(currentKeyStates, mouseMoved);
            ball.checkEdgeCollision(paddle, attempts);
            ball.checkPaddleCollision(paddle);
            int newScore = 0;
            if (twoBall)
            {
                ball2.checkEdgeCollision(paddle, attempts);
                ball2.checkPaddleCollision(paddle);
                newScore += brickManager->checkBallBricksCollision(ball2);
            }
            newScore += brickManager->checkBallBricksCollision(ball);

            score += newScore;
            if (newScore > 0)
                soundPlayer.playSfx("collision");
            if (newScore > 0 && rand() % 10 > 4)
            {
                animator.startAnimation(brickManager->lastHitX, brickManager->lastHitY);
            }
            ball.updatePosition(dt);
            if (twoBall)
                ball2.updatePosition(dt);
            // Check game state
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
            animator.updateAnimation();
            if (currentKeyStates[SDL_SCANCODE_RETURN]) {
                reset(state == GAME_STATE_WON);
            }
            break;
        }
    }
}
void Game::render() {
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
                window.renderButton(twoBallCheckBoxBtn);
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
            if (twoBall)
                window.render(ball2);
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

void Game::handlePaddleMovement(const Uint8* currentKeyStates, bool mouse_moved) {
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

void Game::reset(bool nextLevel) {
    if (nextLevel) {
        brickManager->nextLevel();
    }
    brickManager->generateBricks();
    paddle.x = 0.4 * constants::SCREEN_WIDTH;
    paddle.y = constants::SCREEN_HEIGHT - 0.03 * constants::SCREEN_WIDTH;
    ball.resetPosition(paddle);
    if (twoBall)
        ball2.resetPosition(paddle);
    attempts = 3;
    if (twoBall)
        attempts += 3;
    score = 0;
    state = GAME_STATE_PLAYING;
}


void Game::handleMouseButtonUp()
{
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
        reset(false);
        resetBtn.state = BUTTON_STATE_DEFAULT;
    }
    else if (homeBtn.state == BUTTON_STATE_HOVER)
    {
        state = GAME_STATE_INITIAL;
        homeBtn.state = BUTTON_STATE_DEFAULT;
    }
    else if (startBtn.state == BUTTON_STATE_HOVER)
    {
        reset(false);
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
        soundPlayer.sfxPlaying = !soundPlayer.sfxPlaying;
    }
    else if (twoBallCheckBoxBtn.state == BUTTON_STATE_HOVER)
    {
        twoBallCheckBoxBtn.swapState();
        twoBall = !twoBall;
    }
    else if (exitBtn.state == BUTTON_STATE_HOVER)
        gameRunning = false;
    soundPlayer.playSfx("button_click");
}