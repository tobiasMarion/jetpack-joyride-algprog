#include <stdio.h>
#include "raylib.h"
#include "./libs/constants.h"
#include "./libs/map.c"
#include "./libs/player.c"
#include "./libs/ui.c"

int main() {
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Jetpack Joyride - INF5102");
    InitAudioDevice();
    SetTargetFPS(60);
    int framesCounter = 0;
    int isGameRunning = 1;
    GameScreen currentScreen = HOME;
    char errorMessage[ERROR_MESSAGE_LENGTH] = {0};

    Sounds sounds = {
        LoadSound("resources/sounds/button1.wav"),
        LoadSound("resources/sounds/coin.mp3"),
        LoadSound("resources/sounds/hit.mp3")
    };

    Player player;
    Level level = {0};
    double upLevelAt = 0;
    int currentLevel = 1;
    int isLevelLoaded = 0;

    MapSection loadedMap[2] = {0};

    //--------------------------------------------------------------------------------------

    while (!WindowShouldClose() && isGameRunning) {
        framesCounter++;

        // Mechanics
        if (currentScreen == GAMEPLAY) {
            if (!isLevelLoaded) {
                isLevelLoaded = loadLevel(currentLevel, &level, errorMessage);

                if (!isLevelLoaded) {
                    currentScreen = ERROR;
                }

                loadEmptyMap(loadedMap[0]);
                loadMapRandomly(loadedMap[1], level.mapSections);
            }

            if(player.lives <= 0 || IsKeyPressed(KEY_G)) {
                currentScreen = GAMEOVER;
            }

            player.distance += moveMap(level.speed, loadedMap);

            // Checks if a new sections needs to be loaded
            if (framesCounter % (int)(1 / level.speed * SECTION_WIDTH) == 0) {
                loadMapRandomly(loadedMap[1], level.mapSections);
            }

            if (player.isInvulnerable && GetTime() > player.invulnerableUntill) {
                player.isInvulnerable = 0;
            }

            if (!player.isTouchingTheGround) {
                movePlayer(&player, level.gravity);
            }

            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_SPACE)) {
                player.isTouchingTheGround = 0;
                movePlayer(&player, player.jumpPower);
            }

            checksCollision(&player, loadedMap[0], &sounds);

            if (player.distance > level.requiredDistanceToNextLevel && isLevelLoaded) {
                currentLevel++;
                upLevelAt = GetTime();
                isLevelLoaded = 0;
                currentScreen = NEXT_LEVEL;
                unloadMapTextures(&level.mapTextures);
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch(currentScreen) {
            case HOME:
                drawHomeScreen(&isGameRunning, &currentScreen, &player, &sounds.button);
                break;

            case GAMEPLAY:
                if (isLevelLoaded) {
                    DrawText(TextFormat("LIVES: %d", player.lives), 20, 110, 35, RED);
                    DrawText(TextFormat("COINS: %d", player.coins), 20, 80, 35, GOLD);
                    drawMap(loadedMap, &level.mapTextures);
                    drawPlayer(&player);
                }

                break;

            case NEXT_LEVEL:
                drawNextLevelScreen(currentLevel, upLevelAt, &currentScreen);
                break;

            case GAMEOVER:
                drawGameOverScreen(&isGameRunning, &currentScreen, &player, &currentLevel, &isLevelLoaded, &sounds.button);
                break;

            case SAVEGAME:
                drawSaveGameScreen(&currentScreen, &player, &sounds.button);
                break;

            case ERROR:
                drawErrorScreen(&isGameRunning, errorMessage);

            default: break;
        }

        EndDrawing();
    }
    //--------------------------------------------------------------------------------------


    // De-Initialization
    unloadMapTextures(&level.mapTextures);
    UnloadTexture(player.texture);
    UnloadSound(sounds.button);
    UnloadSound(sounds.coin);
    UnloadSound(sounds.hit);

    CloseWindow();

    return 0;
}
