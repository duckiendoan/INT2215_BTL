#pragma once
#include "GameObject.h"
#include "RenderWindow.h"
#include "Common.h"
#include "Ball.h"
#include <vector>

class Game
{
public:
    Game(int p_trials, int bricks_per_row, int num_rows);
    void run();
private:
    RenderWindow window = RenderWindow("Test Game", constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT);
    SDL_Texture* paddleTex = window.loadTexture("assets\\paddle.png");
    SDL_Texture* ballTex = window.loadTexture("assets\\ball.png");
    SDL_Texture* brickTex = window.loadTexture("assets\\brick.png");
    SDL_Texture* pauseBtnTex = window.loadTexture("assets\\pause.png");
    std::vector<SDL_Texture*> backgroundTex;
    std::vector<GameObject> bricks;
    Button pauseBtn = Button({0, 0, 865, 865}, pauseBtnTex);

    SDL_Event event;
    int trials;
    int bricksPerRow;
    int numRows;
    int remainingBricks;
    int score;
    bool gameRunning;
    GameState state = Initial;
    void generateBricks();
    void loadBackground();
    void update(Ball& ball, GameObject& paddle, double dt);
    void render(Ball& ball, GameObject& paddle, GameObject& background);
    void handlePaddleMovement(GameObject& paddle);
    void reset(Ball& ball, GameObject& paddle, bool next_level);
    void loadButtons();
};
