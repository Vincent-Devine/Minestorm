#pragma once
#include "object.h"
#include "game.h"

// Hitbox
void rotationHitbox(Core* core, float rotation);
void changeHitboxTP(Core* core, float barycentre, bool isX);

void giveRandomPosition(Core* core);

// Movement
Vector2D calculationDirectionMineToPlayer(Core coreEnemy, Core corePlayer);
void movementFloatingMine(Core* core);
void movementMagneticMine(Core* coreEnemy, Core corePlayer);
void movementEnemy(Game* game);
void movementCalculation2D(Core* core, bool isMoving);
void computeInertiaForEveryone(Core* core);
void movementFireball(FireBall* fireball);

// Change Scope
void changeScope(Core* core);
void changeScopeFireball(FireBall* fireball);