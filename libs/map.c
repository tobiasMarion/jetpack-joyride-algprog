#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_PATH_SIZE 30
#define MAP_HEIGHT 12
#define MAP_WIDTH 240
#define SECTION_WIDTH 30
#define TOTAL_SECTIONS MAP_WIDTH / SECTION_WIDTH

int readMapFile(int levelNumber, char map[TOTAL_SECTIONS][MAP_HEIGHT][SECTION_WIDTH]) {
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
            map[section][i][pos] = a;
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


void printMap(char map[TOTAL_SECTIONS][MAP_HEIGHT][SECTION_WIDTH]) {
    for (int section = 0; section < TOTAL_SECTIONS; section++) {
        printf("Section %d:\n", section + 1);
        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < SECTION_WIDTH; j++) {
                printf("%c", map[section][i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
}
