#pragma once
#include "object.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 800
#define SPRITE_WIDTH 1024
#define SPRITE_HEIGHT 512
#define MAX_FIREBALL 100
#define LEVEL_TOTAL 6

extern int32_t SEED;

typedef enum GameScreen
{
    GS_MENU,
    GS_GAME,
    GS_PAUSE_MENU,
    GS_GAME_OVER,
    GS_VICTORY, 
}GameScreen;

typedef enum NumberPlayer
{
    NP_ONE,
    NP_TWO,
}NumberPlayer;

typedef struct Game
{
    NumberPlayer numberPlayer;
    Character player1;
    Character player2;
    int score;
    int enemyAmount;
    int enemyTotal;
    Enemy enemy[100];
    int numberFireball;
    FireBall fireBall[100];
    int timeElapsedFromStart;
    int level;
}Game;

bool isCollidingAABB(Core* coreA, Core* coreB);
bool isCollidingSAT(Core* coreA, Core* coreB);

// Random
int32_t getNextRandomNumber(int32_t state);
void giveRandomRotation(Core* core);

// Save
void saveSeed();
void loadSeed();

void shoot(Game* game, Core* playerCore);
void enemyShoot(Game* game, Core* enemyCore);

void checkCollidingPlayer(Game* game);
Core aimClosestPlayer(Core coreEnemy, Core player1, Core player2);
void checkCollidingFireball(Game* game);