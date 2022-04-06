#include "SoundPlayer.h"

SoundPlayer::SoundPlayer() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 16);
    music = Mix_LoadMUS("assets/audio/background_music.mp3");
    if (!music)
        std::cout << "Error loading music: " << SDL_GetError() << std::endl;
    chunkMap.emplace(std::string("button_click"), Mix_LoadWAV("assets/audio/button_click.wav"));
    chunkMap.emplace("collision", Mix_LoadWAV("assets/audio/collision.wav"));
}

void SoundPlayer::playMusic()
{
    Mix_PlayMusic(music, -1);
}

void SoundPlayer::pauseMusic() {
    Mix_PauseMusic();
}

void SoundPlayer::resumeMusic() {
    Mix_ResumeMusic();
}

void SoundPlayer::playSfx(const std::string& effect) {
    if (sfxPlaying)
        Mix_PlayChannel(-1, chunkMap[effect], 0);
}

