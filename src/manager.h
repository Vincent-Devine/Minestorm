#pragma once
#include <raylib.h>
#include "game.h"
#include "object.h"

// Game management
int menuManagement(GameScreen* currentScreen, Texture2D backGround, Texture2D foreGround, Game* game);
void gameManagement(GameScreen* currentScreen, Texture2D backGround, Texture2D foreGround, Texture2D sprite, Game* game);
void pauseMenuManagement(GameScreen* currentScreen, Texture2D backGround, Texture2D foreGround, Game* game);
void gameOverManagement(GameScreen* currentScreen, Texture2D backGround, Texture2D foreGround, Game* game);
void victoryManagement(GameScreen* currentScreen, Texture2D backGround, Texture2D foreGround, Game* game);
int screenChoice(GameScreen* currentScreen, Texture2D backGround, Texture2D foreGround, Texture2D sprite, Game* game);