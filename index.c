#include <stdio.h>
#include "raylib.h"
#include "./libs/constants.h"
#include "./libs/utils.c"
#include "./libs/map.c"
#include "./libs/player.c"
#include "./libs/leaderboard.c"
#include "./libs/ui.c"


int main() {
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Jetpack Joyride - INF5102");
    InitAudioDevice();
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
    int framesCounter = 0;
    int isGameRunning = 1;
    GameScreen currentScreen = HOME;
    char errorMessage[ERROR_MESSAGE_LENGTH] = {0};
    char saveMessage[ERROR_MESSAGE_LENGTH] = {0};

    Sounds sounds = {
        LoadSound("resources/sounds/button1.wav"),
        LoadSound("resources/sounds/coin.mp3"),
        LoadSound("resources/sounds/hit.mp3"),
        LoadSound("resources/sounds/laser.mp3")
    };

    Save allSaves[MAX_SAVES] = {0};
    Save currentSave;
    initializeSave(&currentSave);
    int allSaveSize = openSavesFile(allSaves);

    Player player;
    Level level = {0};

    double upLevelAt = 0;

    int currentLevel = 1;
    int isLevelLoaded = 0;
    float currentSpeed;
    int isSlowMotionActive = 0;
    int slowMotionUntil = 0;

    MapSection loadedMap[2] = {0};
    Lasers lasers = {0};
    Texture2D laserTexture = LoadTexture("resources/laser.png");

    //--------------------------------------------------------------------------------------

    while (!WindowShouldClose() && isGameRunning) {
        framesCounter++;

        // Mechanics
        if (currentScreen == GAMEPLAY) {
            if (!isLevelLoaded) {
                isLevelLoaded = loadLevel(currentLevel, &level, errorMessage);

                if (!isLevelLoaded) {
                    currentScreen = ERROR;
                    continue;
                }

                loadEmptyMap(loadedMap[0]);
                loadMapRandomly(loadedMap[1], level.mapSections);
            }

            if(player.lives <= 0) {
                currentScreen = GAMEOVER;
                removeAllLasers(lasers);
            }

            if(IsKeyPressed(KEY_ESCAPE)) {
                currentScreen = PAUSE;
            }

            checkCheatWords(&player, &slowMotionUntil, &isSlowMotionActive);

            if (isSlowMotionActive) {
                currentSpeed = level.speed * 0.5;
            } else {
                currentSpeed = level.speed;
            }

            player.distance += moveMap(currentSpeed, loadedMap);

            // Checks if a new sections needs to be loaded
            if (framesCounter % (int)(1 / level.speed * SECTION_WIDTH) == 0) {
                loadMapRandomly(loadedMap[1], level.mapSections);
            }

            if (framesCounter % ((int)(1 / level.speed * SECTION_WIDTH) * 4) == 0) {
                spawnLasers(lasers, &level);
            }

            if (player.isInvulnerable && GetTime() > player.invulnerableUntill) {
                player.isInvulnerable = 0;
            }

            if (isSlowMotionActive && GetTime() > slowMotionUntil) {
                isSlowMotionActive = 0;
            }

            if (!player.isTouchingTheGround) {
                movePlayer(&player, level.gravity);
            }

            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_SPACE) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                player.isTouchingTheGround = 0;
                movePlayer(&player, player.jumpPower);
            }

            checkMapCollision(&player, loadedMap[0], sounds);
            checkLasersColision(&player, lasers, sounds.hit);

            if (player.distance > level.requiredDistanceToNextLevel && isLevelLoaded) {
                currentLevel++;
                upLevelAt = GetTime();
                isLevelLoaded = 0;
                currentScreen = NEXT_LEVEL;
                unloadMapTextures(&level.mapTextures);
                removeAllLasers(lasers);
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch(currentScreen) {
            case HOME:
                drawHomeScreen(&isGameRunning, &currentScreen, &player, &currentSave, &sounds.button, &currentLevel, &isLevelLoaded, allSaves, allSaveSize);
                break;

            case GAMEPLAY:
                if (!isLevelLoaded) {
                    break;
                }

                DrawText(TextFormat("DISTANCE: %d", player.distance), 20, 110, 35, RED);
                DrawText(TextFormat("COINS: %d", player.coins), 20, 80, 35, GOLD);
                drawMap(loadedMap, &level.mapTextures);
                drawPlayer(&player);
                drawLasers(lasers, &laserTexture, sounds.laser);
                DrawFPS(0, 0);
                break;

            case NEXT_LEVEL:
                drawNextLevelScreen(currentLevel, upLevelAt, &currentScreen);
                break;

            case GAMEOVER:
                drawGameOverScreen(&isGameRunning, &currentScreen, &player, &currentSave, &currentLevel, &isLevelLoaded, &sounds.button);
                break;

            case SAVEGAME:
                drawSaveGameScreen(&currentScreen, &currentSave, &sounds.button, &player, saveMessage, allSaves, allSaveSize);
                break;

            case ERROR:
                drawErrorScreen(&isGameRunning, errorMessage);
                break;

            case HIGHSCORES:
                drawHighScoresScreen(allSaves, allSaveSize, saveMessage, &sounds.button, &isGameRunning, &currentScreen);
                break;

            case PAUSE:
                drawPauseScreen(&sounds.button, &currentScreen);

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

    saveSavesFile(allSaves, allSaveSize);

    CloseWindow();

    return 0;
}
