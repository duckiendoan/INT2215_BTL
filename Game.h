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
    SDL_Texture* pauseMenuTex = window.loadTexture("assets\\paused_menu.png");
    SDL_Texture* pauseMenuBtnTex = window.loadTexture("assets\\pause_menu_buttons.png");

    std::vector<SDL_Texture*> backgroundTex;
    std::vector<GameObject> bricks;
    Button pauseBtn = Button({0, 0, 50, 50}, pauseBtnTex, 1);
    Button homeBtn = Button({0, 0, 203, 203}, pauseMenuBtnTex, 4);
    Button continueBtn = Button({0, 203, 203, 203}, pauseMenuBtnTex, 4);
    Button resetBtn = Button({0, 609, 203, 203}, pauseMenuBtnTex, 4);
    GameObject pauseMenu = GameObject(0, 0, pauseMenuTex);

    SDL_Event event;
    int trials;
    int bricksPerRow;
    int numRows;
    int remainingBricks;
    int score;
    bool gameRunning;
    GameState state = GAME_STATE_INITIAL;
    void generateBricks();
    void loadBackground();
    void update(Ball& ball, GameObject& paddle, double dt);
    void render(Ball& ball, GameObject& paddle, GameObject& background);
    void handlePaddleMovement(GameObject& paddle, const Uint8* currentKeyStates, bool mouse_moved);
    void reset(Ball& ball, GameObject& paddle, bool next_level);
    void loadButtons();
};
