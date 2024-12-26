#include <stdio.h>
#include "raylib.h"
#include "./libs/map.c"
#include "./libs/player.c"

#define SCREEN_WIDTH SECTION_WIDTH * CELL_SIZE
#define SCREEN_HEIGHT MAP_HEIGHT * CELL_SIZE
#define CELL_SIZE 64
#define GRAVITY 0.6


typedef enum GameScreen { HOME, TITLE, GAMEPLAY, ERROR, GAMEOVER } GameScreen;


int main() {
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Jetpack Joyride - INF5102");
    InitAudioDevice();
    SetTargetFPS(60);
    int isGameRunning = 1;

    Sounds sounds = {
        LoadSound("resources/sounds/button1.wav"),
        LoadSound("resources/sounds/coin.mp3"),
        LoadSound("resources/sounds/hit.mp3")
    };

    Player player = {0};
    initializePlayer(&player, "resources/player.png", 6);

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

    int RECTANGLE_WIDTH = 500;
    int RECTANGLE_HEIGHT = 100;
    Rectangle restartRectangle = { (SCREEN_WIDTH - RECTANGLE_WIDTH) / 2 , 200 , RECTANGLE_WIDTH, RECTANGLE_HEIGHT};
    Rectangle saveRectangle = { (SCREEN_WIDTH - RECTANGLE_WIDTH) / 2 , 325, RECTANGLE_WIDTH, RECTANGLE_HEIGHT};
    Rectangle menuRectangle = { (SCREEN_WIDTH - RECTANGLE_WIDTH) / 2 , 450 , RECTANGLE_WIDTH, RECTANGLE_HEIGHT};
    Rectangle exitRectangle = { (SCREEN_WIDTH - RECTANGLE_WIDTH) / 2 , 575, RECTANGLE_WIDTH, RECTANGLE_HEIGHT};


    //--------------------------------------------------------------------------------------

    while (!WindowShouldClose() && isGameRunning) {
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

                movePlayer(&player, GRAVITY);

                if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_SPACE)) {
                    movePlayer(&player, player.jumpPower);
                }

                checksCollision(&player, loadedMap[0], &sounds);

                break;

            case GAMEOVER:
                Vector2 mousePosition = GetMousePosition();

                if(CheckCollisionPointRec(mousePosition, restartRectangle)) {

                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        PlaySound(sounds.button);
                        currentScreen = GAMEPLAY;
                    }
                }

                if(CheckCollisionPointRec(mousePosition, saveRectangle)) {
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        PlaySound(sounds.button);
                    }
                }

                if(CheckCollisionPointRec(mousePosition, menuRectangle)) {
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        PlaySound(sounds.button);
                        currentScreen = HOME;
                    }
                }

                if(CheckCollisionPointRec(mousePosition, exitRectangle)) {
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        PlaySound(sounds.button);
                        isGameRunning = 0;
                    }
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
                if (isMapRead) {
                    DrawText(TextFormat("LIVES: %d", player.lives), 20, 110, 35, RED );
                    DrawText(TextFormat("COINS: %d", player.coins), 20, 80, 35, GOLD);
                    DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
                    DrawText("PRESS ENTER or TAP to JUMP to TITLE SCREEN", 130, 220, 20, MAROON);
                    drawMap(loadedMap, &mapTextures);
                    drawPlayer(&player);
                }


                break;

            case GAMEOVER:
                DrawText("YOU DIED",  (SCREEN_WIDTH - MeasureText("YOU DIED",100)) / 2 , 80, 100, RED);



                DrawRectangleRec(restartRectangle, BLACK);
                DrawText("Restart Game", (SCREEN_WIDTH - MeasureText("Restart Game",40)) / 2, 228, 40, WHITE);

                DrawRectangleRec(saveRectangle, BLACK);
                DrawText("Save Game", (SCREEN_WIDTH - MeasureText("Save Game",40)) / 2, 353, 40, WHITE);

                DrawRectangleRec(menuRectangle, BLACK);
                DrawText("Back to menu", (SCREEN_WIDTH - MeasureText("Back to menu",40)) / 2, 478, 40, WHITE);

                DrawRectangleRec(exitRectangle, BLACK);
                DrawText("Exit Game", (SCREEN_WIDTH - MeasureText("Exit Game",40)) / 2, 603, 40, WHITE);

                break;



                default: break;
            }

        EndDrawing();
    }
    //--------------------------------------------------------------------------------------


    // De-Initialization
    // TODO: Unload all loaded data (textures, fonts, audio) here!
    UnloadTexture(mapTextures.coinTexture);
    UnloadTexture(mapTextures.spikeTexture);
    UnloadTexture(mapTextures.wallTexture);
    UnloadTexture(player.texture);
    UnloadSound(sounds.button);
    UnloadSound(sounds.coin);

    CloseWindow();

    return 0;
}
