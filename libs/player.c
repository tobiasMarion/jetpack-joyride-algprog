#include "raylib.h"

#define GRAVITY 0.5
#define INITIAL_X_POSITION 6

typedef struct Player {
    int gridX;
    float positionY;
    float speedY;
    Texture2D texture;
} Player;


void initializePlayer(Player *player, char textureName[], float startYPosition) {
    player->gridX = INITIAL_X_POSITION;
    player->texture = LoadTexture(textureName);
    player->positionY = startYPosition * CELL_SIZE;
    player->speedY = 0;
}

void movePlayer(Player *player) {
    player->speedY += GRAVITY;

    if (IsKeyDown(KEY_UP)) {
        player->speedY = -8.0;
    }

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

