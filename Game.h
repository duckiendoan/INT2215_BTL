#pragma once
#include "GameObject.h"
#include "RenderWindow.h"
#include "Common.h"
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
    std::vector<SDL_Texture*> backgroundTex;

    std::vector<GameObject> bricks;
    int trials;
    int bricksPerRow;
    int numRows;
    GameState state = Initial;
    void generateBricks();
    void loadBackground();
    void handlePaddleMovement(GameObject& paddle);
};
