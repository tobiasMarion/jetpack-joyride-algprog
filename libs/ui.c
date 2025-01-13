#include "raylib.h"
#include "string.h"
#include "constants.h"
//#include "leaderboard.c"



typedef enum GameScreen { HOME, GAMEPLAY, NEXT_LEVEL, GAMEOVER, ENDGAME, SAVEGAME, ERROR, HIGHSCORES} GameScreen;

int amountOfOptionsOnScreen = 0;
int currentSelectedOption = -1;

// Receiveing an callback to execute on click would make it much verbose
int createButton(char label[], int x, int y, int shortcut, int isSelected, Color color, Color hoverColor, Sound *sound) {
    Rectangle button = {
        x, y,
        BUTTON_WIDTH,
        BUTTON_HEIGHT,
    };

    Vector2 mousePosition = GetMousePosition();
    bool isHovering = CheckCollisionPointRec(mousePosition, button);

    if (isHovering || isSelected) {
        DrawRectangle(
            x - LINE_THICKNESS,
            y - LINE_THICKNESS,
            BUTTON_WIDTH + 2 * LINE_THICKNESS,
            BUTTON_HEIGHT + 2 * LINE_THICKNESS,
            hoverColor
        );
    }

    int labelX = x + (BUTTON_WIDTH - MeasureText(label, BUTTON_LABEL_SIZE)) / 2;
    int labelY = y + (BUTTON_HEIGHT - BUTTON_LABEL_SIZE) / 2;


    DrawRectangleRec(button, color);
    DrawText(label, labelX, labelY, BUTTON_LABEL_SIZE, isHovering || isSelected ? hoverColor : WHITE);

    if ((isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(shortcut)|| (isSelected && IsKeyPressed(KEY_ENTER))) {
        if (sound != NULL) {
            PlaySound(*sound);
        }

        return 1;
    }

    return 0;
}


void createInputText(char value[], int x, int y, Color bg, Color border, Color hover) {
    Vector2 mousePosition = GetMousePosition();
    Rectangle rectangle = {
        x, y,
        BUTTON_WIDTH,
        BUTTON_HEIGHT,
    };

    bool isHovering = CheckCollisionPointRec(mousePosition, rectangle);
    int stringSize = strlen(value);
    char c;


    if (isHovering) {
        if(IsKeyPressed(KEY_BACKSPACE) && stringSize > 0) {
            value[stringSize - 1] = '\0';
            stringSize--;
        }

        c = GetCharPressed();

        while (c != 0) {
            if((c >= 32) && (c  <= 125) && (stringSize < MAX_INPUT_CHARS)) {
                value[stringSize] = c;
                value[stringSize + 1] = '\0';
            }
            c = GetCharPressed();
        }
    }


    DrawRectangle(
        x - LINE_THICKNESS,
        y - LINE_THICKNESS,
        BUTTON_WIDTH + 2 * LINE_THICKNESS,
        BUTTON_HEIGHT + 2 * LINE_THICKNESS,
        isHovering? hover : border
    );

    int labelX = x + (BUTTON_WIDTH - MeasureText(value, BUTTON_LABEL_SIZE)) / 2;
    int labelY = y + (BUTTON_HEIGHT - BUTTON_LABEL_SIZE) / 2;

    DrawRectangleRec(rectangle, bg);
    DrawText(value, labelX, labelY, BUTTON_LABEL_SIZE, isHovering ? hover : border);
}

void navigateWithArrowKeys(int totalOptions) {
    amountOfOptionsOnScreen = totalOptions;
    Vector2 mouseDelta = GetMouseDelta();

    if (mouseDelta.x != 0 || mouseDelta.y != 0) {
        currentSelectedOption = -1;
    }

    if (IsKeyPressed(KEY_UP)) {
        currentSelectedOption = (currentSelectedOption + amountOfOptionsOnScreen - 1) % amountOfOptionsOnScreen;
    }

    if (IsKeyPressed(KEY_DOWN)) {
        currentSelectedOption = (currentSelectedOption + 1) % amountOfOptionsOnScreen;
    }
}

int isOptionSelected(int o) {
    return currentSelectedOption == o;
}


void drawHomeScreen(int *isGameRunning, GameScreen *currentScreen, Player *player, Save *currentSave, Sound *buttonClickSound, int *currentLevel, int *isLevelLoaded, Save *allSaves,  int allSavesVectorSize) {
    int buttonX = BUTTON_POSITION_X_CENTER;
    navigateWithArrowKeys(3);

    DrawText("Jetpack Joyride",  (SCREEN_WIDTH - MeasureText("Jetpack Joyride", 100)) / 2 , 80, 100, RED);

    if (createButton("Play", buttonX, 225, KEY_R, isOptionSelected(0), BLACK, RED, buttonClickSound)) {
        initializePlayer(player, 6, "resources/player.png");
        initializeSave(currentSave);
        *currentLevel = 1;
        *isLevelLoaded = 0;
        *currentScreen = GAMEPLAY;

    }

    if (createButton("Highscores", buttonX, 350, KEY_S, isOptionSelected(1), BLACK, RED, buttonClickSound)) {
        *currentScreen = HIGHSCORES;
    }

    if (createButton("Exit Game", buttonX, 475, KEY_E, isOptionSelected(2), BLACK, RED, buttonClickSound)) {
        *isGameRunning = 0;
    }
}


void drawGameOverScreen(
    int *isGameRunning,
    GameScreen *currentScreen,
    Player *player,
    Save *currentSave,
    int *currentLevel,
    int *isLevelLoaded,
    Sound *buttonClickSound
) {
    int buttonX = BUTTON_POSITION_X_CENTER;
    navigateWithArrowKeys(4);

    DrawText("YOU DIED",  (SCREEN_WIDTH - MeasureText("YOU DIED",100)) / 2 , 80, 100, RED);

    if (createButton("Restart Game", buttonX, 225, KEY_R, isOptionSelected(0), BLACK, RED, buttonClickSound)) {
        initializePlayer(player, 6, "resources/player.png");
        initializeSave(currentSave);
        *currentScreen = GAMEPLAY;
        *currentLevel = 1;
        *isLevelLoaded = 0;
    }

    if (createButton("Save Game", buttonX, 350, KEY_S, isOptionSelected(1), BLACK, RED, buttonClickSound)) {
        *currentScreen = SAVEGAME;
    }

    if (createButton("Back to Menu", buttonX, 475, KEY_M, isOptionSelected(2), BLACK, RED, buttonClickSound)) {
        *currentScreen = HOME;
    }

    if (createButton("Exit Game", buttonX, 600, KEY_E, isOptionSelected(3), BLACK, RED, buttonClickSound)) {
        *isGameRunning = 0;
    }
}


void drawSaveGameScreen(GameScreen *currentScreen, Save *save, Sound *buttonClickSound, Player *player,
                        char *globalMessage, Save *allSaves, int allSaveVectorSize) {
    score(save, player);
    navigateWithArrowKeys(2);
    DrawText("Save Game menu", (SCREEN_WIDTH - MeasureText("Save Game Menu", 80)) / 2, 80, 80, BLACK );
    DrawText(TextFormat("You got %d points!", save->points), (SCREEN_WIDTH - MeasureText(TextFormat("You got %d points!"),40)) / 2, 180, 40, DARKGREEN);

    DrawText("Place mouse over input box to write your name!", ( (SCREEN_WIDTH - MeasureText("Place mouse over input box to write your name!",29)) / 2), 350, 29, BLACK);

    int buttonsY = 550;
    int buttonsGap = 50;
    int saveButtonX = (SCREEN_WIDTH - (2 * BUTTON_WIDTH + buttonsGap)) / 2;
    int returnButtonX = saveButtonX + BUTTON_WIDTH + buttonsGap;

    createInputText(save->name, BUTTON_POSITION_X_CENTER, 425, WHITE, GRAY, BLACK);


    if (createButton("Save Progress", saveButtonX, buttonsY, -1, isOptionSelected(0), GREEN, BLACK, buttonClickSound)) {
        if(verifyName(save)) {
            globalMessage[0] = '\0';
            saveGame(save, globalMessage, allSaves, allSaveVectorSize);
            *currentScreen = HIGHSCORES;
        } else{
            strcpy(globalMessage, "Write a name to save game!");
        }
    }

    DrawText(globalMessage, (SCREEN_WIDTH - MeasureText(globalMessage,50)) / 2, 700,50,RED);

    if (createButton("Return", returnButtonX, buttonsY, -1, isOptionSelected(1), BLACK, RED, buttonClickSound)) {
        *currentScreen = GAMEOVER;
    }
}


void drawNextLevelScreen(int n, double upLevelAt, GameScreen *currentScreen) {
    double dt = GetTime() - upLevelAt;

    if (dt > 1) {
        *currentScreen = GAMEPLAY;
        return;
    }

    DrawText(TextFormat("Level %d", n), (SCREEN_WIDTH - MeasureText("Level 0", 100)) / 2, 80, 100, GREEN);
    DrawText(
         TextFormat("Faster, Heavier... Harder", n),
         (SCREEN_WIDTH - MeasureText("Faster, Heavier... Harder", 40)) / 2,
         240, 40, BLACK
    );
}


void drawErrorScreen(int *isGameRunning, char errorMessage[ERROR_MESSAGE_LENGTH]) {
    ClearBackground(RED);

    DrawText("ERROR",
        (SCREEN_WIDTH - MeasureText("ERROR", 100)) / 2,
        80, 100, WHITE
    );

    DrawText(errorMessage,
        (SCREEN_WIDTH - MeasureText(errorMessage, 60)) / 2,
        256, 60, WHITE
    );
}

void drawSave(Save *allSaves, int size) {
    int i = 0;
    int height = 200;

    for( i = 0; i < size ; i++) {
        DrawText(allSaves[i].name, 200, height, 40, WHITE);
        DrawText(TextFormat("%d",allSaves[i].points), 700, height, 40, WHITE);
        DrawText(TextFormat("%2d/%2d/%4d - %2d:%2d",allSaves[i].currentDate.month, allSaves[i].currentDate.day,
                 allSaves[i].currentDate.year,allSaves[i].currentDate.hour,allSaves[i].currentDate.minute),
                 1400,height,40,WHITE);
        height = height + 40;
    }
}


void drawHighScoresScreen(Save *allSaves, int allSavesSize, char *globalMessage, Sound *buttonClickSound, int *isGameRunning, GameScreen *currentScreen) {
    ClearBackground(BLACK);
    navigateWithArrowKeys(2);
    DrawText("Highscores", 200 , 40, 80, RED );
    DrawText(globalMessage, 20, 708, 30, DARKPURPLE);

    DrawText("NAME:", 200, 160, 40, GOLD);
    DrawText("SCORE:", 700, 160, 40, GOLD);
    DrawText("DATE:",1400, 160, 40, GOLD);
    drawSave(allSaves, allSavesSize);

    if(createButton("Exit", 1400, 650, KEY_E, isOptionSelected(0), RED, DARKPURPLE, buttonClickSound)) {
        *isGameRunning = 0;
    }

    if(createButton("Back to menu", 880, 650, KEY_B, isOptionSelected(1), DARKGREEN, GREEN, buttonClickSound)) {
        globalMessage[0] = '\0';
        *currentScreen = HOME;
    }
}

