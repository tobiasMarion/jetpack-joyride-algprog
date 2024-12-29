#include "raylib.h"

typedef enum GameScreen { HOME, TITLE, GAMEPLAY, ERROR, GAMEOVER, ENDGAME, SAVEGAME} GameScreen;

int letterCount = 0;

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

Rectangle outlineTextBox = {((SCREEN_WIDTH - 700 ) / 2) - 5  , 400 - 5, 710, 95};
Rectangle textBox = {(SCREEN_WIDTH - 700 ) / 2 , 400, 700, 85};


void drawSaveGameScreen(GameScreen *currentScreen, Player *player, Sound *buttonClickSound) {
    Vector2 mousePosition = GetMousePosition();
    //TextBox
    if(CheckCollisionPointRec( mousePosition, textBox)) {
        DrawRectangleLinesEx(outlineTextBox, 5, DARKGRAY);
        int key = GetCharPressed();

        while (key > 0) {
            if((key >= 32) && (key  <= 125) && (letterCount < MAX_INPUT_CHARS)) {
                player->name[letterCount] = (char)key;
                player->name[letterCount + 1] = '\0';
                letterCount++;
            }
            key = GetCharPressed();
        }


    if(IsKeyPressed(KEY_BACKSPACE)) {
            letterCount--;
            if(letterCount < 0) {letterCount = 0;}
            player->name[letterCount] = '\0';
        }
    }

    //SaveProgress button
    if(CheckCollisionPointRec( mousePosition, buttons[4])) {
        DrawRectangleLinesEx(outline[4], LINE_THICKNESS, GREEN);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            PlaySound(*buttonClickSound);
        }
    }

    //Return button
    if(CheckCollisionPointRec( mousePosition, buttons[5])) {
        DrawRectangleLinesEx(outline[5], LINE_THICKNESS, BLACK);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            PlaySound(*buttonClickSound);
            *currentScreen = GAMEOVER;
        }
    }

    DrawText("Save Game menu", (SCREEN_WIDTH - MeasureText("Save Game Menu", 80)) / 2, 80, 80, BLACK );
    DrawText(TextFormat("You got %d points!"), (SCREEN_WIDTH - MeasureText(TextFormat("You got %d points!"),40)) / 2, 180, 40, DARKGREEN);

    DrawText("Place mouse over input box to write your name!", ( (SCREEN_WIDTH - MeasureText("Place mouse over input box to write your name!",29)) / 2), 350, 29, BLACK);
    DrawRectangleRec(textBox, GRAY);
    DrawText(player->name, ((SCREEN_WIDTH - 700) / 2) + 5, 395 , 78, DARKGRAY);

    DrawRectangleRec(buttons[4], DARKGREEN);
    DrawText( "Save Progress", ((SCREEN_WIDTH - MeasureText("Save Progress", 40)) / 2) - ((RECTANGLE_WIDTH - ( 2 * LINE_THICKNESS)) /2), 603, 40, GREEN);

    DrawRectangleRec(buttons[5], RED);
    DrawText( "Return", ((SCREEN_WIDTH - MeasureText("Return", 40)) / 2) + ((RECTANGLE_WIDTH + ( 2 * LINE_THICKNESS)) /2), 603, 40, BLACK);
}

void drawGameOverScreen(GameScreen *currentScreen, int *isGameRunning, Player *player, Sound *buttonClickSound) {
    Vector2 mousePosition = GetMousePosition();

    //Restart button
    if(CheckCollisionPointRec(mousePosition, buttons[0])) {
        DrawRectangleLinesEx(outline[0], LINE_THICKNESS, RED);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            PlaySound(*buttonClickSound);
            initializePlayer(player, 6, "resources/player.png");
            *currentScreen = GAMEPLAY;
        }
    }

    //SaveGame button
    if(CheckCollisionPointRec(mousePosition, buttons[1])) {
        DrawRectangleLinesEx(outline[1], LINE_THICKNESS, RED);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            PlaySound(*buttonClickSound);
            *currentScreen = SAVEGAME;
        }
    }

    //Menu button
    if(CheckCollisionPointRec(mousePosition, buttons[2])) {
        DrawRectangleLinesEx(outline[2], LINE_THICKNESS, RED);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            PlaySound(*buttonClickSound);
            *currentScreen = HOME;
        }
    }

    //ExitGame Button
    if(CheckCollisionPointRec(mousePosition, buttons[3])) {
        DrawRectangleLinesEx(outline[3], LINE_THICKNESS, RED);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            PlaySound(*buttonClickSound);
            *isGameRunning = 0;
        }
    }


    DrawText("YOU DIED",  (SCREEN_WIDTH - MeasureText("YOU DIED",100)) / 2 , 80, 100, RED);

    DrawRectangleRec(buttons[0], BLACK);
    DrawText("Restart Game", (SCREEN_WIDTH - MeasureText("Restart Game",40)) / 2, 228, 40, WHITE);

    DrawRectangleRec(buttons[1], BLACK);
    DrawText("Save Game", (SCREEN_WIDTH - MeasureText("Save Game",40)) / 2, 353, 40, WHITE);

    DrawRectangleRec(buttons[2], BLACK);
    DrawText("Back to menu", (SCREEN_WIDTH - MeasureText("Back to menu",40)) / 2, 478, 40, WHITE);

    DrawRectangleRec(buttons[3], BLACK);
    DrawText("Exit Game", (SCREEN_WIDTH - MeasureText("Exit Game",40)) / 2, 603, 40, WHITE);
}

void drawTitleScreen(GameScreen *currentScreen) {
     if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
        *currentScreen = GAMEPLAY;
    }

    DrawRectangle(0, 0,SCREEN_WIDTH, SCREEN_HEIGHT, GREEN);
    DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
    DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
}

void drawHomeScreen(GameScreen *currentScreen) {
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
        *currentScreen = TITLE;
    }

    DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
}
