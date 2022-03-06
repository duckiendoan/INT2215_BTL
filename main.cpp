#include <iostream>
#include <SDL2/SDL.h>
#include "Game.h"
using namespace std;

const int BRICKS_PER_ROW = 8;
const int NUM_OF_ROWS = 5;

int main(int argc, char* args[]) {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
        cout << "sdl error: " << SDL_GetError() << endl;
    Game g(3, BRICKS_PER_ROW, NUM_OF_ROWS);
    g.run();
    SDL_Quit();
    return 0;
}