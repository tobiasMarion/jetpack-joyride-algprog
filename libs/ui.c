#include "raylib.h"
#include "string.h"
#include "./constants.h"

typedef enum GameScreen { HOME, GAMEPLAY, ERROR, GAMEOVER, ENDGAME, SAVEGAME} GameScreen;

// Receiveing an callback to execute on click would make it much verbose
int createButton(char label[], int x, int y, int shortcut, Color color, Color hoverColor, Sound *sound) {
    Rectangle button = {
        x, y,
        BUTTON_WIDTH,
        BUTTON_HEIGHT,
    };

    Vector2 mousePosition = GetMousePosition();
    bool isHovering = CheckCollisionPointRec(mousePosition, button);

    if (isHovering) {
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
    DrawText(label, labelX, labelY, BUTTON_LABEL_SIZE, isHovering ? hoverColor : WHITE);

    if ((isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(shortcut)) {
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


void drawHomeScreen(int *isGameRunning, GameScreen *currentScreen, Player *player, Sound *buttonClickSound) {
    int buttonX = BUTTON_POSITION_X_CENTER;
    DrawText("Jetpack Joyride",  (SCREEN_WIDTH - MeasureText("Jetpack Joyride", 100)) / 2 , 80, 100, RED);

    if (createButton("Play", buttonX, 225, KEY_R, BLACK, RED, buttonClickSound)) {
        initializePlayer(player, 6, "resources/player.png");
        *currentScreen = GAMEPLAY;
    }

    if (createButton("Highscores", buttonX, 350, KEY_S, BLACK, RED, buttonClickSound)) {
        *currentScreen = SAVEGAME;
    }

    if (createButton("Exit Game", buttonX, 475, KEY_E, BLACK, RED, buttonClickSound)) {
        *isGameRunning = 0;
    }
}


void drawGameOverScreen( int *isGameRunning, GameScreen *currentScreen, Player *player, Sound *buttonClickSound) {
    int buttonX = BUTTON_POSITION_X_CENTER;
    DrawText("YOU DIED",  (SCREEN_WIDTH - MeasureText("YOU DIED",100)) / 2 , 80, 100, RED);

    if (createButton("Restart Game", buttonX, 225, KEY_R, BLACK, RED, buttonClickSound)) {
        initializePlayer(player, 6, "resources/player.png");
        *currentScreen = GAMEPLAY;
    }

    if (createButton("Save Game", buttonX, 350, KEY_S, BLACK, RED, buttonClickSound)) {
        *currentScreen = SAVEGAME;
    }

    if (createButton("Back to Menu", buttonX, 475, KEY_M, BLACK, RED, buttonClickSound)) {
        *currentScreen = HOME;
    }

    if (createButton("Exit Game", buttonX, 600, KEY_E, BLACK, RED, buttonClickSound)) {
        *isGameRunning = 0;
    }
}


void drawSaveGameScreen(GameScreen *currentScreen, Player *player, Sound *buttonClickSound) {
    DrawText("Save Game menu", (SCREEN_WIDTH - MeasureText("Save Game Menu", 80)) / 2, 80, 80, BLACK );
    DrawText(TextFormat("You got %d points!"), (SCREEN_WIDTH - MeasureText(TextFormat("You got %d points!"),40)) / 2, 180, 40, DARKGREEN);

    DrawText("Place mouse over input box to write your name!", ( (SCREEN_WIDTH - MeasureText("Place mouse over input box to write your name!",29)) / 2), 350, 29, BLACK);

    int buttonsY = 550;
    int buttonsGap = 50;
    int saveButtonX = (SCREEN_WIDTH - (2 * BUTTON_WIDTH + buttonsGap)) / 2;
    int returnButtonX = saveButtonX + BUTTON_WIDTH + buttonsGap;

    createInputText(player->name, BUTTON_POSITION_X_CENTER, 425, WHITE, GRAY, BLACK);

    if (createButton("Save Progress", saveButtonX, buttonsY, -1, GREEN, BLACK, buttonClickSound)) {
    }

    if (createButton("Return", returnButtonX, buttonsY, -1, BLACK, RED, buttonClickSound)) {
        *currentScreen = GAMEOVER;
    }
}
