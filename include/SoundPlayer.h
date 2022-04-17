#pragma once
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <map>
#include <string>

class SoundPlayer {
public:
    SoundPlayer();
    void playMusic();
    void pauseMusic();
    void resumeMusic();
    void playSfx(const std::string& effect);
    bool musicPlaying = true, sfxPlaying = true;
private:
    Mix_Music* music;
    std::map<std::string, Mix_Chunk*> chunkMap;
};