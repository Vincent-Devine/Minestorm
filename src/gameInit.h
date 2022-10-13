#pragma once
#include "game.h"
#include "object.h"

void initPositions(Game* game);
void initPlayer(Core* core, bool have2Player, bool isPlayer2);
void initEnemy(Enemy* enemy);
void initFireball(Core* core);
void startNewLevel(Game* game);
void gameInit(Game* game);