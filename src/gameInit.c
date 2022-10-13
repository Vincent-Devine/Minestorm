#include "gameInit.h"
#include "move.h"

void initPlayer(Core* core, bool have2Player, bool isPlayer2)
{
    core->rotation = 90.f;
    core->lastThrustRotation = 0.f;
    core->speed = 0.f;
    if(have2Player)
    {
        core->barycentre.x = SCREEN_WIDTH / 3;
        core->barycentre.y = 600;
        if(isPlayer2)
        {
            core->barycentre.x = SCREEN_WIDTH - (SCREEN_WIDTH / 3);
            core->barycentre.y = 600;
        }
    }
    else
    {
        core->barycentre.x = SCREEN_WIDTH / 2;
        core->barycentre.y = 600;
    }
    core->hitbox[0] = (Vector2D){core->barycentre.x, core->barycentre.y - 35.f};
    core->hitbox[1] = (Vector2D){core->barycentre.x - 27.f, core->barycentre.y + 35.f};
    core->hitbox[2] = (Vector2D){core->barycentre.x + 27.f, core->barycentre.y + 35.f};
    core->numberVector = 3;
    core->convexShapeAmount = 1;
    core->convexShapes[0] = (ConvexShape){3, {core->hitbox[0], core->hitbox[1], core->hitbox[2]}};
    core->lifePoint = 3;
}

void enemyHitboxAndSpeedInit(Enemy* enemy)
{
    Core *core = &enemy->core;
    core->speed = 1.f;
    float splitter = 1.f;
    if (enemy->size == ES_MEDIUM)
    {
        splitter = 1.5f;
        core->speed = 1.5f;
    }
    else if (enemy->size == ES_SMALL)
    {
        splitter = 2.0f;
        core->speed = 2.f;
    }
    switch (enemy->type)
    {
    case ET_FLOATING_MINE:
        core->numberVector = 6;
        core->hitbox[0] = (Vector2D){core->barycentre.x, core->barycentre.y - 8.f / splitter};
        core->hitbox[1] = (Vector2D){core->barycentre.x - 25.f / splitter, core->barycentre.y};
        core->hitbox[2] = (Vector2D){core->barycentre.x, core->barycentre.y + 8.f / splitter};
        core->hitbox[3] = (Vector2D){core->barycentre.x + 27.5f / splitter, core->barycentre.y + 28.5f / splitter};
        core->hitbox[4] = (Vector2D){core->barycentre.x + 25.f / splitter, core->barycentre.y};
        core->hitbox[5] = (Vector2D){core->barycentre.x + 27.5f / splitter, core->barycentre.y - 28.5 / splitter};
        break;
    case ET_FIREBALL_MINE:
        core->numberVector = 8;
        core->hitbox[0] = (Vector2D){core->barycentre.x, core->barycentre.y - 20.f / splitter};
        core->hitbox[1] = (Vector2D){core->barycentre.x - 28.f / splitter, core->barycentre.y - 28.f / splitter};
        core->hitbox[2] = (Vector2D){core->barycentre.x - 20.f / splitter, core->barycentre.y};
        core->hitbox[3] = (Vector2D){core->barycentre.x - 28.f / splitter, core->barycentre.y + 28.f / splitter};
        core->hitbox[4] = (Vector2D){core->barycentre.x, core->barycentre.y + 20.f / splitter};
        core->hitbox[5] = (Vector2D){core->barycentre.x + 28.f / splitter, core->barycentre.y + 28.f / splitter};
        core->hitbox[6] = (Vector2D){core->barycentre.x + 20.f / splitter, core->barycentre.y};
        core->hitbox[7] = (Vector2D){core->barycentre.x + 28.f / splitter, core->barycentre.y - 28.f / splitter};
        break;
    case ET_MAGNETIC_MINE:
        core->numberVector = 8;
        core->hitbox[0] = (Vector2D){core->barycentre.x, core->barycentre.y - 12.f / splitter};
        core->hitbox[1] = (Vector2D){core->barycentre.x - 25.f / splitter, core->barycentre.y - 24.f / splitter};
        core->hitbox[2] = (Vector2D){core->barycentre.x - 12.f / splitter, core->barycentre.y};
        core->hitbox[3] = (Vector2D){core->barycentre.x - 25.f / splitter, core->barycentre.y + 24.f / splitter};
        core->hitbox[4] = (Vector2D){core->barycentre.x, core->barycentre.y + 12.f / splitter};
        core->hitbox[5] = (Vector2D){core->barycentre.x + 25.f / splitter, core->barycentre.y + 24.f / splitter};
        core->hitbox[6] = (Vector2D){core->barycentre.x + 12.f / splitter, core->barycentre.y};
        core->hitbox[7] = (Vector2D){core->barycentre.x + 25.f / splitter, core->barycentre.y - 24.f / splitter};
        break;
    case ET_MAGNETIC_FIREBALL_MINE:
        core->numberVector = 8;
        core->hitbox[0] = (Vector2D){core->barycentre.x - 15.f / splitter, core->barycentre.y - 22.f / splitter};
        core->hitbox[1] = (Vector2D){core->barycentre.x - 22.f / splitter, core->barycentre.y - 37.f / splitter};
        core->hitbox[2] = (Vector2D){core->barycentre.x - 22.f / splitter, core->barycentre.y + 15.f / splitter};
        core->hitbox[3] = (Vector2D){core->barycentre.x - 36.f / splitter, core->barycentre.y + 22.f / splitter};
        core->hitbox[4] = (Vector2D){core->barycentre.x + 15.f / splitter, core->barycentre.y + 22.f / splitter};
        core->hitbox[5] = (Vector2D){core->barycentre.x + 22.f / splitter, core->barycentre.y + 37.f / splitter};
        core->hitbox[6] = (Vector2D){core->barycentre.x + 22.f / splitter, core->barycentre.y - 15.f / splitter};
        core->hitbox[7] = (Vector2D){core->barycentre.x + 36.f / splitter, core->barycentre.y - 22.f / splitter};
        break;
    case ET_MINE_LAYER:
        //TODO
        break;
    }
}

void initEnemyConvexShape(Enemy* enemy)
{
    Core *core = &enemy->core;
    if (enemy->type == ET_FLOATING_MINE)
    {
        core->convexShapeAmount = 3;
        core->convexShapes[0] = (ConvexShape){4, {core->hitbox[0], core->hitbox[1], core->hitbox[2], core->hitbox[4]}};
        core->convexShapes[1] = (ConvexShape){3, {core->hitbox[2], core->hitbox[3], core->hitbox[4]}};
        core->convexShapes[2] = (ConvexShape){3, {core->hitbox[0], core->hitbox[4], core->hitbox[5]}};
    }
    else if (enemy->type == ET_FIREBALL_MINE || enemy->type == ET_MAGNETIC_MINE)
    {
        core->convexShapeAmount = 3;
        core->convexShapes[0] = (ConvexShape){6, {core->hitbox[0], core->hitbox[1], core->hitbox[2], core->hitbox[4], core->hitbox[5], core->hitbox[6]}};
        core->convexShapes[1] = (ConvexShape){3, {core->hitbox[2], core->hitbox[3], core->hitbox[4]}};
        core->convexShapes[2] = (ConvexShape){3, {core->hitbox[0], core->hitbox[6], core->hitbox[7]}};
    }
    else if (enemy->type == ET_MAGNETIC_FIREBALL_MINE)
    {
        core->convexShapeAmount = 4;
        core->convexShapes[0] = (ConvexShape){3, {core->barycentre, core->hitbox[1], core->hitbox[2]}};
        core->convexShapes[1] = (ConvexShape){3, {core->barycentre, core->hitbox[3], core->hitbox[4]}};
        core->convexShapes[2] = (ConvexShape){3, {core->barycentre, core->hitbox[5], core->hitbox[6]}};
        core->convexShapes[3] = (ConvexShape){3, {core->barycentre, core->hitbox[7], core->hitbox[0]}};
    }
    else if (enemy->type == ET_MINE_LAYER)
    {
        //TODO
    }
    
    if (enemy->type == ET_FLOATING_MINE || enemy->type == ET_FIREBALL_MINE)
        giveRandomRotation(&enemy->core);
    
}

void initEnemy(Enemy* enemy)
{
    Core *core = &enemy->core;
    core->rotation = 90.f;
    core->lifePoint = 1;
    enemy->hasSpawned = true;
    enemy->minionSpawned = 0;
    enemyHitboxAndSpeedInit(enemy);
    initEnemyConvexShape(enemy);
}

void levelInit(Game* game)
{
    switch (game->level)
    {
    case 1:
        game->enemy[0].type = ET_FLOATING_MINE;
        game->enemy[1].type = ET_FLOATING_MINE;
        game->enemy[2].type = ET_FLOATING_MINE;
        break;
    case 2:
        game->enemy[0].type = ET_FLOATING_MINE;
        game->enemy[1].type = ET_FLOATING_MINE;
        game->enemy[2].type = ET_MAGNETIC_MINE;
        break;
    case 3:
        game->enemy[0].type = ET_MAGNETIC_MINE;
        game->enemy[1].type = ET_MAGNETIC_MINE;
        game->enemy[2].type = ET_FIREBALL_MINE;
        break;
    case 4:
        game->enemy[0].type = ET_MAGNETIC_FIREBALL_MINE;
        game->enemy[1].type = ET_MAGNETIC_MINE;
        game->enemy[2].type = ET_FIREBALL_MINE;
        break;
    case 5:
        game->enemyTotal = 28;
        game->enemy[0].type = ET_MAGNETIC_FIREBALL_MINE;
        game->enemy[1].type = ET_MAGNETIC_FIREBALL_MINE;
        game->enemy[2].type = ET_FIREBALL_MINE;
        game->enemy[3].type = ET_FIREBALL_MINE;
        break;
    case 6:
        game->enemyTotal = 49;
        game->enemy[0].type = ET_FLOATING_MINE;
        game->enemy[1].type = ET_FLOATING_MINE;
        game->enemy[2].type = ET_FLOATING_MINE;
        game->enemy[3].type = ET_FLOATING_MINE;
        game->enemy[4].type = ET_FLOATING_MINE;
        game->enemy[5].type = ET_FLOATING_MINE;
        game->enemy[6].type = ET_FLOATING_MINE;
        break;
    default:
        break;
    }
    if (game->level < 5)
        game->enemyTotal = 21;
    game->enemyAmount = game->enemyTotal / 7;
    for (int i = 0; i < game->enemyAmount; i++)
        game->enemy[i].size = ES_BIG;
}

void startNewLevel(Game* game)
{
    if (game->level < LEVEL_TOTAL)
        game->level++;
    for (int i = 0; i < game->enemyTotal; i++)
    {
        game->enemy[i].core.lifePoint = 0;
        game->enemy[i].hasSpawned = false;
        giveRandomPosition(&game->enemy[i].core);
    }
    game->timeElapsedFromStart = 0;
    levelInit(game);
    for (int i = 0; i < game->enemyTotal; i++)
        giveRandomPosition(&game->enemy[i].core);
}

void gameInit(Game *game)
{
    if(game->numberPlayer == NP_TWO)
    {
        initPlayer(&game->player1.core, true, false);
        initPlayer(&game->player2.core, true, true);
    }
    else
        initPlayer(&game->player1.core, false, false);
    startNewLevel(game);        
}