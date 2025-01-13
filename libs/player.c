#include "raylib.h"
#include <math.h>
#include "constants.h"

typedef struct Sounds {
    Sound button;
    Sound coin;
    Sound hit;
    Sound laser;
} Sounds;

typedef struct Player {
    int gridX;
    int coins;
    int lives;
    int distance;
    int isTouchingTheGround;
    int isInvulnerable;
    float invulnerableUntill;
    float jumpPower;
    float positionY;
    float speedY;
    Texture2D texture;
    char inputBuffer[MAX_INPUT_CHARS + 1];
} Player;

void initializePlayer(Player *player, float startYPosition, char textureName[]) {
    player->gridX = INITIAL_X_POSITION;
    player->jumpPower = INITIAL_JUMP_POWER;
    player->texture = LoadTexture(textureName);
    player->positionY = startYPosition * CELL_SIZE;
    player->speedY = 0;
    player->isTouchingTheGround = 0;
    player->lives = 1;
    player->coins = 0;
    player->distance = 0;
    player->isInvulnerable = 1;
    player->invulnerableUntill = GetTime() + INVULNERABLE_AFTER_HIT_DURATION;
    player->inputBuffer[0] = '\0';
}

void movePlayer(Player *player, float speedToAdd) {
    player->speedY += speedToAdd;
    player->speedY = minMax(player->speedY, MIN_Y_SPEED, MAX_Y_SPEED);

    player->positionY += player->speedY;
}

void drawPlayer(Player *player) {
    Rectangle sourceRect = {0, 0, (float)player->texture.width, (float)player->texture.height};
    Rectangle destRect = {(float)(player->gridX * CELL_SIZE), player->positionY, CELL_SIZE, CELL_SIZE};

    float scaleX = CELL_SIZE / (float)player->texture.width;
    float scaleY = CELL_SIZE / (float)player->texture.height;

    Color color = player->isInvulnerable ? RED : WHITE;

    DrawTexturePro(player->texture, sourceRect, destRect, (Vector2){0, 0}, 0.0f, color);
}

int checksCollision(Player *player, MapSection map, Lasers lasers, Sounds *sounds) {
    int y = (int)(player->positionY / CELL_SIZE);
    int x = round(INITIAL_X_POSITION + offsetX);
    float decimalPartY = player->positionY - y;


    if (player->speedY < 0 && map[y][x] == 'X') {
        player->speedY = 0;
        player->positionY = (y + 1) * CELL_SIZE;
        y++;
    }

    if (player->speedY > 0 && map[y+1][x] == 'X') {
        player->speedY = 0;
        player->positionY = y * CELL_SIZE;
        y--;
        player->isTouchingTheGround = 1;
    }

    if (map[y][x] == 'C') {
        player->coins += 1;
        map[y][x] = ' ';
        PlaySound(sounds->coin);
    }

    if (map[y+1][x] == 'C') {
        player->coins += 1;
        map[y+1][x] = ' ';
        PlaySound(sounds->coin);
    }

    if (player->isInvulnerable) {
        return 0;
    }

    if (map[y][x] == 'Z') {
        player->lives -= 1;
        player->isInvulnerable = 1;
        player->invulnerableUntill = GetTime() + INVULNERABLE_AFTER_HIT_DURATION;
        PlaySound(sounds->hit);

        return 1;
    }

    if (map[y][x] == 'Z') {
        player->lives -= 1;
        player->isInvulnerable = 1;
        player->invulnerableUntill = GetTime() + INVULNERABLE_AFTER_HIT_DURATION;
        PlaySound(sounds->hit);

        return 1;
    }

    if (decimalPartY > 0.7 && map[y+1][x] == 'Z') {
        player->lives -= 1;
        player->isInvulnerable = 1;
        player->invulnerableUntill = GetTime() + INVULNERABLE_AFTER_HIT_DURATION;
        PlaySound(sounds->hit);

        return 1;
    }


    if (map[y][x+1] == 'X') {
        player->lives -= 1;
        player->isInvulnerable = 1;
        player->invulnerableUntill = GetTime() + INVULNERABLE_AFTER_HIT_DURATION;
        PlaySound(sounds->hit);

        return 1;
    }

    int currentTime = GetTime();
    int dt = currentTime - LASER_ACTIVATION_DELAY;
    int isTouchingLaser = (lasers[y] < dt && lasers[y] != 0) || (decimalPartY > 0.7 && lasers[y+1] < dt && lasers[y+1] != 0);

    if (isTouchingLaser) {
        player->lives -= 1;
        player->isInvulnerable = 1;
        player->invulnerableUntill = GetTime() + INVULNERABLE_AFTER_HIT_DURATION;
        PlaySound(sounds->hit);
    }

    return 0;
}

void addCharToBuffer(char charToAdd, char buffer[MAX_INPUT_CHARS + 1]) {
    int length = strlen(buffer);

    printf("%d\n", length);

    if (length < MAX_INPUT_CHARS) {
        buffer[length] = charToAdd;
        buffer[length + 1] = '\0';
        return;
    }

    for (int i = 0; i < MAX_INPUT_CHARS - 1; i++) {
        buffer[i] = buffer[i + 1];
    }

    buffer[MAX_INPUT_CHARS - 1] = charToAdd;
}

int verifyWordOnBuffer(char *wordToCheck, char *buffer) {
    int wordLenght = strlen(wordToCheck);
    int bufferLength = strlen(buffer);

    if (wordLenght > bufferLength) {
        return 0;
    }

    char *initialPointerToCheck = buffer + bufferLength - wordLenght;

    printf("%s %s", wordToCheck, initialPointerToCheck);

    return strcmp(initialPointerToCheck, wordToCheck) == 0;

}

void checkCheatWords(Player *player, int *slowMotionUntil, int *isSlowMotionActive) {
    for (int key = 'A'; key <= 'Z'; key++) {
        if (IsKeyPressed(key)) {
            addCharToBuffer(key, player->inputBuffer);
            printf("%s\n", player->inputBuffer);

            if (verifyWordOnBuffer(INVULNERABLE_CHEAT_CODE, player->inputBuffer)) {
                player->isInvulnerable = 1;
                player->invulnerableUntill = GetTime() + 5;
                player->inputBuffer[0] = '\0';
            }

            if (verifyWordOnBuffer(SLOW_CHEAT_CODE, player->inputBuffer)) {
                *isSlowMotionActive = 1;
                *slowMotionUntil = GetTime() + 5;
                player->inputBuffer[0] = '\0';
            }
        }
    }
}

