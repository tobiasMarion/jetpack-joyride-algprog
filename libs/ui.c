#include "raylib.h"

typedef enum GameScreen { HOME, GAMEPLAY, ERROR, GAMEOVER, ENDGAME, SAVEGAME} GameScreen;

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

void DrawButton(char label[], int y) {
    Rectangle button = {
        BUTTON_POSITION_X_CENTER,
        y,
        BUTTON_WIDTH,
        BUTTON_HEIGHT,
    };

    Vector2 mousePosition = GetMousePosition();
    bool isHovering = CheckCollisionPointRec(mousePosition, button);

    if (isHovering) {
        DrawRectangle(
            BUTTON_POSITION_X_CENTER - LINE_THICKNESS,
            y - LINE_THICKNESS,
            BUTTON_WIDTH + 2 * LINE_THICKNESS,
            BUTTON_HEIGHT + 2 * LINE_THICKNESS,
            RED
        );
    }

    int labelX = BUTTON_POSITION_X_CENTER + (BUTTON_WIDTH - MeasureText(label, BUTTON_LABEL_SIZE)) / 2;
    int labelY = y + (BUTTON_HEIGHT - BUTTON_LABEL_SIZE) / 2;


    DrawRectangleRec(button, BLACK);
    DrawText(label, labelX, labelY, BUTTON_LABEL_SIZE, isHovering ? RED : WHITE);
}


void drawHomeScreen(GameScreen *currentScreen, Sound *buttonSoundClick) {
    Rectangle playRectangle = { (SCREEN_WIDTH - RECTANGLE_WIDTH) / 2, 200, RECTANGLE_WIDTH, RECTANGLE_HEIGHT };
    Rectangle highscoresRectangle = { (SCREEN_WIDTH - RECTANGLE_WIDTH) / 2, 325, RECTANGLE_WIDTH, RECTANGLE_HEIGHT };
    Rectangle exitRectangle1 = { (SCREEN_WIDTH - RECTANGLE_WIDTH) / 2, 450, RECTANGLE_WIDTH, RECTANGLE_HEIGHT };
    Rectangle restartRectangle = { (SCREEN_WIDTH - RECTANGLE_WIDTH) / 2, 200, RECTANGLE_WIDTH, RECTANGLE_HEIGHT};
    Rectangle saveRectangle = { (SCREEN_WIDTH - RECTANGLE_WIDTH) / 2, 325, RECTANGLE_WIDTH, RECTANGLE_HEIGHT};
    Rectangle menuRectangle = { (SCREEN_WIDTH - RECTANGLE_WIDTH) / 2, 450, RECTANGLE_WIDTH, RECTANGLE_HEIGHT};
    Rectangle exitRectangle = { (SCREEN_WIDTH - RECTANGLE_WIDTH) / 2, 575, RECTANGLE_WIDTH, RECTANGLE_HEIGHT};
    int selectedOption = 0; // O menu começa com "Play" selecionado
    int selectedOption2 = 0; // O menu começa com "Restart Game" selecionado
    Color playColor = WHITE;
    Color highscoresColor = WHITE;
    Color exitColor = WHITE;
    Color exitColor2 = WHITE;
    Color backMenuColor = WHITE;
    Color saveColor = WHITE;
    Color restartColor = WHITE;


    if (selectedOption == 0) {
        PlaySound(*buttonSoundClick);
        playColor = RED;
        highscoresColor = WHITE;
        exitColor = WHITE;
    } else if (selectedOption == 1) {
        PlaySound(*buttonSoundClick);
        playColor = WHITE;
        highscoresColor = RED;
        exitColor = WHITE;
    } else if (selectedOption == 2) {
        PlaySound(*buttonSoundClick);
        playColor = WHITE;
        highscoresColor = WHITE;
        exitColor = RED;
    }

    if (IsKeyPressed(KEY_DOWN)) {
        selectedOption = (selectedOption + 1) % 3;
    } else if (IsKeyPressed(KEY_UP)) {
        selectedOption = (selectedOption + 2) % 3;
    }
}


void drawGameOverScreen() {
    DrawText("YOU DIED",  (SCREEN_WIDTH - MeasureText("YOU DIED",100)) / 2 , 80, 100, RED);

    DrawButton("Restart Game", 228);
    DrawButton("Save Game", 353);
    DrawButton("Back to Menu", 478);
    DrawButton("Exit Game", 603);
}


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
