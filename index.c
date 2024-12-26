#include <stdio.h>
#include "raylib.h"
#include "./libs/map.c"
#include "./libs/player.c"

#define SCREEN_WIDTH SECTION_WIDTH * CELL_SIZE
#define SCREEN_HEIGHT MAP_HEIGHT * CELL_SIZE
#define CELL_SIZE 64
#define GRAVITY 0.8

typedef enum GameScreen { HOME, TITLE, GAMEPLAY, ERROR } GameScreen;


int main() {
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Jetpack Joyride - INF5102");
    SetTargetFPS(60);
    int framesCounter = 0;

    GameScreen currentScreen = HOME;

    int isMapRead = 0;
    float levelSpeed = 0.2;
    MapSection loadedMap[2] = {0};
    MapSection mapSections[TOTAL_SECTIONS] = {0};
    MapTextures mapTextures = {
        LoadTexture("resources/coin.png"),
        LoadTexture("resources/spike.png"),
        LoadTexture("resources/wall.png")
    };

    Player player = {0};
    initializePlayer(&player, "./resources/player.png", 6);

    srand(time(NULL));


    //--------------------------------------------------------------------------------------

    while (!WindowShouldClose()) {
        framesCounter++;

        // Mechanics
        switch(currentScreen) {
            case HOME:
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                    currentScreen = TITLE;
                }

                break;

            case TITLE:
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                    currentScreen = GAMEPLAY;
                }

                break;

            case GAMEPLAY:
                if (!isMapRead) {
                    isMapRead = readMapFile(1, mapSections);

                    loadMapInto(loadedMap[0], mapSections);
                    loadMapInto(loadedMap[1], mapSections);
                }

                moveMap(levelSpeed, loadedMap);

                // Checks if a new sections needs to be loaded
                if (framesCounter % (int)(1 / levelSpeed * SECTION_WIDTH) == 0) {
                    loadMapInto(loadedMap[1], mapSections);
                }

                movePlayer(&player, GRAVITY);

                if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_SPACE)) {
                    movePlayer(&player, player.jumpPower);
                }

                break;


            default: break;
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch(currentScreen) {
            case HOME:
                // TODO: Draw LOGO screen here!
                DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
                break;

            case TITLE:
                // TODO: Draw TITLE screen here!
                DrawRectangle(0, 0,SCREEN_WIDTH, SCREEN_HEIGHT, GREEN);
                DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
                DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
                break;

            case GAMEPLAY:
                // TODO: Draw GAMEPLAY screen here!
                drawMap(loadedMap, &mapTextures);
                drawPlayer(&player);

                break;

            default: break;
        }

        EndDrawing();
    }
    //--------------------------------------------------------------------------------------


    // De-Initialization
    // TODO: Unload all loaded data (textures, fonts, audio) here!
    UnloadTexture(mapTextures.spikeTexture);
    UnloadTexture(mapTextures.coinTexture);
    UnloadTexture(player.texture);

    CloseWindow();

    return 0;
}
