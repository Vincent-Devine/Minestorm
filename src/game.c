#include "game.h"
#include "gameInit.h"

Vec2mM getMinAndMax(Core* core)
{
    Vec2mM minMaxValues = {{9999999.f, 9999999.f}, {-9999999.f, -9999999.f}};
    for (int i = 0; i < core->convexShapeAmount; i++)
    {
        ConvexShape convShape = core->convexShapes[i];
        for (int j = 0; j < convShape.vertexAmount; j++)
        {
            if (convShape.vertices[j].x < minMaxValues.min.x)
                minMaxValues.min.x = convShape.vertices[j].x;
            if (convShape.vertices[j].x > minMaxValues.max.x)
                minMaxValues.max.x = convShape.vertices[j].x;
            if (convShape.vertices[j].y < minMaxValues.min.y)
                minMaxValues.min.y = convShape.vertices[j].y;
            if (convShape.vertices[j].y > minMaxValues.max.y)
                minMaxValues.max.y = convShape.vertices[j].y;
        }  
    }
    return minMaxValues;
}

bool isCollidingAABB(Core* coreA, Core* coreB)
{
    Vec2mM a = getMinAndMax(coreA);
    Vec2mM b = getMinAndMax(coreB);
    if ((b.min.x > a.max.x)
	|| (b.max.x < a.min.x)
	|| (b.min.y > a.max.y)
	|| (b.max.y < a.min.y))
        return false;
    else
        return true;
}

bool noSpaceBetween(Core* coreA, Core* coreB)
{
    float minA = 9999999.f, minB= 9999999.f, maxA = -9999999.f, maxB = -9999999.f, dotProduct;
    for (int i = 0; i < coreA->convexShapeAmount; i++)
    {
        ConvexShape convShapeA = coreA->convexShapes[i];
        for (int j = 0; j < convShapeA.vertexAmount; j++)
        {
            Vector2D normal = (Vector2D){ -1.f * (convShapeA.vertices[(j + 1) % convShapeA.vertexAmount].y - convShapeA.vertices[j].y), (convShapeA.vertices[(j + 1) % convShapeA.vertexAmount].x - convShapeA.vertices[j].x) };
            for (int k = 0; k < coreB->convexShapeAmount; k++)
            {
                ConvexShape convShapeB = coreB->convexShapes[k];
                for (int l = 0; l < convShapeB.vertexAmount; l++)
                {
                    Vector2D ab = (Vector2D) {convShapeB.vertices[l].x - convShapeA.vertices[j].x, convShapeB.vertices[l].y - convShapeA.vertices[j].y};
                    dotProduct = normal.x * ab.x + normal.y * ab.y;
                    if (dotProduct < minB)
                        minB = dotProduct;
                    if (dotProduct > maxB)
                        maxB = dotProduct;
                }
            }
            for (int m = 0; m < convShapeA.vertexAmount; m++)
            {
                Vector2D aa = (Vector2D) {convShapeA.vertices[m].x - convShapeA.vertices[j].x, convShapeA.vertices[m].y - convShapeA.vertices[j].y};
                dotProduct = normal.x * aa.x + normal.y * aa.y;
                if (dotProduct < minA)
                    minA = dotProduct;
                if (dotProduct > maxA)
                    maxA = dotProduct;
            }
            if (minA > maxB || minB > maxA)
                return false;
        }
    }
    return true;
}

bool isCollidingSAT(Core* coreA, Core* coreB)
{
    if(!isCollidingAABB(coreA, coreB))
        return false;
    return noSpaceBetween(coreA,coreB) && noSpaceBetween(coreB,coreA);
}

// Random
int32_t SEED = 0;

int32_t getNextRandomNumber(int32_t state)
{
    uint64_t result = (((int64_t)1103515245) * state + 12345) % 2147483648;
    return result;
};

void giveRandomRotation(Core* core)
{
    core->lastThrustRotation = fabs((double)(SEED % 360));
    SEED = getNextRandomNumber(SEED);
}

// Save
void loadSeed()
{
    FILE *save = NULL;
    save = fopen("saveSeed.bin", "rb");

    if (save != NULL)
    {
        fread(&SEED, sizeof(int32_t), 1, save);
        fclose(save);
    }
}

void saveSeed()
{
    FILE *save = fopen("saveSeed.bin", "wb");

    if (save != NULL)
    {
        fwrite(&SEED, sizeof(int32_t), 1, save);
        fclose(save);
    }
}

int researchFreeSpotFireball(Game *game)
{
    for (int i = 0; i < MAX_FIREBALL; i++)
    {
        if (!game->fireBall[i].isActive)
            return i;
    }
    return 0;
}

void shoot(Game *game, Core *playerCore)
{
    int countFireball = game->numberFireball;
    if (countFireball >= MAX_FIREBALL)
    {
        countFireball = researchFreeSpotFireball(game);
        game->numberFireball--;
    }

    FireBall *fireBall = &game->fireBall[countFireball];

    fireBall->moveVector = (Vector2D){
        (playerCore->hitbox[0].x - playerCore->barycentre.x) / sqrtf(powf(playerCore->hitbox[0].x - playerCore->barycentre.x, 2.f) + powf(playerCore->hitbox[0].y - playerCore->barycentre.y, 2.f)) * 12.f,
        (playerCore->hitbox[0].y - playerCore->barycentre.y) / sqrtf(powf(playerCore->hitbox[0].x - playerCore->barycentre.x, 2.f) + powf(playerCore->hitbox[0].y - playerCore->barycentre.y, 2.f)) * 12.f
    };

    fireBall->hitbox.centre = (Vector2D){
        playerCore->hitbox[0].x + fireBall->moveVector.x / 2.4f,
        playerCore->hitbox[0].y + fireBall->moveVector.y / 2.4f
    };
    fireBall->hitbox.radius = 5.f;

    fireBall->time = 30;
    fireBall->isActive = true;
    game->numberFireball++;
}

void enemyShoot(Game *game, Core *enemyCore)
{
    int countFireball = game->numberFireball;
    if (countFireball >= MAX_FIREBALL)
    {
        countFireball = researchFreeSpotFireball(game);
        game->numberFireball--;
    }

    FireBall *fireBall = &game->fireBall[countFireball];
    
    Core closestPlayer;
    if (game->numberPlayer == NP_ONE)
        closestPlayer = game->player1.core;
    else
        closestPlayer = aimClosestPlayer(*enemyCore, game->player1.core, game->player2.core);
    Vector2D vectEnemyPlayer = (Vector2D){
        closestPlayer.barycentre.x - enemyCore->barycentre.x,
        closestPlayer.barycentre.y - enemyCore->barycentre.y};
    float magnitude = sqrtf(powf(vectEnemyPlayer.x, 2.f) + powf(vectEnemyPlayer.y, 2.f));
    fireBall->moveVector = (Vector2D){
        vectEnemyPlayer.x / magnitude * 2.f,
        vectEnemyPlayer.y / magnitude * 2.f
    };
    fireBall->hitbox.centre = (Vector2D){
        enemyCore->barycentre.x + fireBall->moveVector.x * 3.f,
        enemyCore->barycentre.y + fireBall->moveVector.y * 3.f
    };
    fireBall->hitbox.radius = 5.f;

    fireBall->time = 180;
    fireBall->isActive = true;
    game->numberFireball++;
}

void calculatedScore(Game* game, Enemy enemy)
{
    switch (enemy.type)
    {
    case ET_FLOATING_MINE:
        if(enemy.size == ES_BIG)
            game->score += 100;
        else if(enemy.size == ES_MEDIUM)
            game->score += 135;
        else if(enemy.size == ES_SMALL)
            game->score += 200;
        break;
    
    case ET_FIREBALL_MINE:
        if(enemy.size == ES_BIG)
            game->score += 325;
        else if(enemy.size == ES_MEDIUM)
            game->score += 360;
        else if(enemy.size == ES_SMALL)
            game->score += 425;
        break;
    
    case ET_MAGNETIC_MINE:
        if(enemy.size == ES_BIG)
            game->score += 500;
        else if(enemy.size == ES_MEDIUM)
            game->score += 535;
        else if(enemy.size == ES_SMALL)
            game->score += 600;
        break;

    case ET_MAGNETIC_FIREBALL_MINE:
        if(enemy.size == ES_BIG)
            game->score += 750;
        else if(enemy.size == ES_MEDIUM)
            game->score += 585;
        else if(enemy.size == ES_SMALL)
            game->score += 850;
        break;

    default:
        break;
    }
}

void checkCollidingPlayer(Game *game)
{
    for (int i = 0; i < game->enemyAmount; i++)
    {
        if (game->enemy[i].core.lifePoint > 0)
        {
            if (game->player1.core.lifePoint > 0 && game->player1.invisibility < 1)           
                if (isCollidingSAT(&game->player1.core, &game->enemy[i].core))
                {
                    game->player1.core.lifePoint--;
                    game->player1.invisibility = 60.f;
                }
            if(game->numberPlayer == NP_TWO && game->player2.core.lifePoint > 0 && game->player2.invisibility < 1)
            {
                if (isCollidingSAT(&game->player2.core, &game->enemy[i].core))
                {
                    game->player2.core.lifePoint--;
                    game->player2.invisibility = 60.f;
                }
            }
        }
    }
}

void checkCollidingFireballWithPlayer(Game* game)
{
    for(int i = 0; i < game->numberFireball; i++)
    {
        if(game->fireBall[i].isActive)
        {
            if (game->numberPlayer == NP_ONE || (game->numberPlayer == NP_TWO && game->player1.core.lifePoint > 0))
            {
                for(int j = 0; j < game->player1.core.numberVector; j++)
                {
                    if(collisionSegmentCircle2D(game->fireBall[i].hitbox, game->player1.core.hitbox[j], game->player1.core.hitbox[(j + 1) % game->player1.core.numberVector]) && game->player1.invisibility < 1)
                    {
                        game->fireBall[i].isActive = false;
                        game->player1.invisibility = 60.f;
                        game->player1.core.lifePoint--;
                    }
                }
            }  
            if(game->numberPlayer == NP_TWO && game->player2.core.lifePoint > 0)
            {
                for(int j = 0; j < game->player2.core.numberVector; j++)
                {
                    if(collisionSegmentCircle2D(game->fireBall[i].hitbox, game->player2.core.hitbox[j], game->player2.core.hitbox[(j + 1) % game->player2.core.numberVector]) && game->player2.invisibility < 1)
                    {
                        game->fireBall[i].isActive = false;
                        game->player2.invisibility = 60.f;
                        game->player2.core.lifePoint--;
                    }
                }
            }
        }
    }
}

// Calculate distance between players and a given enemy and return the closet player core
Core aimClosestPlayer(Core coreEnemy, Core player1, Core player2)
{
    float distancePlayer1 = vectorMagnitude2D(coreEnemy.barycentre, player1.barycentre);
    float distancePlayer2 = vectorMagnitude2D(coreEnemy.barycentre, player2.barycentre);
    if(distancePlayer1 <= distancePlayer2)
        return player1;
    else
        return player2;
}

void checkCollidingFireballWithEnemy(Game* game)
{
    for(int i = 0; i < game->numberFireball; i++)
        if(game->fireBall[i].isActive)
            for(int j = 0; j < game->enemyAmount; j++)
                if (game->enemy[j].core.lifePoint > 0)
                    for(int t = 0; t < game->enemy[j].core.numberVector; t++)
                        if(collisionSegmentCircle2D(game->fireBall[i].hitbox, game->enemy[j].core.hitbox[t], game->enemy[j].core.hitbox[(t + 1) % game->enemy[j].core.numberVector]))
                        {
                            calculatedScore(game, game->enemy[j]);
                            game->enemy[j].core.lifePoint = 0;
                            game->fireBall[i].isActive = false;
                            if(game->enemy[j].type == ET_FIREBALL_MINE || game->enemy[j].type == ET_MAGNETIC_FIREBALL_MINE)
                                enemyShoot(game, &game->enemy[j].core);
                            if (game->enemy[j].size == ES_SMALL)
                                break;
                            while (game->enemy[j].minionSpawned < 2)
                            {
                                Enemy* enemy = &game->enemy[game->enemyAmount];
                                if (game->enemy[j].size == ES_BIG)
                                    enemy->size = ES_MEDIUM;
                                if (game->enemy[j].size == ES_MEDIUM)
                                    enemy->size = ES_SMALL;
                                enemy->type = game->enemy[j].type;
                                initEnemy(enemy);
                                game->enemyAmount++;
                                game->enemy[j].minionSpawned++;
                            }
                        }
}

void checkCollidingFireballWithFireball(Game* game)
{
    for (int i = 0; i < game->numberFireball; i++)
        if (game->fireBall[i].isActive)
            for (int j = i + 1; j < game->numberFireball; j++)
                if (game->fireBall[j].isActive)
                    if (collisionCircleCircle2D(game->fireBall[i].hitbox, game->fireBall[j].hitbox))
                    {
                        game->score += 110;
                        game->fireBall[i].isActive = false;
                        game->fireBall[j].isActive = false;
                    }
}

void checkCollidingFireball(Game* game)
{
    if(game->player1.invisibility > 0)
        game->player1.invisibility--;
    if(game->numberPlayer == NP_TWO && game->player2.invisibility > 0)
        game->player2.invisibility--;
    checkCollidingFireballWithPlayer(game);
    checkCollidingFireballWithEnemy(game);
    checkCollidingFireballWithFireball(game);
}