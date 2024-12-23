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
#define CELL_SIZE 48

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
            printf("Erro: esperado caractere de nova linha após a linha %d.\n", i + 1);
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

void moveMap(MapSection loadedSections[2]) {
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

void drawMap(MapSection map) {
    int x, y;
    Color color;


    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < SECTION_WIDTH; j++) {
            x = j * CELL_SIZE;
            y = i * CELL_SIZE;

            if (map[i][j] != ' ') {
                switch (map[i][j]) {
                    case 'X':
                        color = BLACK;
                        break;
                    case 'C':
                        color = YELLOW;
                        break;
                    case 'Z':
                        color = RED;
                        break;
                }

                DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, color);
            }
        }
    }
}


