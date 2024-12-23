#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.c"
#include "raylib.h"

#define MAX_PATH_SIZE 30
#define MAP_HEIGHT 12
#define MAP_WIDTH 240
#define SECTION_WIDTH 30
#define TOTAL_SECTIONS MAP_WIDTH / SECTION_WIDTH
#define CELL_SIZE 64

typedef struct Player {
    Vector2 position;     // Posição do jogador (x, y)
    Texture2D texture;    // Textura do jogador
    float velocityY;      // Velocidade vertical
    float gravity;        // Intensidade da gravidade
    int gridX;            // Posição fixa no grid (coluna 6)
} Player;

// This value is used to draw the map more fluidly
float offsetX = 0;

typedef char MapSection[MAP_HEIGHT][SECTION_WIDTH];

int readMapFile(int levelNumber, MapSection mapSections[TOTAL_SECTIONS]) {
    FILE *file = NULL;
    char validChars[] = "XCZ ";
    char filePath[MAX_PATH_SIZE] = {0};
    char a;

    snprintf(filePath, MAX_PATH_SIZE, "./maps/mapa%d.txt", levelNumber);

    file = fopen(filePath, "r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", filePath);
        return 0;
    }

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            a = fgetc(file);

            if (strchr(validChars, a) == NULL) {
                printf("Caractere invalido '%c' encontrado na linha %d, coluna %d\n", a, i + 1, j + 1);
                fclose(file);
                return 0;
            }

            int section = j / SECTION_WIDTH;
            int pos = j % SECTION_WIDTH;
            mapSections[section][i][pos] = a;
        }

        if (fgetc(file) != '\n') {
            printf("Erro: esperado caractere de nova linha apos a linha %d.\n", i + 1);
            fclose(file);
            return 0;
        }
    }

    fclose(file);

    return 1;
}

void printMapSection(MapSection section) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < SECTION_WIDTH; j++) {
            printf("%c", section[i][j]);
        }
        printf("\n");
    }

    printf("\n\n");
}

void loadMapInto(MapSection map, MapSection mapSections[TOTAL_SECTIONS]) {
    int randomSection = getRandIntBetween(0, TOTAL_SECTIONS);

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < SECTION_WIDTH; j++) {
            map[i][j] = mapSections[randomSection][i][j];
        }
    }
}

void moveMapMatrix(MapSection loadedSections[2]) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < SECTION_WIDTH - 1; j++) {
            loadedSections[0][i][j] = loadedSections[0][i][j + 1];
        }

        loadedSections[0][i][SECTION_WIDTH - 1] = loadedSections[1][i][0];
    }

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < SECTION_WIDTH - 1; j++) {
            loadedSections[1][i][j] = loadedSections[1][i][j + 1];
        }
    }
}

void moveMap(float speed, MapSection loadedSections[2]) {
    offsetX += speed;

    if (offsetX > 1) {
        moveMapMatrix(loadedSections);
        offsetX--;
    }
}


void drawCell(char c, int coordX, int coordY, Texture2D wallTexture, Texture2D coinTexture) {
    int x = (coordX * CELL_SIZE) - (offsetX * CELL_SIZE);
    int y = coordY * CELL_SIZE;

    if (c == ' ') {
        return; // Célula vazia, nada para desenhar
    }

    Rectangle sourceRect = {0, 0, (float)wallTexture.width, (float)wallTexture.height}; // Retângulo de origem (tamanho original da textura)
    Rectangle destRect = {x, y, CELL_SIZE, CELL_SIZE}; // Retângulo de destino com o tamanho da célula

    switch (c) {
        case 'Z':  // Parede
            DrawTexturePro(wallTexture, sourceRect, destRect, (Vector2){0, 0}, 0.0f, WHITE);
            break;
        case 'C':  // Moeda
            DrawTexturePro(coinTexture, sourceRect, destRect, (Vector2){0, 0}, 0.0f, WHITE);
            break;
        case 'X':  // Obstáculo ou outro elemento (opcional)
            DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, BLACK); // Ou adicione outra textura aqui
            break;
    }
}


void drawMap(MapSection loadedMap[2], Texture2D wallTexture, Texture2D coinTexture) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < SECTION_WIDTH; j++) {
            drawCell(loadedMap[0][i][j], j, i, wallTexture, coinTexture);
        }
    }

    for (int i = 0; i < MAP_HEIGHT; i++) {
        drawCell(loadedMap[1][i][0], SECTION_WIDTH, i, wallTexture, coinTexture);
    }
}

void initializePlayer(Player *player, Vector2 startPosition, Texture2D texture) {
    player->gridX = 6;  // Coluna fixa no grid
    player->position = startPosition;
    player->texture = texture;
    player->velocityY = 0.0f;  // Velocidade inicial
    player->gravity = 0.5f;    // Gravidade padrão
}

void movePlayer(Player *player) {
    // Aplique a gravidade
    player->velocityY += player->gravity;

    // Controle para subir
    if (IsKeyDown(KEY_UP)) {
        player->velocityY = -8.0f; // Impulso para cima
    }

    // Atualize a posição vertical
    player->position.y += player->velocityY;

    // Restrinja o jogador dentro dos limites da tela
    if (player->position.y < CELL_SIZE) {
        player->position.y = CELL_SIZE;
        player->velocityY = 0; // Evita "grudar" no topo
    }

    if (player->position.y > (MAP_HEIGHT - 2) * CELL_SIZE) {
        player->position.y = (MAP_HEIGHT - 2) * CELL_SIZE;
        player->velocityY = 0; // Evita ultrapassar o chão
    }
}


void drawPlayer(Player *player) {
    // Redefine a fonte da textura para ser proporcional ao tamanho da célula
    Rectangle sourceRect = {0, 0, (float)player->texture.width, (float)player->texture.height};
    Rectangle destRect = {(float)(player->gridX * CELL_SIZE), player->position.y, CELL_SIZE, CELL_SIZE}; // Destino com o tamanho da célula

    // Desenha o jogador com a textura redimensionada
    DrawTexturePro(player->texture, sourceRect, destRect, (Vector2){0, 0}, 0.0f, WHITE);
}
