#pragma once
#include <raylib.h>
#include "game.h"

// Game
void displayPlayer(Texture2D sprite, Character* player, bool isPlayer2, int time);
void displayEnemy(Texture2D sprite, Enemy* enemy);
void displayFireball(Texture2D sprite, FireBall* fireball);
void displaySpawnPoint(Texture2D sprite, Vector2D centre);

// Game screen
void displayMenu();
void displayGame(Texture2D sprite, Game* game);
void displayPauseMenu();
void displayGameOver(int score);
void displayVictoryScreen(int score);
void displayLifePlayer(Game game);
void displayGameInfo(Game* game);
void displayCommandPlayer();

// debug
void displayDebug(Core* core);
void displayHitbox(Game* game);