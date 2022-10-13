#include "display.h" 
#include "manager.h"
#include "move.h"

// Game
void displayPlayer(Texture2D sprite, Character* player, bool isPlayer2, int time)
{
    Core* core = &player->core;
    Rectangle textureBound = (Rectangle){
        0.f,
        0.f,
        SPRITE_WIDTH / 4.f,
        SPRITE_HEIGHT / 2.f
    };
    Rectangle destRec = {core->barycentre.x, core->barycentre.y, SPRITE_WIDTH/8, SPRITE_HEIGHT/4};

    Vector2 barycentre = (Vector2){destRec.width/2, destRec.height/2};

    changeScope(core);
    if(isPlayer2)
    {
        if (time % 10 < 5 && player->invisibility > 0)
            DrawTexturePro(sprite, textureBound, destRec, barycentre, core->rotation - 90.f, GREEN);
        else
            DrawTexturePro(sprite, textureBound, destRec, barycentre, core->rotation - 90.f, DARKGREEN);
    }
    else
    {
        if (time % 10 < 5 && player->invisibility > 0)
            DrawTexturePro(sprite, textureBound, destRec, barycentre, core->rotation - 90.f, SKYBLUE);
        else
            DrawTexturePro(sprite, textureBound, destRec, barycentre, core->rotation - 90.f, BLUE);
    }    
}

void displayEnemy(Texture2D sprite, Enemy* enemy)
{
    Core* core = &enemy->core;
    Rectangle textureBound = {0.f, 0.f, SPRITE_WIDTH / 4.f, SPRITE_HEIGHT / 2.f};
    switch (enemy->type)
    {
    case ET_FLOATING_MINE:
        textureBound.y = SPRITE_HEIGHT / 2;
        break;
    case ET_FIREBALL_MINE:
        textureBound.x = SPRITE_WIDTH * 0.75f;
        textureBound.y = SPRITE_HEIGHT / 2.f;
        break;
    case ET_MAGNETIC_MINE:
        textureBound.x = SPRITE_WIDTH / 4.f;
        textureBound.y = SPRITE_HEIGHT / 2.f;
        break;
    case ET_MAGNETIC_FIREBALL_MINE:
        textureBound.x = SPRITE_WIDTH / 2.f;
        textureBound.y = SPRITE_HEIGHT / 2.f;
        break;
    case ET_MINE_LAYER:
        textureBound.x = SPRITE_WIDTH / 2.f;
        break;
    }
    
    int sizeX = 16;
    int sizeY = 8;
    if(enemy->size == ES_BIG)
    {
        sizeX = 8;
        sizeY = 4;
    }
    else if(enemy->size == ES_MEDIUM)
    {
        sizeX = 12;
        sizeY = 6;
    }
    
    Rectangle destRec = {core->barycentre.x, core->barycentre.y, SPRITE_WIDTH/sizeX, SPRITE_HEIGHT/sizeY};

    Vector2 barycentre = (Vector2){destRec.width/2, destRec.height/2};

    changeScope(core);

    DrawTexturePro(sprite, textureBound, destRec, barycentre, -90.f, RED);
}

void displayFireball(Texture2D sprite, FireBall* fireball)
{

    Rectangle textureBound = (Rectangle){
        0.75f * SPRITE_WIDTH,
        0.f,
        SPRITE_WIDTH / 4.f,
        SPRITE_HEIGHT / 2.f
    };
    
    Rectangle destRec = {
        fireball->hitbox.centre.x,
        fireball->hitbox.centre.y,
        SPRITE_WIDTH / 12.f,
        SPRITE_HEIGHT / 6.f
    };

    changeScopeFireball(fireball);

    DrawTexturePro(sprite, textureBound, destRec, (Vector2){destRec.width / 2, destRec.height / 2}, 0.f, RED);
}

void displaySpawnPoint(Texture2D sprite, Vector2D center)
{
    Rectangle textureBound = (Rectangle){ SPRITE_WIDTH / 4.f, 0.f, SPRITE_WIDTH / 4.f, SPRITE_HEIGHT / 2.f };
    Rectangle destRec = { center.x, center.y, SPRITE_WIDTH / 24.f, SPRITE_HEIGHT / 12.f };
    DrawTexturePro(sprite, textureBound, destRec, (Vector2){destRec.width / 2, destRec.height / 2}, 0.f, LIGHTGRAY);
}

// Game screen

void displayMenu()
{
    DrawText("Do you want to play ?", SCREEN_WIDTH / 4 - 20, SCREEN_HEIGHT / 3, 35, WHITE);
    DrawText("Press \"F\" :      Solo mode", SCREEN_WIDTH / 3 - 40, SCREEN_HEIGHT / 3 + 50, 20, BLUE);
    DrawText("Press \"K\" :      Two players mode", SCREEN_WIDTH / 3 - 40, SCREEN_HEIGHT / 3 + 80, 20, GREEN);
    DrawText("Press \"Esc\" :    Exit", SCREEN_WIDTH / 3 - 40, SCREEN_HEIGHT / 3 + 110, 20, RED);
}

void displayGame(Texture2D sprite, Game* game)
{   
    for (int i = 0; i < game->enemyTotal; i++)
        if (!game->enemy[i].hasSpawned)
            displaySpawnPoint(sprite, game->enemy[i].core.barycentre);
    for(int i = 0; i < game->enemyAmount; i++)
        if (game->enemy[i].core.lifePoint > 0)
            displayEnemy(sprite, &game->enemy[i]);
    for (int i = 0; i < MAX_FIREBALL; i++)
        if(game->fireBall[i].isActive)
            displayFireball(sprite, &game->fireBall[i]);
    if(game->player1.core.lifePoint > 0)
        displayPlayer(sprite, &game->player1, false, game->timeElapsedFromStart);
    if(game->numberPlayer == NP_TWO && game->player2.core.lifePoint > 0)
        displayPlayer(sprite, &game->player2, true, game->timeElapsedFromStart);
}

void displayPauseMenu()
{
    DrawText("Pause", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 3, 35, WHITE);
    DrawText("Press \"SPACE\" :     Resume", SCREEN_WIDTH / 4 - 25, SCREEN_HEIGHT / 3 + 50, 20, BLUE);
    DrawText("Press \"Esc\" :         Back to main menu", SCREEN_WIDTH / 4 - 25, SCREEN_HEIGHT / 3 + 80, 20, RED);
}

void displayGameOver(int score)
{
    DrawText("GAME OVER", SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3, 35, WHITE);
    DrawText(TextFormat("Score : %06i", score), SCREEN_WIDTH / 3 + 35, SCREEN_HEIGHT / 3 + 50, 20, GREEN);
    DrawText("Press \"Esc\" :         Back to main menu", SCREEN_WIDTH / 4 - 25, SCREEN_HEIGHT / 3 + 100, 20, BLUE);
}

void displayVictoryScreen(int score)
{
    DrawText("YOU WIN !", SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 3, 35, GOLD);
    DrawText(TextFormat("Score : %06i", score), SCREEN_WIDTH / 3 + 35, SCREEN_HEIGHT / 3 + 50, 20, GREEN);
    DrawText("Press \"Esc\" :         Back to main menu", SCREEN_WIDTH / 4 - 25, SCREEN_HEIGHT / 3 + 100, 20, BLUE);
}

void displayLifePlayer(Game game)
{
    DrawRectangle(80, 80, 120, 50, BLUE);
    DrawText("Player 1", 110, 85, 15, BLACK);
    float xOffset = 0.f;
    for(int i = 0; i < game.player1.core.lifePoint; i++)
    {
        Vector2 A = (Vector2){110.f + xOffset, 105.f};
        Vector2 B = (Vector2){102.5f + xOffset, 125.f};
        Vector2 C = (Vector2){117.5f + xOffset, 125.f};
        DrawTriangle(A, B, C, BLACK);
        xOffset += 30.f;
    }
    if(game.numberPlayer == NP_TWO)
    {
        DrawRectangle(420, 80, 120, 50, DARKGREEN);
        DrawText("Player 2", 450, 85, 15, BLACK);
        float xOffset = 0.f;
        for(int i = 0; i < game.player2.core.lifePoint; i++)
        {
            Vector2 A = (Vector2){450.f + xOffset, 105.f};
            Vector2 B = (Vector2){442.5f + xOffset, 125.f};
            Vector2 C = (Vector2){457.5f + xOffset, 125.f};
            DrawTriangle(A, B, C, BLACK);
            xOffset += 30.f;
        }
    }
}

void displayGameInfo(Game* game)
{
    DrawText(TextFormat("Score : %06i", game->score), SCREEN_WIDTH / 2 - 85, 720, 20, YELLOW);
    // DrawText(TextFormat("Level : %d", game->level), SCREEN_WIDTH / 2 - 40, 10, 20, BLUE);
    DrawText(TextFormat("%d", game->level), SCREEN_WIDTH / 2 - 11, 35, 46, YELLOW);
}

void displayCommandPlayer()
{
    // Player 1
    DrawRectangle(80, 80, 120, 50, BLUE);
    DrawText("Player 1", 110, 85, 20,  BLACK);
    DrawText("Rotate : 'D' and 'G'", 80, 135, 20, LIGHTGRAY);
    DrawText("Move : 'R'", 80, 155, 20, LIGHTGRAY);
    DrawText("Shoot : 'F'", 80, 175, 20, LIGHTGRAY);
    DrawText("Teleport : 'E' and 'T'", 80, 195, 20, LIGHTGRAY);

    // Player 2
    DrawRectangle(420, 80, 120, 50, DARKGREEN);
    DrawText("Player 2", 450, 85, 20,  BLACK);
    DrawText("Rotate : 'J' and 'L'", 365, 135, 20, LIGHTGRAY);
    DrawText("Move : 'I'", 365, 155, 20, LIGHTGRAY);
    DrawText("Shoot : 'K'", 365, 175, 20, LIGHTGRAY);
    DrawText("Teleport : 'U' and 'O'", 365, 195, 20, LIGHTGRAY);
}

// Debug
void displayDebug(Core* core)
{
    DrawText(TextFormat("player.position.x : %0.2f player.rotation : %0.2f", core->barycentre.x, core->rotation), 0, 0, 10, WHITE);
    DrawText(TextFormat("player.position.y : %0.2f player.speed : %0.2f", core->barycentre.y, core->speed), 0, 15, 10, WHITE);
}

void displayHitbox(Game* game)
{
    // Player
    Core core;
    if (game->player1.core.lifePoint > 0)
    {
        core = game->player1.core;
        for (int i = 0; i < core.numberVector; i++)
            DrawLine(core.hitbox[i].x, core.hitbox[i].y, core.hitbox[(i + 1) % core.numberVector].x, core.hitbox[(i + 1) % core.numberVector].y, YELLOW);
        DrawLine(core.barycentre.x, core.barycentre.y, core.barycentre.x - 40.f * cosf(core.lastThrustRotation * MY_PI / 180.f), core.barycentre.y - 40.f * sinf(core.lastThrustRotation * MY_PI / 180.f), SKYBLUE);
    }
    if(game->numberPlayer == NP_TWO && game->player2.core.lifePoint > 0)
    {
        core = game->player2.core;
        for (int i = 0; i < core.numberVector; i++)
            DrawLine(core.hitbox[i].x, core.hitbox[i].y, core.hitbox[(i + 1) % core.numberVector].x, core.hitbox[(i + 1) % core.numberVector].y, YELLOW);
        DrawLine(core.barycentre.x, core.barycentre.y, core.barycentre.x - 40.f * cosf(core.lastThrustRotation * MY_PI / 180.f), core.barycentre.y - 40.f * sinf(core.lastThrustRotation * MY_PI / 180.f), SKYBLUE);
    }

    // Mine
    for(int i = 0; i < game->enemyAmount; i++)
    {
        core = game->enemy[i].core;
        if (core.lifePoint > 0)
            for (int j = 0; j < core.convexShapeAmount; j++)
            {
                ConvexShape convShape = core.convexShapes[j];
                for (int k = 0; k < convShape.vertexAmount; k++)
                    DrawLine(convShape.vertices[k].x, convShape.vertices[k].y, convShape.vertices[(k + 1) % convShape.vertexAmount].x, convShape.vertices[(k + 1) % convShape.vertexAmount].y, YELLOW);
                if (game->enemy[i].type == ET_MAGNETIC_MINE || game->enemy[i].type == ET_MAGNETIC_FIREBALL_MINE)
                {
                    if (game->numberPlayer == NP_ONE)
                    {
                        DrawLine(core.barycentre.x, core.barycentre.y, game->player1.core.barycentre.x, game->player1.core.barycentre.y, SKYBLUE);
                    }
                    else
                    {
                        Core closestPlayer = aimClosestPlayer(core, game->player1.core, game->player2.core);
                        DrawLine(core.barycentre.x, core.barycentre.y, closestPlayer.barycentre.x, closestPlayer.barycentre.y, SKYBLUE);
                    }
                }
                else
                    DrawLine(core.barycentre.x, core.barycentre.y, core.barycentre.x + 40.f * cosf(core.lastThrustRotation * MY_PI / 180.f), core.barycentre.y + 40.f * sinf(core.lastThrustRotation * MY_PI / 180.f), SKYBLUE);
            }
    }

    // Fireball
    for(int i = 0; i < game->numberFireball; i++)
        if(game->fireBall[i].isActive)
        {
            DrawCircle(game->fireBall[i].hitbox.centre.x, game->fireBall[i].hitbox.centre.y, game->fireBall[i].hitbox.radius, YELLOW);
            DrawLine(game->fireBall[i].hitbox.centre.x, game->fireBall[i].hitbox.centre.y, game->fireBall[i].hitbox.centre.x + game->fireBall[i].moveVector.x * 2.5f, game->fireBall[i].hitbox.centre.y + game->fireBall[i].moveVector.y * 2.5f, SKYBLUE);
        }
}