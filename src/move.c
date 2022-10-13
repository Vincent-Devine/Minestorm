#include "move.h"
#include "my_math.h"

void rotationHitbox(Core* core, float rotation)
{
    float radAngle = rotation * MY_PI / 180.f;

    for(int i = 0; i < core->numberVector; i++)
    {
        float x = core->hitbox[i].x - core->barycentre.x;
        float y = core->hitbox[i].y - core->barycentre.y;

        core->hitbox[i].x = (x * cosf(radAngle)) - (y * sinf(radAngle)) + core->barycentre.x;
        core->hitbox[i].y = (x * sinf(radAngle)) + (y * cosf(radAngle)) + core->barycentre.y;
    }
}

void changeHitboxTP(Core* core, float barycentre, bool isX)
{
    for(int i = 0; i < core->numberVector; i++)
    {
        if(isX)
            core->hitbox[i].x += barycentre;
        else
            core->hitbox[i].y += barycentre;
    }
}

void giveRandomPosition(Core* core)
{
    changeHitboxTP(core, -1.f * core->barycentre.x, true);
    changeHitboxTP(core, -1.f * core->barycentre.y, false);
    do
    {
        core->barycentre.x = fabs((double)(SEED % (SCREEN_WIDTH - 60)));
        SEED = getNextRandomNumber(SEED);
        core->barycentre.y = fabs((double)(SEED % (SCREEN_HEIGHT - 90)));
        SEED = getNextRandomNumber(SEED);
    } while (core->barycentre.x < 60 || core->barycentre.y < 90);

    changeHitboxTP(core, core->barycentre.x, true);
    changeHitboxTP(core, core->barycentre.y, false);
}

// Movement
Vector2D calculationDirectionMineToPlayer(Core coreEnemy, Core corePlayer)
{
    Vector2D direction = (Vector2D)
    {
        corePlayer.barycentre.x - coreEnemy.barycentre.x,
        corePlayer.barycentre.y - coreEnemy.barycentre.y
    };

    float normeDirection = (sqrtf((powf(direction.x, 2.f) + powf(direction.y, 2.f))));

    direction.x = direction.x / normeDirection;
    direction.y = direction.y / normeDirection;

    return direction;
}

void movementFloatingMine(Core* core)
{
    float radAngle = core->lastThrustRotation * MY_PI / 180.f;
    float movementX = cosf(radAngle) * core->speed;
    float movementY = sinf(radAngle) * core->speed;

    core->barycentre.x += movementX;
    core->barycentre.y += movementY;

    for(int i = 0; i < core->numberVector; i++)
    {
        core->hitbox[i].x += movementX;
        core->hitbox[i].y += movementY;
    }
}

void movementMagneticMine(Core* coreEnemy, Core corePlayer)
{
    Vector2D direction = calculationDirectionMineToPlayer(*coreEnemy, corePlayer);
    
    float movementX = direction.x * coreEnemy->speed;
    float movementY = direction.y * coreEnemy->speed;

    coreEnemy->barycentre.x += movementX;
    coreEnemy->barycentre.y += movementY;

    for(int i = 0; i < coreEnemy->numberVector; i++)
    {
        coreEnemy->hitbox[i].x += movementX;
        coreEnemy->hitbox[i].y += movementY;
    }
}

void movementEnemy(Game* game)
{
    Core* enemyCore;
    for(int i = 0; i < game->enemyAmount; i++)
    {
        enemyCore = &game->enemy[i].core;
        if(enemyCore->lifePoint > 0)
        {
            if(game->enemy[i].type == ET_MAGNETIC_MINE || game->enemy[i].type == ET_MAGNETIC_FIREBALL_MINE)
            {
                if(game->numberPlayer == NP_TWO)
                {
                    
                    if(game->player1.core.lifePoint <= 0)
                        movementMagneticMine(enemyCore, game->player2.core);
                    else if(game->player2.core.lifePoint <= 0)
                        movementMagneticMine(enemyCore, game->player1.core);
                    else
                    {
                        Core closetPlayer = aimClosestPlayer(*enemyCore, game->player1.core, game->player2.core);
                        movementMagneticMine(enemyCore, closetPlayer);
                    }
                }
                else
                    movementMagneticMine(enemyCore, game->player1.core);
            }
            else if(game->enemy[i].type == ET_FLOATING_MINE || game->enemy[i].type == ET_FIREBALL_MINE)
                movementFloatingMine(enemyCore);
            switch (game->enemy[i].type)
            {
                case ET_FLOATING_MINE:
                    enemyCore->convexShapes[0] = (ConvexShape){4, {enemyCore->hitbox[0], enemyCore->hitbox[1], enemyCore->hitbox[2], enemyCore->hitbox[4]}};
                    enemyCore->convexShapes[1] = (ConvexShape){3, {enemyCore->hitbox[2], enemyCore->hitbox[3], enemyCore->hitbox[4]}};
                    enemyCore->convexShapes[2] = (ConvexShape){3, {enemyCore->hitbox[0], enemyCore->hitbox[4], enemyCore->hitbox[5]}};
                    break;
                case ET_FIREBALL_MINE:
                    enemyCore->convexShapes[0] = (ConvexShape){6, {enemyCore->hitbox[0], enemyCore->hitbox[1], enemyCore->hitbox[2], enemyCore->hitbox[4], enemyCore->hitbox[5], enemyCore->hitbox[6]}};
                    enemyCore->convexShapes[1] = (ConvexShape){3, {enemyCore->hitbox[2], enemyCore->hitbox[3], enemyCore->hitbox[4]}};
                    enemyCore->convexShapes[2] = (ConvexShape){3, {enemyCore->hitbox[0], enemyCore->hitbox[6], enemyCore->hitbox[7]}};
                    break;
                case ET_MAGNETIC_MINE:
                    enemyCore->convexShapes[0] = (ConvexShape){6, {enemyCore->hitbox[0], enemyCore->hitbox[1], enemyCore->hitbox[2], enemyCore->hitbox[4], enemyCore->hitbox[5], enemyCore->hitbox[6]}};
                    enemyCore->convexShapes[1] = (ConvexShape){3, {enemyCore->hitbox[2], enemyCore->hitbox[3], enemyCore->hitbox[4]}};
                    enemyCore->convexShapes[2] = (ConvexShape){3, {enemyCore->hitbox[0], enemyCore->hitbox[6], enemyCore->hitbox[7]}};
                    break;
                case ET_MAGNETIC_FIREBALL_MINE:
                    enemyCore->convexShapes[0] = (ConvexShape){3, {enemyCore->barycentre, enemyCore->hitbox[1], enemyCore->hitbox[2]}};
                    enemyCore->convexShapes[1] = (ConvexShape){3, {enemyCore->barycentre, enemyCore->hitbox[3], enemyCore->hitbox[4]}};
                    enemyCore->convexShapes[2] = (ConvexShape){3, {enemyCore->barycentre, enemyCore->hitbox[5], enemyCore->hitbox[6]}};
                    enemyCore->convexShapes[3] = (ConvexShape){3, {enemyCore->barycentre, enemyCore->hitbox[7], enemyCore->hitbox[0]}};
                    break;
                default:
                    break;
            }
        }
        
    }
}

void movementCalculation2D(Core* core, bool isMoving)
{
    if (core->rotation > 360.f)
        core->rotation = 0.f;
    else if (core->rotation < 0.f)
        core->rotation = 360.f;
    if (isMoving)
        core->lastThrustRotation = core->rotation;
    
    float radAngle = core->lastThrustRotation * MY_PI / 180.f;

    float movementX = cosf(radAngle) * core->speed;
    float movementY = sinf(radAngle) * core->speed;

    core->barycentre.x += movementX;
    core->barycentre.y += movementY;

    for(int i = 0; i < core->numberVector; i++)
    {
        core->hitbox[i].x += movementX;
        core->hitbox[i].y += movementY;
    }
    core->convexShapes[0] = (ConvexShape){3, {core->hitbox[0], core->hitbox[1], core->hitbox[2]}};
}

void computeInertiaForEveryone(Core* core)
{
    core->speed += 0.05f;
    if (core->speed < -6.f)
        core->speed = -6.f;
    else if (core->speed > 0.f)
        core->speed = 0.f;
    movementCalculation2D(core, false);
}

void movementFireball(FireBall* fireball)
{
    if(fireball->isActive)
    {
        fireball->hitbox.centre.x += fireball->moveVector.x;
        fireball->hitbox.centre.y += fireball->moveVector.y;
        fireball->time--;
        if(fireball->time <= 0)
            fireball->isActive = false;
    }
}

// Change Scope
void changeScope(Core* core)
{
    if(core->barycentre.x > SCREEN_WIDTH - 40.f)
    {
        changeHitboxTP(core, -1.f * core->barycentre.x, true);
        core->barycentre.x = 40.f;
        changeHitboxTP(core, core->barycentre.x, true);
    }
    else if(core->barycentre.x < 40.f)
    {
        changeHitboxTP(core, -1.f * core->barycentre.x, true);
        core->barycentre.x = SCREEN_WIDTH - 40.f;
        changeHitboxTP(core, core->barycentre.x, true);
    }
    if(core->barycentre.y > SCREEN_HEIGHT - 60.f)
    {
        changeHitboxTP(core, -1.f * core->barycentre.y, false);
        core->barycentre.y = 40.f;
        changeHitboxTP(core, core->barycentre.y, false);
    }
    else if(core->barycentre.y < 40.f)
    {
        changeHitboxTP(core, -1.f * core->barycentre.y, false);
        core->barycentre.y = SCREEN_HEIGHT - 60.f;
        changeHitboxTP(core, core->barycentre.y, false);
    }
}

void changeScopeFireball(FireBall* fireball)
{
    if(fireball->hitbox.centre.x > SCREEN_WIDTH - 40.f)
    {
        fireball->hitbox.centre.x = 40.f;
    }
    else if(fireball->hitbox.centre.x < 40.f)
    {
        fireball->hitbox.centre.x = SCREEN_WIDTH - 40.f;
    }
    if(fireball->hitbox.centre.y > SCREEN_HEIGHT - 60.f)
    {
        fireball->hitbox.centre.y = 40.f;
    }
    else if(fireball->hitbox.centre.y < 40.f)
    {
        fireball->hitbox.centre.y = SCREEN_HEIGHT - 60.f;
    }
}