#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.c"
#include "raylib.h"

typedef char MapSection[MAP_HEIGHT][SECTION_WIDTH];

typedef struct MapTextures {
    Texture2D coinTexture;
    Texture2D spikeTexture;
    Texture2D wallTexture;
} MapTextures;

typedef struct Level {
    int requiredDistanceToNextLevel;
    float speed;
    int startedAt;
    float gravity;
    MapSection mapSections[TOTAL_SECTIONS];
    MapTextures mapTextures;
} Level;


// This value is used to draw the map more fluidly
float offsetX = 0;

MapSection emptyMap = { "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
                        "                              ",
                        "                              ",
                        "                              ",
                        "                              ",
                        "                              ",
                        "                              ",
                        "                              ",
                        "                              ",
                        "                              ",
                        "                              ",
                        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" };


int isValidChar(char c) {
    char validChars[] = "XCZ ";

    if (strchr(validChars, c) == NULL) {
        return 0;
    }

    return 1;

}

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

            if (!isValidChar(a)) {
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

void loadEmptyMap(MapSection map) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < SECTION_WIDTH; j++) {
            map[i][j] = emptyMap[i][j];
        }
    }
}

void loadMapRandomly(MapSection map, MapSection mapSections[TOTAL_SECTIONS]) {
    int randomSection = getRandIntBetween(0, TOTAL_SECTIONS - 1);

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

int moveMap(float speed, MapSection loadedSections[2]) {
    offsetX += speed;

    if (offsetX < 1) {
        return 0;
    }

    moveMapMatrix(loadedSections);
    offsetX--;

    return 1;
}

void drawCell(char c, int coordX, int coordY, MapTextures *mapTextures) {
    if (c == ' ' || !isValidChar(c)) {
        return;
    }

    int x = (coordX * CELL_SIZE) - (offsetX * CELL_SIZE);
    int y = coordY * CELL_SIZE;
    Texture2D texture;

    switch (c) {
        case 'Z':
            texture = mapTextures->spikeTexture;
            break;

        case 'C':
            texture = mapTextures->coinTexture;
            break;

        case 'X':
            texture = mapTextures->wallTexture;
            break;
    }

    Rectangle cellRectangle = {x, y, CELL_SIZE, CELL_SIZE};
    Rectangle sourceRectangle = {0, 0, texture.width, texture.height};
    Vector2 anchorPoint = {0, 0};

    DrawTexturePro(texture, sourceRectangle, cellRectangle, anchorPoint, 0, WHITE);
}

void drawMap(MapSection loadedMap[2], MapTextures *mapTextures) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < SECTION_WIDTH; j++) {
            drawCell(loadedMap[0][i][j], j, i, mapTextures);
        }
    }

    for (int i = 0; i < MAP_HEIGHT; i++) {
        drawCell(loadedMap[1][i][0], SECTION_WIDTH, i, mapTextures);
    }
}

int loadLevel(int levelNumber, Level *level, char errorMessage[ERROR_MESSAGE_LENGTH]) {
    int isMapRead = readMapFile(levelNumber, level->mapSections);
    level->startedAt = GetTime();

    if (!isMapRead) {
        snprintf(errorMessage, ERROR_MESSAGE_LENGTH, "ERROR: Error on load level %d \n", levelNumber);
        printf(errorMessage);
        return 0;
    }

    level->requiredDistanceToNextLevel = levelNumber == AMOUNT_OF_LEVELS ? INT_MAX : 100 * levelNumber;

    char coinPath[MAX_PATH_SIZE], spikePath[MAX_PATH_SIZE], wallPath[MAX_PATH_SIZE];

    snprintf(coinPath,  MAX_PATH_SIZE, "./resources/levels/%d/coin.png",   levelNumber);
    snprintf(spikePath, MAX_PATH_SIZE, "./resources/levels/%d/spike.png",  levelNumber);
    snprintf(wallPath,  MAX_PATH_SIZE, "./resources/levels/%d/wall.png",   levelNumber);

    MapTextures textures = {
        LoadTexture(coinPath),
        LoadTexture(spikePath),
        LoadTexture(wallPath)
    };

    level->mapTextures = textures;

    level->speed = 0.2 + (levelNumber - 1) * LEVEL_SPEED_MULTIPLIER;
    level->gravity = INITIAL_GRAVITY + (levelNumber - 1) * 0.05;

    printf("Level %d loaded successfully \n", levelNumber);

    return 1;
}

void unloadMapTextures(MapTextures *mapTextures) {
    UnloadTexture(mapTextures->coinTexture);
    UnloadTexture(mapTextures->spikeTexture);
    UnloadTexture(mapTextures->wallTexture);

    return;
}
