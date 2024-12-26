#include "raylib.h"
#define INITIAL_X_POSITION 6
#define INITIAL_JUMP_POWER -1.2
#define MIN_Y_SPEED -10
#define MAX_Y_SPEED 15

typedef struct Sounds {
    Sound button;
    Sound coin;
} Sounds;

typedef struct Player {
    int gridX;
    int coins;
    int lives;
    float jumpPower;
    float positionY;
    float speedY;
    Texture2D texture;
} Player;

void initializePlayer(Player *player, char textureName[], float startYPosition) {
    player->gridX = INITIAL_X_POSITION;
    player->jumpPower = INITIAL_JUMP_POWER;
    player->texture = LoadTexture(textureName);
    player->positionY = startYPosition * CELL_SIZE;
    player->speedY = 0;
    player->lives = 3;
    player->coins = 0;
}

void movePlayer(Player *player, float speedToAdd) {
    player->speedY += speedToAdd;
    player->speedY = minMax(player->speedY, MIN_Y_SPEED, MAX_Y_SPEED);

    player->positionY += player->speedY;

    if (player->positionY < CELL_SIZE) {
        player->positionY = CELL_SIZE;
        player->speedY = 0;
    }

    if (player->positionY > (MAP_HEIGHT - 2) * CELL_SIZE) {
        player->positionY = (MAP_HEIGHT - 2) * CELL_SIZE;
        player->speedY = 0;
    }
}

void drawPlayer(Player *player) {
    Rectangle sourceRect = {0, 0, (float)player->texture.width, (float)player->texture.height};
    Rectangle destRect = {(float)(player->gridX * CELL_SIZE), player->positionY, CELL_SIZE, CELL_SIZE};

    float scaleX = CELL_SIZE / (float)player->texture.width;
    float scaleY = CELL_SIZE / (float)player->texture.height;

    DrawTexturePro(player->texture, sourceRect, destRect, (Vector2){0, 0}, 0.0f, WHITE);
}

int checksCollision(Player *player, MapSection map, Sounds *sounds) {
    int y = (int)player->positionY / CELL_SIZE;
    int x = (int)player->gridX + offsetX;

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

    return 0;
}
