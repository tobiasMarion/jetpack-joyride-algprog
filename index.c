#include <stdio.h>
#include "raylib.h"
#include "./libs/map.c"

#define SCREEN_WIDTH SECTION_WIDTH * CELL_SIZE
#define SCREEN_HEIGHT MAP_HEIGHT * CELL_SIZE

typedef enum GameScreen { HOME, TITLE, GAMEPLAY, ERROR } GameScreen;


int main() {
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Jetpack Joyride - INF5102");
    SetTargetFPS(60);
    srand(time(NULL));

    Texture2D wallTexture = LoadTexture("resources/wall.png");
    Texture2D coinTexture = LoadTexture("resources/coin.png");
    Texture2D playerTexture = LoadTexture("resources/player.png");

    Player player = {0};
    initializePlayer(&player, (Vector2){6 * CELL_SIZE, SCREEN_HEIGHT / 2}, playerTexture);

    int framesCounter = 0;
    GameScreen currentScreen = HOME;
    MapSection loadedMap[2] = {0};
    MapSection mapSections[TOTAL_SECTIONS] = {0};
    int isMapRead = 0;
    float levelSpeed = 0.2;


    //--------------------------------------------------------------------------------------

    // Main game loop
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
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                    currentScreen = TITLE;
                }

                if (!isMapRead) {
                    isMapRead = readMapFile(1, mapSections);

                    loadMapInto(loadedMap[0], mapSections);
                    loadMapInto(loadedMap[1], mapSections);
                }

                moveMap(levelSpeed, loadedMap);


                if (framesCounter % (int)(1 / levelSpeed * SECTION_WIDTH) == 0) {
                    loadMapInto(loadedMap[1], mapSections);
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
                    DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
                    DrawText("PRESS ENTER or TAP to JUMP to TITLE SCREEN", 130, 220, 20, MAROON);
                    movePlayer(&player);               // Atualiza o movimento vertical do jogador
                    drawMap(loadedMap, wallTexture, coinTexture); // Desenha o mapa movendo-se da direita para a esquerda
                    drawPlayer(&player);               // Desenha o jogador

                    break;

                default: break;
            }

        EndDrawing();
    }
    //--------------------------------------------------------------------------------------


    // De-Initialization
    // TODO: Unload all loaded data (textures, fonts, audio) here!
    
    UnloadTexture(wallTexture);
    UnloadTexture(coinTexture);
    UnloadTexture(playerTexture);

    CloseWindow();

    return 0;
}
