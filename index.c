#include <stdio.h>
#include "raylib.h"

typedef enum GameScreen { LOGO, TITLE, GAMEPLAY, ERROR } GameScreen;


int main(void) {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Jetpack Joyride - INF5102");

    GameScreen currentScreen = LOGO;

    int framesCounter = 0;

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {
        // Mechanics
        switch(currentScreen) {
            case LOGO:
                framesCounter++;
                if (framesCounter > 120) {
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

            switch(currentScreen)
            {
                case LOGO:
                {
                    // TODO: Draw LOGO screen here!
                    DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
                    DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);

                } break;
                case TITLE:
                {
                    // TODO: Draw TITLE screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
                    DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
                    DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);

                } break;
                case GAMEPLAY:
                {
                    // TODO: Draw GAMEPLAY screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, PURPLE);
                    DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
                    DrawText("PRESS ENTER or TAP to JUMP to TITLE SCREEN", 130, 220, 20, MAROON);

                } break;
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
