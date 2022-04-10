#include "Game.h"

void Game::loadButtons()
{
    pauseBtn.setScale(1);
    pauseBtn.x = constants::SCREEN_WIDTH - pauseBtn.getWidth() - 10;
    pauseBtn.y = 7;

    gameTitle.setScale(0.5 * constants::SCREEN_WIDTH / gameTitle.getWidth());
    gameTitle.x = constants::SCREEN_WIDTH * 0.5 - gameTitle.getWidth() * 0.5;
    gameTitle.y = constants::SCREEN_HEIGHT * 0.35 - gameTitle.getHeight() * 0.35;
    loadPauseMenuButtons();
    loadStartMenuButtons();
}

void Game::loadStartMenuButtons()
{
    startBtn.setScale(0.2 * constants::SCREEN_WIDTH / startBtn.srcRect.w);
    startBtn.x = constants::SCREEN_WIDTH * 0.5 - startBtn.getWidth() * 0.5;
    startBtn.y = constants::SCREEN_HEIGHT * 0.5 - startBtn.getHeight() * 0.5;

    settingsBtn.setScale(startBtn.getScale());
    settingsBtn.x = startBtn.x;
    settingsBtn.y = startBtn.y + startBtn.getHeight();

    helpBtn.setScale(startBtn.getScale());
    helpBtn.x = startBtn.x;
    helpBtn.y = settingsBtn.y + settingsBtn.getHeight();

    exitBtn.setScale(startBtn.getScale());
    exitBtn.x = startBtn.x;
    exitBtn.y = helpBtn.y + helpBtn.getHeight();

    menuSprite.setScale(startBtn.getScale());
    menuSprite.x = constants::SCREEN_WIDTH * 0.5 - menuSprite.getWidth() * 0.5;
    menuSprite.y = constants::SCREEN_HEIGHT * 0.5 - menuSprite.getHeight() * 0.5;

    okButton.setScale(startBtn.getScale());
    okButton.x = menuSprite.x + menuSprite.getWidth() * 0.5f - 0.5f * okButton.getWidth();
    okButton.y = menuSprite.y + menuSprite.getHeight() * 0.82f;

    musicCheckboxBtn.setScale(startBtn.getScale() + 0.1);
    sfxCheckBoxBtn.setScale(startBtn.getScale() + 0.1);
    twoBallCheckBoxBtn.setScale(musicCheckboxBtn.getScale());

    musicCheckboxBtn.x = menuSprite.x + menuSprite.getWidth() * 0.75f;
    sfxCheckBoxBtn.x = musicCheckboxBtn.x;
    twoBallCheckBoxBtn.x = musicCheckboxBtn.x;

    musicCheckboxBtn.y = menuSprite.y + menuSprite.getHeight() * 0.4f - musicCheckboxBtn.getHeight() * 0.62f;
    sfxCheckBoxBtn.y = musicCheckboxBtn.y + 1.1f * musicCheckboxBtn.getHeight();
    twoBallCheckBoxBtn.y = sfxCheckBoxBtn.y + 1.1f * sfxCheckBoxBtn.getHeight();

    twoBallCheckBoxBtn.swapState();
}

void Game::loadPauseMenuButtons()
{
    pauseMenu.setScale(0.4 * constants::SCREEN_WIDTH / pauseMenu.getWidth());
    pauseMenu.x = 0.5f * constants::SCREEN_WIDTH - 0.5f * pauseMenu.getWidth();
    pauseMenu.y = 0.5f * constants::SCREEN_HEIGHT - 0.5f * pauseMenu.getHeight();

    int btnWidth = homeBtn.srcRect.w;
    homeBtn.setScale(0.09 * constants::SCREEN_WIDTH / btnWidth);
    continueBtn.setScale(homeBtn.getScale());
    resetBtn.setScale(homeBtn.getScale());

    float gap = float(pauseMenu.getWidth() - homeBtn.getWidth() * 4) / 2;
    homeBtn.x = pauseMenu.x + 0.5f * homeBtn.getWidth();
    homeBtn.y = pauseMenu.y + pauseMenu.getHeight() * 0.45f;

    continueBtn.x = homeBtn.x + homeBtn.getWidth() + gap;
    continueBtn.y = homeBtn.y;

    resetBtn.x = continueBtn.x + homeBtn.getWidth() + gap;
    resetBtn.y = homeBtn.y;
}