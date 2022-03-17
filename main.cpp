#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Game.h"
using namespace std;

const int BRICKS_PER_ROW = 8;
const int NUM_OF_ROWS = 5;

int main(int argc, char* args[]) {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
        cout << "SDL error: " << SDL_GetError() << endl;
    if (TTF_Init() != 0)
        cout << "SDL_ttf error: " << TTF_GetError() << endl;
    Game g(3, 7, 2);
    g.run();

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    return 0;
}