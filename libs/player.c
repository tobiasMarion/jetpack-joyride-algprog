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

void applyDamageToPlayer(Player *player, Sound hitSound) {
    player->lives -= 1;
    player->isInvulnerable = 1;
    player->invulnerableUntill = GetTime() + INVULNERABLE_AFTER_HIT_DURATION;
    PlaySound(hitSound);
}

void collectCoin(Player *player, Sound coinSound) {
    player->coins += 1;
    PlaySound(coinSound);
}

void checkMapCollision(Player *player, MapSection map, Sounds sounds) {
    player->isTouchingTheGround = 0;
    Rectangle playerHitbox = { INITIAL_X_POSITION * CELL_SIZE,
                               player->positionY,
                               CELL_SIZE,
                               CELL_SIZE };

    // Checks all cells from the current and next column on map
    // It is necessary to check the next one because offsetX
    for (int col = player->gridX; col < player->gridX + 2; col++) {
        for (int row = 0; row < MAP_HEIGHT; row++) {
            Rectangle cell = {col * CELL_SIZE - offsetX, row * CELL_SIZE, CELL_SIZE, CELL_SIZE};

            if (CheckCollisionRecs(playerHitbox, cell)) {
                switch (map[row][col]) {
                    case 'C':
                        collectCoin(player, sounds.coin);
                        map[row][col] = ' ';
                        break;

                    case 'X':
                        if (col == player->gridX) {
                            if (player->speedY < 0) {
                                player->positionY = (row + 1) * CELL_SIZE;
                                player->speedY = 0;
                            } else if (player->speedY >= 0) {
                                player->positionY = (row - 1) * CELL_SIZE;
                                player->speedY = 0;
                                player->isTouchingTheGround = 1;
                            }
                        } else {
                            Rectangle overlap = GetCollisionRec(playerHitbox, cell);
                            int area = overlap.width * overlap.height;
                            if (area > CELL_SIZE / 4) {
                                applyDamageToPlayer(player, sounds.hit);
                            }
                        }
                        break;

                    case 'Z':
                        if (!player->isInvulnerable) {
                            applyDamageToPlayer(player, sounds.hit);
                        }
                }
            }
        }
    }
}

void checkLasersColision(Player *player, Lasers lasers, Sound hitSound) {
    int currentTime = GetTime();

    Rectangle playerHitbox = { INITIAL_X_POSITION * CELL_SIZE,
                               player->positionY,
                               CELL_SIZE,
                               CELL_SIZE };


    for (int i = 1; i < MAP_HEIGHT - 1; i++) {
        if (lasers[i] != 0 && currentTime > lasers[i] + LASER_ACTIVATION_DELAY) {
            Rectangle laser = { 0, i * CELL_SIZE, SCREEN_WIDTH, CELL_SIZE };
            if (CheckCollisionRecs(playerHitbox, laser)) {
                applyDamageToPlayer(player, hitSound);
            }
        }
    }
}

void addCharToBuffer(char charToAdd, char buffer[MAX_INPUT_CHARS + 1]) {
    int length = strlen(buffer);

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

    return strcmp(initialPointerToCheck, wordToCheck) == 0;

}

void checkCheatWords(Player *player, int *slowMotionUntil, int *isSlowMotionActive) {
    for (int key = 'A'; key <= 'Z'; key++) {
        if (IsKeyPressed(key)) {
            addCharToBuffer(key, player->inputBuffer);

            if (verifyWordOnBuffer(INVULNERABLE_CHEAT_CODE, player->inputBuffer)) {
                printf("CHEAT ACTIVATED: Player Invulnerable\n");
                player->isInvulnerable = 1;
                player->invulnerableUntill = GetTime() + 5;
                player->inputBuffer[0] = '\0';
            }

            if (verifyWordOnBuffer(SLOW_CHEAT_CODE, player->inputBuffer)) {
                printf("CHEAT ACTIVATED: Slow Map\n");
                *isSlowMotionActive = 1;
                *slowMotionUntil = GetTime() + 5;
                player->inputBuffer[0] = '\0';
            }
        }
    }
}

