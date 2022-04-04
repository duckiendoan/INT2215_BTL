#pragma once
#include "GameObject.h"
#include "Common.h"
#include "BrickManager.h"
#include "Animator.h"
#include "SoundPlayer.h"
#include "ToggleButton.h"

class Game
{
public:
    Game(int p_trials, int bricks_per_row, int num_rows);
    void run();
private:
    RenderWindow window = RenderWindow("Breakout Remake", constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT);
    SDL_Texture* paddleTex = window.loadTexture("assets/paddle.png");
    SDL_Texture* ballTex = window.loadTexture("assets/ball.png");
    SDL_Texture* brickTex = window.loadTexture("assets/bricks.png");
    SDL_Texture* pauseBtnTex = window.loadTexture("assets/pause.png");
    SDL_Texture* pauseMenuTex = window.loadTexture("assets/paused_menu.png");
    SDL_Texture* pauseMenuBtnTex = window.loadTexture("assets/pause_menu_buttons.png");
    SDL_Texture* startMenuBtnTex = window.loadTexture("assets/start_menu_buttons.png");
    SDL_Texture* gameTitleTex = window.loadTexture("assets/BreakoutRemake.png");
    SDL_Texture* backgroundTex = window.loadTexture("assets/background1.png");
    SDL_Texture* settingsMenuTex = window.loadTexture("assets/settings_menu.png");
    SDL_Texture* checkboxBtnTex = window.loadTexture("assets/checkbox.png");

    GameObject background = GameObject(0, 0, backgroundTex);
    GameObject gameTitle = GameObject(0, 0, gameTitleTex);

    Sprite brickSprite = Sprite(1, 5, brickTex);
    Sprite menuSprite = Sprite(1, 2, settingsMenuTex);

    // Pause buttons
    Button pauseBtn = Button({0, 0, 50, 50}, pauseBtnTex, 1);
    Button homeBtn = Button({0, 0, 203, 203}, pauseMenuBtnTex, 4);
    Button continueBtn = Button({0, 203, 203, 203}, pauseMenuBtnTex, 4);
    Button resetBtn = Button({0, 406, 203, 203}, pauseMenuBtnTex, 4);
    Button nextLevelBtn = Button({0, 609, 203, 203}, pauseMenuBtnTex, 4);
    ToggleButton musicCheckboxBtn = ToggleButton({0, 0, 100, 100}, checkboxBtnTex, 2);
    ToggleButton sfxCheckBoxBtn = ToggleButton({0, 0, 100, 100}, checkboxBtnTex, 2);
    Button okButton = Button({0, 100, 100, 100}, checkboxBtnTex, 2);
    GameObject pauseMenu = GameObject(0, 0, pauseMenuTex);

    // Start menu buttons
    Button startBtn = Button({0, 0, 640, 180}, startMenuBtnTex, 4);
    Button settingsBtn = Button({0, 180, 640, 180}, startMenuBtnTex, 4);
    Button helpBtn = Button({0, 360, 640, 180}, startMenuBtnTex, 4);
    Button exitBtn = Button({0, 540, 640, 180}, startMenuBtnTex, 4);

    Animator animator = Animator(window);
    SoundPlayer soundPlayer;

    BrickManager* brickManager;
    SDL_Event event;
    int attempts;
    int score;
    bool gameRunning;
    bool settingsMenu = false, helpMenu = false;

    GameState state = GAME_STATE_INITIAL;
    void update(Ball& ball, GameObject& paddle, double dt);
    void render(Ball& ball, GameObject& paddle);
    void handlePaddleMovement(GameObject& paddle, const Uint8* currentKeyStates, bool mouse_moved);
    void reset(Ball& ball, GameObject& paddle, bool next_level);
    void loadButtons();
    void loadStartMenuButtons();
    void loadPauseMenuButtons();
};
