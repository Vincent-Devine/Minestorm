#include "manager.h"
#include "gameInit.h"
#include "move.h"
#include "display.h"

// Game management
int menuManagement(GameScreen* currentScreen, Texture2D backGround, Texture2D foreGround, Game* game)
{
    game->score = 0;
    if(IsKeyPressed(KEY_F))
    {
        game->timeElapsedFromStart = 0;
        *currentScreen = GS_GAME;
        game->numberPlayer = NP_ONE;
        gameInit(game);
    }
    if(IsKeyPressed(KEY_K))
    {
        game->timeElapsedFromStart = 0;
        *currentScreen = GS_GAME;
        game->numberPlayer = NP_TWO;
        gameInit(game);
    }
    if(IsKeyPressed(KEY_ESCAPE))
        return 1;

    // Display
    DrawTexture(backGround, SCREEN_WIDTH/2 - backGround.width/2, SCREEN_HEIGHT/2 - backGround.height/2, WHITE);
    DrawTexture(foreGround, SCREEN_WIDTH/2 - foreGround.width/2, SCREEN_HEIGHT/2 - foreGround.height/2, WHITE);
    displayCommandPlayer();
    displayMenu();
    return 0;
}

void gameManagement(GameScreen* currentScreen, Texture2D backGround, Texture2D foreGround, Texture2D sprite, Game* game)
{
    int enemiesDestroyed = 0;
    for (int i = 0; i < game->enemyTotal; i++)
    {
        if (game->enemy[i].hasSpawned == true && game->enemy[i].core.lifePoint < 1)
            enemiesDestroyed++;
    } 
    if (enemiesDestroyed == game->enemyTotal)
        startNewLevel(game);
    if (game->timeElapsedFromStart == 180)
    {
        for (int i = 0; i < game->enemyAmount; i++)
            initEnemy(&game->enemy[i]);
    }
    if(IsKeyPressed(KEY_SPACE))
        *currentScreen = GS_PAUSE_MENU;

    // Display
    DrawTexture(backGround, SCREEN_WIDTH/2 - backGround.width/2, SCREEN_HEIGHT/2 - backGround.height/2, WHITE);
    displayGame(sprite, game);
    DrawTexture(foreGround, SCREEN_WIDTH/2 - foreGround.width/2, SCREEN_HEIGHT/2 - foreGround.height/2, WHITE);
    displayLifePlayer(*game);
    displayGameInfo(game);

    if (enemiesDestroyed == game->enemyTotal && game->level == LEVEL_TOTAL)
    {
        *currentScreen = GS_VICTORY;
        return;
    }   

    movementEnemy(game);

    if(IsKeyDown(KEY_C))
        displayHitbox(game);

    // Player 1
    if(game->player1.core.lifePoint > 0)
    {
        // Shoot
        if(IsKeyPressed(KEY_F))
            shoot(game, &game->player1.core);
        // Rotation
        if(IsKeyDown(KEY_D))
        {
            game->player1.core.rotation-=4.f;
            rotationHitbox(&game->player1.core, -3.9545f);
        }
        if(IsKeyDown(KEY_G))
        {
            game->player1.core.rotation+=4.f;
            rotationHitbox(&game->player1.core, +3.9545f);
        }
        //Movement
        if(IsKeyDown(KEY_R))
        {
            game->player1.core.speed -= 0.1f;
            if (game->player1.core.speed < -6.f)
                game->player1.core.speed = -6.f;
            else if (game->player1.core.speed > 0.f)
                game->player1.core.speed = 0.f;
            movementCalculation2D(&game->player1.core, true);
        }
        computeInertiaForEveryone(&game->player1.core);
        // TP
        if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_T))
            giveRandomPosition(&game->player1.core);
    }

    // Player 2
    if(game->numberPlayer == NP_TWO && game->player2.core.lifePoint > 0)
    {
        // Shoot
        if(IsKeyPressed(KEY_K))
            shoot(game, &game->player2.core);
        // Rotation
        if(IsKeyDown(KEY_J))
        {
            game->player2.core.rotation-=4.f;
            rotationHitbox(&game->player2.core, -3.9545f);
        }
        if(IsKeyDown(KEY_L))
        {
            game->player2.core.rotation+=4.f;
            rotationHitbox(&game->player2.core, +3.9545f);
        }
        //Movement
        if(IsKeyDown(KEY_I))
        {
            game->player2.core.speed -= 0.1f;
            if (game->player2.core.speed < -6.f)
                game->player2.core.speed = -6.f;
            else if (game->player2.core.speed > 0.f)
                game->player2.core.speed = 0.f;
            movementCalculation2D(&game->player2.core, true);   
        }
        computeInertiaForEveryone(&game->player2.core);
        // TP
        if (IsKeyPressed(KEY_U) || IsKeyPressed(KEY_O))
            giveRandomPosition(&game->player2.core);
    }
    
    
    // Fireball
    for (int i = 0; i < game->numberFireball; i++)
        movementFireball(&game->fireBall[i]);
    // Colliding
    checkCollidingPlayer(game);
    checkCollidingFireball(game);
    
    if(game->numberPlayer == NP_TWO)
    {
        if(game->player1.core.lifePoint <= 0 && game->player2.core.lifePoint <= 0)
            *currentScreen = GS_GAME_OVER;
    }
    else if(game->numberPlayer == NP_ONE && game->player1.core.lifePoint <= 0)
        *currentScreen = GS_GAME_OVER;
}

void pauseMenuManagement(GameScreen* currentScreen, Texture2D backGround, Texture2D foreGround, Game* game)
{
    if(IsKeyPressed(KEY_SPACE))
        *currentScreen = GS_GAME;
    if(IsKeyPressed(KEY_ESCAPE))
    {
        game->level = 0;
        *currentScreen = GS_MENU;
    }
    // Display
    DrawTexture(backGround, SCREEN_WIDTH - backGround.width, SCREEN_HEIGHT - backGround.height, WHITE);
    DrawTexture(foreGround, SCREEN_WIDTH - foreGround.width, SCREEN_HEIGHT - foreGround.height, WHITE);
    displayCommandPlayer();
    displayPauseMenu();
}

void gameOverManagement(GameScreen* currentScreen, Texture2D backGround, Texture2D foreGround, Game* game)
{
    if(IsKeyPressed(KEY_ESCAPE))
    {
        game->level = 0;
        *currentScreen = GS_MENU;
    }
    // Display
    DrawTexture(backGround, SCREEN_WIDTH - backGround.width, SCREEN_HEIGHT - backGround.height, WHITE);
    DrawTexture(foreGround, SCREEN_WIDTH - foreGround.width, SCREEN_HEIGHT - foreGround.height, WHITE);
    displayCommandPlayer();
    displayGameOver(game->score);
}

void victoryManagement(GameScreen* currentScreen, Texture2D backGround, Texture2D foreGround, Game* game)
{
    if(IsKeyPressed(KEY_ESCAPE))
    {
        game->level = 0; 
        *currentScreen = GS_MENU;
    }
    // Display
    DrawTexture(backGround, SCREEN_WIDTH - backGround.width, SCREEN_HEIGHT - backGround.height, WHITE);
    DrawTexture(foreGround, SCREEN_WIDTH - foreGround.width, SCREEN_HEIGHT - foreGround.height, WHITE);
    displayCommandPlayer();
    displayVictoryScreen(game->score);
}

int screenChoice(GameScreen* currentScreen, Texture2D backGround, Texture2D foreGround, Texture2D sprite, Game* game)
{
    switch (*currentScreen)
    {
        case GS_MENU:
            if(menuManagement(currentScreen, backGround, foreGround, game) == 1)
                return 1;
            break;
        case GS_GAME:
            gameManagement(currentScreen, backGround, foreGround, sprite, game);
            break;
        case GS_PAUSE_MENU:
            pauseMenuManagement(currentScreen, backGround, foreGround, game);
            break;
        case GS_GAME_OVER:
            gameOverManagement(currentScreen, backGround, foreGround, game);
            break;
        case GS_VICTORY:
            victoryManagement(currentScreen, backGround, foreGround, game);
            break;
    }
    return 0;
}