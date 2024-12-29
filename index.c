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

    Sounds sounds = {
        LoadSound("resources/sounds/button1.wav"),
        LoadSound("resources/sounds/coin.mp3"),
        LoadSound("resources/sounds/hit.mp3")
    };

    Player player;
    initializePlayer(&player, 6, "resources/player.png");

    int isMapRead = 0;
    float levelSpeed = 0.2;
    MapSection loadedMap[2] = {0};
    MapSection mapSections[TOTAL_SECTIONS] = {0};
    MapTextures mapTextures = {
        LoadTexture("resources/coin.png"),
        LoadTexture("resources/spike.png"),
        LoadTexture("resources/wall.png")
    };


    //--------------------------------------------------------------------------------------

    while (!WindowShouldClose() && isGameRunning) {
        framesCounter++;

        // Mechanics
        switch(currentScreen) {
            case GAMEPLAY:
                if (!isMapRead) {
                    isMapRead = readMapFile(1, mapSections);

                    loadMapInto(loadedMap[0], mapSections);
                    loadMapInto(loadedMap[1], mapSections);
                    break;
                }

                if(player.lives <= 0 || IsKeyPressed(KEY_G)) {
                    currentScreen = GAMEOVER;
                }

                moveMap(levelSpeed, loadedMap);

                // Checks if a new sections needs to be loaded
                if (framesCounter % (int)(1 / levelSpeed * SECTION_WIDTH) == 0) {
                    loadMapInto(loadedMap[1], mapSections);
                }

                if (player.isInvulnerable && GetTime() > player.invulnerableUntill) {
                    player.isInvulnerable = 0;
                }

                if (!player.isTouchingTheGround) {
                    movePlayer(&player, GRAVITY);
                }

                if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_SPACE)) {
                    player.isTouchingTheGround = 0;
                    movePlayer(&player, player.jumpPower);
                }

                checksCollision(&player, loadedMap[0], &sounds);

                break;

            default: break;
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch(currentScreen) {
            case HOME:
                drawHomeScreen(&currentScreen);
                break;

            case TITLE:
                drawTitleScreen(&currentScreen);
                break;


            case GAMEPLAY:
                if (isMapRead) {
                    DrawText(TextFormat("LIVES: %d", player.lives), 20, 110, 35, RED );
                    DrawText(TextFormat("COINS: %d", player.coins), 20, 80, 35, GOLD);
                    drawMap(loadedMap, &mapTextures);
                    drawPlayer(&player);
                }

                break;

            case GAMEOVER:
                drawGameOverScreen(&currentScreen, &isGameRunning, &player, &sounds.button);
                break;

            case SAVEGAME:
                drawSaveGameScreen(&currentScreen, &player, &sounds.button);
                break;

            default: break;
        }

        EndDrawing();
    }
    //--------------------------------------------------------------------------------------


    // De-Initialization
    UnloadTexture(mapTextures.coinTexture);
    UnloadTexture(mapTextures.spikeTexture);
    UnloadTexture(mapTextures.wallTexture);
    UnloadTexture(player.texture);
    UnloadSound(sounds.button);
    UnloadSound(sounds.coin);

    CloseWindow();

    return 0;
}
