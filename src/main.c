#include <stdlib.h>
#include <raylib.h>
#include "game.h"
#include "manager.h"

int main()
{
    loadSeed();
    Texture2D backGround, foreGround, sprite;
    Game* game = calloc(1, sizeof(Game));
    GameScreen currentScreen = GS_MENU;
    game->timeElapsedFromStart = 0;
    game->level = 0;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minestorm");
    SetExitKey(KEY_X);
    SetTargetFPS(60);
    backGround = LoadTexture("asset/minestorm_background.png");
    foreGround = LoadTexture("asset/minestorm_foreground.png");
    sprite = LoadTexture("asset/minestorm_sprite_atlas_mine_storm.png");
    // Main game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if(screenChoice(&currentScreen, backGround, foreGround, sprite, game) == 1)
        {
            saveSeed();
            CloseWindow();
            free(game);
            return 0;
        }
        EndDrawing();
        if(currentScreen == GS_GAME)
            game->timeElapsedFromStart++;  
    }
    saveSeed();
    UnloadTexture(backGround);
    UnloadTexture(foreGround);
    UnloadTexture(sprite);
    CloseWindow();
    free(game);
    return 0;
}