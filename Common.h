#pragma once

namespace constants
{
    const double PI = 3.141592;
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;
    const float DEFAULT_BALL_SPEED = 600.0; // 300 px/second
    //inline const char* DEFAULT_GAME_TITLE = "Test Game";
    const int BRICK_GAP = 3;
    const int BRICK_MARGIN = 25;
}

enum FontType
{
    Default,
    Menu,
    Score,
    GameOver
};

enum GameState
{
    GAME_STATE_INITIAL,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_WON,
    GAME_STATE_LOST
};