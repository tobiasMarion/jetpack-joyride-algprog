#include "raylib.h"
#include <math.h>


typedef struct Sounds {
    Sound button;
    Sound coin;
    Sound hit;
} Sounds;

typedef struct Player {
    int gridX;
    int coins;
    int lives;
    int isTouchingTheGround;
    int isInvulnerable;
    float invulnerableUntill;
    float jumpPower;
    float positionY;
    float speedY;
    Texture2D texture;
    char name[MAX_INPUT_CHARS + 1];

} Player;

void initializePlayer(Player *player, float startYPosition, char textureName[]) {
    player->gridX = INITIAL_X_POSITION;
    player->jumpPower = INITIAL_JUMP_POWER;
    player->texture = LoadTexture(textureName);
    player->positionY = startYPosition * CELL_SIZE;
    player->speedY = 0;
    player->isTouchingTheGround = 0;
    player->lives = 3;
    player->coins = 0;
    player->isInvulnerable = 1;
    player->invulnerableUntill = GetTime() + INVULNERABLE_AFTER_HIT_DURATION;
    player->name[0] = '\0';
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

int checksCollision(Player *player, MapSection map, Sounds *sounds) {
    int y = (int)(player->positionY / CELL_SIZE);
    int x = round(INITIAL_X_POSITION + offsetX);

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

    if (player->isInvulnerable) {
        return 0;
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

    if (map[y][x] == 'Z') {
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

    return 0;
}
