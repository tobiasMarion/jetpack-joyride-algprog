#include <stdio.h>
#include "raylib.h"
#include "./libs/map.c"

#define CELL_SIZE 64
#define SCREEN_WIDTH SECTION_WIDTH * CELL_SIZE
#define SCREEN_HEIGHT MAP_HEIGHT * CELL_SIZE

typedef enum GameScreen { HOME, TITLE, GAMEPLAY, ERROR } GameScreen;


int main(void) {
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Jetpack Joyride - INF5102");
    SetTargetFPS(60);

    int framesCounter = 0;
    GameScreen currentScreen = HOME;
    char loadedMap[MAP_HEIGHT][SECTION_WIDTH * 2] = {0};
    char mapSections[TOTAL_SECTIONS][MAP_HEIGHT][SECTION_WIDTH] = {0};
    int isMapLoaded = 0;


    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {
        // Mechanics
        switch(currentScreen) {
            case HOME:
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                    if (!isMapLoaded) {
                        isMapLoaded = readMapFile(1, mapSections);
                        printMap(mapSections);
                    }

                    currentScreen = TITLE;
                }

                break;

            case TITLE:
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                    currentScreen = GAMEPLAY;
                }

                break;

            case GAMEPLAY:
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                    currentScreen = TITLE;
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
                    DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);
                    break;

                case TITLE:
                    // TODO: Draw TITLE screen here!
                    DrawRectangle(0, 0,SCREEN_WIDTH, SCREEN_HEIGHT, GREEN);
                    DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
                    DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
                    break;

                case GAMEPLAY:
                    // TODO: Draw GAMEPLAY screen here!
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, PURPLE);
                    DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
                    DrawText("PRESS ENTER or TAP to JUMP to TITLE SCREEN", 130, 220, 20, MAROON);
                    break;

                default: break;
            }

        EndDrawing();
    }
    //--------------------------------------------------------------------------------------


    // De-Initialization
    // TODO: Unload all loaded data (textures, fonts, audio) here!

    CloseWindow();

    return 0;
}
