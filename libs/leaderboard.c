#include <stdio.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "constants.h"


typedef struct date {
    int year;
    int month;
    int day;
    int hour;
    int minute;
} Date;

typedef struct Save {
    char name[MAX_INPUT_CHARS + 1];
    int points;
    Date currentDate;
} Save;

void initializeSave (Save *save) {
    save->name[0] = '\0';
    save->points = 0;
    save->currentDate.year = 0;
    save->currentDate.month = 0;
    save->currentDate.day = 0;
    save->currentDate.hour = 0;
    save->currentDate.minute = 0;
}


void giveDate(Save *currentSave) {
    time_t t = time(NULL);

    struct tm date = *localtime(&t);

    currentSave->currentDate.year = date.tm_year + 1900;
    currentSave->currentDate.month = date.tm_mon + 1;
    currentSave->currentDate.day = date.tm_mday;
    currentSave->currentDate.hour= date.tm_hour;
    currentSave->currentDate.minute = date.tm_min;

}

void score(Save *save, Player *player) {
    int distance = player->distance;
    int coins = player->coins;

    save->points = (distance * 10) + coins;
}

int verifyName(Save *save) {

    if(save->name[0] == '\0') {
        return 0;   //Se o nome não foi escrito.
    }
    return 1;
}

void sortSaves(Save *allSave, int allSaveVectorSize) {
    Save temp;
    int i;
    int change = 1;

    while( change != 0) {
        change = 0;

        for(i = 0; i < allSaveVectorSize - 1; i++){

            if( allSave[i].points < allSave[i+1].points) {

                temp = allSave[i];
                allSave[i] = allSave[i+1];
                allSave[i+1] = temp;
                change++;
            }
        }
    }
}

int openFile(Save *vectorSaves) {
    FILE *file = fopen("saves/saves.bin", "r");
    if(file == NULL) { return 0;}
    int i;

    fseek(file, 0, SEEK_END);
    int fileSize = ftell(file) / sizeof(Save);
    rewind(file);

    int result = fread(vectorSaves, sizeof(Save), fileSize, file);
    fclose(file);

    return result; //Return quantidy of structs Save that was written in vectorSaves (use to pass as allSaveVectorSize parameter of saveGame function)
}


void saveFile(Save *vectorSaves, int vectorSize) {
    FILE *file = fopen("saves/saves.bin", "w");

    fwrite(vectorSaves, sizeof(Save), vectorSize, file);
    fclose(file);
}


//Parametros:
// 1) Um ponteiro para um tipo Save, nessa caso será o currentSave, declarado no Index.c.
// 2) Um ponteiro para uma string globalMessage, também declarado no index.c.
// 3) Um ponteiro para um vetor de Saves, também declarado no index.c.
// 4) Um ponteiro para um inteiro, também declarado no index.c
// Todos esse parametros serão passados para a função drawnSaveGameScreen, que de fato vai chamar a função saveGame.

void saveGame(Save *currentSave, char *globalMessage, Save *allsaves, int *allSaveVectorSize) {

    int result = verifyName(currentSave);
    giveDate(currentSave);
    if(result == 1) {

        if( *allSaveVectorSize == 0) {

            allsaves[*allSaveVectorSize] = *currentSave;
            (*allSaveVectorSize)++;
            strcpy(globalMessage, "Score saved!!");

        }else if(*allSaveVectorSize == MAX_SAVES)
        {
            if(allsaves[*allSaveVectorSize - 1].points < currentSave->points)
            {
                allsaves[*allSaveVectorSize - 1] = *currentSave;
                strcpy(globalMessage, "Score saved!!");

            }else
            {
                strcpy(globalMessage, "New score is not high enough to be saved, Git Gud!");
            }
        }else if( *allSaveVectorSize < MAX_SAVES)
        {
            allsaves[*allSaveVectorSize] = *currentSave;
            (*allSaveVectorSize)++;
            strcpy(globalMessage, "Score saved!!");

        }

        sortSaves(allsaves, *allSaveVectorSize);

    }

}
