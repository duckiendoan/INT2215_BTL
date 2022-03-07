#include <string>
#include "Game.h"
#include "BallHandler.h"

Game::Game(int p_trials, int bricks_per_row, int num_rows)
    : trials(p_trials), bricksPerRow(bricks_per_row), numRows(num_rows)
{

}

void Game::run() {
    loadBackground();
    generateBricks();
    // Create paddle and ball
    GameObject paddle(0.4 * constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT - 0.03 * constants::SCREEN_WIDTH, paddleTex);
    paddle.setScale(0.2f * constants::SCREEN_WIDTH / paddle.getWidth());

    GameObject ball(paddle.x + paddle.getWidth() / 2 - paddle.getHeight() / 2, paddle.y - paddle.getHeight(), ballTex);
    ball.setScale(paddle.getScale());

    GameObject background(0, 0, backgroundTex[0]);
    background.setScale(float(constants::SCREEN_WIDTH) / background.getWidth());

    double ballAngle = constants::PI / 3;
    BallHandler ballHandler(ball, ballAngle, constants::DEFAULT_BALL_SPEED);

    bool gameRunning = true;
    SDL_Event event;

    state = Playing;
    while (gameRunning)
    {
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
                ballHandler.checkEdgeCollision(paddle, trials);
                ballHandler.checkPaddleCollision(paddle);
                ballHandler.checkBrickCollision(bricks);
                ballHandler.updatePosition();

                if (trials < 0)
                {
                    trials++;
                    state = Lost;
                    std::cout << "You lost!" << std::endl;
                }

                // Rendering
                std::string attemptText = "Attempts: " + std::to_string(trials);
                window.clearScreen();
                window.render(background);
                window.renderText(attemptText.c_str(), Score, 10, 10);
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
            case Won:
                break;
            case Lost:
                std::string attemptText = "Attempts: " + std::to_string(trials);
                window.clearScreen();
                window.render(background);
                window.renderText(attemptText.c_str(), Score, 10, 10);
                window.renderText("You lost!", Score, constants::SCREEN_WIDTH / 2, constants::SCREEN_HEIGHT / 2);
                window.render(paddle);
                window.render(ball);

                for (auto brick: bricks)
                    if (brick.shown)
                        window.render(brick);

                window.display();

                break;
        }

        SDL_Delay(5);
    }

    window.cleanUp();
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
        int row = i / 8;
        int column = i % 8;
        float brick_y = constants::BRICK_MARGIN + 15 + constants::BRICK_GAP * (row) + newBrickHeight * row;
        float brick_x = constants::BRICK_MARGIN + constants::BRICK_GAP * (column) + newBrickWidth * column;

        bricks.emplace_back(brick_x, brick_y, brickTex);
    }

    for (auto &brick : bricks)
        brick.setScale(brickScale);
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

void Game::loadBackground() {
    backgroundTex.push_back(window.loadTexture("assets\\background1.png"));
    backgroundTex.push_back(window.loadTexture("assets\\background2.png"));
}