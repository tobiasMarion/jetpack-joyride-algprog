#include <stdio.h>
#include "raylib.h"
#include "./libs/map.c"
#include "./libs/player.c"

#define SCREEN_WIDTH SECTION_WIDTH * CELL_SIZE
#define SCREEN_HEIGHT MAP_HEIGHT * CELL_SIZE
#define CELL_SIZE 64
#define GRAVITY 0.6
#define LINE_THICKNESS 5
#define RECTANGLE_WIDTH 500
#define RECTANGLE_HEIGHT 100
#define RECTANGLE_POSITION_X_CENTER (SCREEN_WIDTH - RECTANGLE_WIDTH ) / 2


typedef enum GameScreen { HOME, TITLE, GAMEPLAY, ERROR, GAMEOVER, ENDGAME, SAVEGAME} GameScreen;
//Implementar a tela ENDGAME(QUANDO O JOGADOR PASSAR TODAS AS FASES) e a tela SAVEGAME onde o jogador pode salvar
//o progresso do personagem

int main() {
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Jetpack Joyride - INF5102");
    InitAudioDevice();
    SetTargetFPS(60);
    int isGameRunning = 1;
    int letterCount = 0;
    Vector2 mousePosition = GetMousePosition();

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


    Rectangle buttons[] = {
        { RECTANGLE_POSITION_X_CENTER + LINE_THICKNESS, 200 + LINE_THICKNESS, RECTANGLE_WIDTH - ( 2 * LINE_THICKNESS) , RECTANGLE_HEIGHT - ( 2 * LINE_THICKNESS)}, //Restart Button 0
        { RECTANGLE_POSITION_X_CENTER + LINE_THICKNESS, 325 + LINE_THICKNESS, RECTANGLE_WIDTH - ( 2 * LINE_THICKNESS) , RECTANGLE_HEIGHT - ( 2 * LINE_THICKNESS)}, //Save Button 1
        { RECTANGLE_POSITION_X_CENTER + LINE_THICKNESS, 450 + LINE_THICKNESS, RECTANGLE_WIDTH - ( 2 * LINE_THICKNESS) , RECTANGLE_HEIGHT - ( 2 * LINE_THICKNESS)}, //Menu Button 2
        { RECTANGLE_POSITION_X_CENTER + LINE_THICKNESS, 575 + LINE_THICKNESS, RECTANGLE_WIDTH - ( 2 * LINE_THICKNESS) , RECTANGLE_HEIGHT - ( 2 * LINE_THICKNESS)}, //Exit Button 3
        { RECTANGLE_POSITION_X_CENTER + LINE_THICKNESS - (RECTANGLE_WIDTH / 2) - 10, 575 + LINE_THICKNESS, RECTANGLE_WIDTH - ( 2 * LINE_THICKNESS) , RECTANGLE_HEIGHT - ( 2 * LINE_THICKNESS)}, //SavePlayer Button 4
        { RECTANGLE_POSITION_X_CENTER + LINE_THICKNESS + (RECTANGLE_WIDTH / 2) + 10, 575 + LINE_THICKNESS, RECTANGLE_WIDTH - ( 2 * LINE_THICKNESS) , RECTANGLE_HEIGHT - ( 2 * LINE_THICKNESS)}  //Return Button 5

    };

    Rectangle outline[] = {
        { (SCREEN_WIDTH - RECTANGLE_WIDTH) / 2, 200, RECTANGLE_WIDTH, RECTANGLE_HEIGHT },
        { (SCREEN_WIDTH - RECTANGLE_WIDTH) / 2, 325, RECTANGLE_WIDTH, RECTANGLE_HEIGHT },
        { (SCREEN_WIDTH - RECTANGLE_WIDTH) / 2, 450, RECTANGLE_WIDTH, RECTANGLE_HEIGHT },
        { (SCREEN_WIDTH - RECTANGLE_WIDTH) / 2, 575, RECTANGLE_WIDTH, RECTANGLE_HEIGHT },
        { RECTANGLE_POSITION_X_CENTER - (RECTANGLE_WIDTH / 2) - 10, 575, RECTANGLE_WIDTH, RECTANGLE_HEIGHT},
        { RECTANGLE_POSITION_X_CENTER + (RECTANGLE_WIDTH / 2) + 10, 575, RECTANGLE_WIDTH, RECTANGLE_HEIGHT}

    };

    Rectangle textBox = {(SCREEN_WIDTH - 700 ) / 2 , 400, 700, 85};
    Rectangle outlineTextBox = {((SCREEN_WIDTH - 700 ) / 2) - 5  , 400 - 5, 710, 95};


    //--------------------------------------------------------------------------------------

    while (!WindowShouldClose() && isGameRunning) {
        framesCounter++;
        Vector2 mousePosition = GetMousePosition();

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

                //Restart button
                if(CheckCollisionPointRec(mousePosition, buttons[0])) {
                    DrawRectangleLinesEx(outline[0], LINE_THICKNESS, RED);
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        PlaySound(sounds.button);
                        initializePlayer(&player, "resources/player.png", 6);
                        currentScreen = GAMEPLAY;
                    }
                }

                //SaveGame button
                if(CheckCollisionPointRec(mousePosition, buttons[1])) {
                    DrawRectangleLinesEx(outline[1], LINE_THICKNESS, RED);
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        PlaySound(sounds.button);
                        currentScreen = SAVEGAME;
                    }
                }

                //Menu button
                if(CheckCollisionPointRec(mousePosition, buttons[2])) {
                    DrawRectangleLinesEx(outline[2], LINE_THICKNESS, RED);
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        PlaySound(sounds.button);
                        currentScreen = HOME;
                    }
                }

                //ExitGame Button
                if(CheckCollisionPointRec(mousePosition, buttons[3])) {
                    DrawRectangleLinesEx(outline[3], LINE_THICKNESS, RED);
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        PlaySound(sounds.button);
                        isGameRunning = 0;
                    }
                }

                break;

            case SAVEGAME:

                //TextBox
                if(CheckCollisionPointRec( mousePosition, textBox)) {
                    DrawRectangleLinesEx(outlineTextBox, 5, DARKGRAY);
                    int key = GetCharPressed();

                    while ( key > 0)
                    {
                        if( (key >= 32) && (key  <= 125) && (letterCount < MAX_INPUT_CHARS))
                        {
                            player.name[letterCount] = (char)key;
                            player.name[letterCount + 1] = '\0';
                            letterCount++;
                        }
                        key = GetCharPressed();
                    }


                if(IsKeyPressed(KEY_BACKSPACE))
                    {
                        letterCount--;
                        if(letterCount < 0) {letterCount = 0;}
                        player.name[letterCount] = '\0';
                    }
                }

                //SaveProgress button
                if(CheckCollisionPointRec( mousePosition, buttons[4])) {
                    DrawRectangleLinesEx(outline[4], LINE_THICKNESS, GREEN);
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        PlaySound(sounds.button);
                    }
                }

                //Return button
                if(CheckCollisionPointRec( mousePosition, buttons[5])) {
                    DrawRectangleLinesEx(outline[5], LINE_THICKNESS, BLACK);
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        PlaySound(sounds.button);
                        currentScreen = GAMEOVER;
                    }
                }

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

                DrawRectangleRec(buttons[0], BLACK);
                DrawText("Restart Game", (SCREEN_WIDTH - MeasureText("Restart Game",40)) / 2, 228, 40, WHITE);

                DrawRectangleRec(buttons[1], BLACK);
                DrawText("Save Game", (SCREEN_WIDTH - MeasureText("Save Game",40)) / 2, 353, 40, WHITE);

                DrawRectangleRec(buttons[2], BLACK);
                DrawText("Back to menu", (SCREEN_WIDTH - MeasureText("Back to menu",40)) / 2, 478, 40, WHITE);

                DrawRectangleRec(buttons[3], BLACK);
                DrawText("Exit Game", (SCREEN_WIDTH - MeasureText("Exit Game",40)) / 2, 603, 40, WHITE);

                break;

            case SAVEGAME:
                DrawText("Save Game menu", (SCREEN_WIDTH - MeasureText("Save Game Menu", 80)) / 2, 80, 80, BLACK );
                DrawText(TextFormat("You got %d points!"), (SCREEN_WIDTH - MeasureText(TextFormat("You got %d points!"),40)) / 2, 180, 40, DARKGREEN);

                DrawText("Place mouse over input box to write your name!", ( (SCREEN_WIDTH - MeasureText("Place mouse over input box to write your name!",29)) / 2), 350, 29, BLACK);
                DrawRectangleRec(textBox, GRAY);
                DrawText(player.name, ((SCREEN_WIDTH - 700) / 2) + 5, 395 , 78, DARKGRAY);

                DrawRectangleRec(buttons[4], DARKGREEN);
                DrawText( "Save Progress", ((SCREEN_WIDTH - MeasureText("Save Progress", 40)) / 2) - ((RECTANGLE_WIDTH - ( 2 * LINE_THICKNESS)) /2), 603, 40, GREEN);

                DrawRectangleRec(buttons[5], RED);
                DrawText( "Return", ((SCREEN_WIDTH - MeasureText("Return", 40)) / 2) + ((RECTANGLE_WIDTH + ( 2 * LINE_THICKNESS)) /2), 603, 40, BLACK);

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
