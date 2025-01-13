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

int openSavesFile(Save *vectorSaves) {
    FILE *file = fopen(LEADERBOARD_FILE_PATH, "r");
    if(file == NULL) { return 0;}
    int i;

    fseek(file, 0, SEEK_END);
    int fileSize = ftell(file) / sizeof(Save);
    rewind(file);

    int result = fread(vectorSaves, sizeof(Save), fileSize, file);
    fclose(file);

    return result; //Return quantidy of structs Save that was written in vectorSaves (use to pass as allSaveVectorSize parameter of saveGame function)
}


int saveSavesFile(Save *vectorSaves, int vectorSize) {
    FILE *file = fopen(LEADERBOARD_FILE_PATH, "w");

    if (file == NULL) {
        return 0;
    }

    fwrite(vectorSaves, sizeof(Save), vectorSize, file);
    fclose(file);
    return 1;
}


int saveGame(Save *currentSave, char *globalMessage, Save *allsaves, int allSaveVectorSize) {
    if (!verifyName(currentSave)) {
        return 0;
    }

    giveDate(currentSave);

    if(allSaveVectorSize < MAX_SAVES) {
        allsaves[allSaveVectorSize] = *currentSave;
        allSaveVectorSize++;
        strcpy(globalMessage, "Score saved!!");
        sortSaves(allsaves, allSaveVectorSize);

        return 1;
    }


    if (allsaves[allSaveVectorSize - 1].points > currentSave->points) {
        strcpy(globalMessage, "New score is not high enough to be saved, Git Gud!");

        return 0;
    }

    allsaves[allSaveVectorSize - 1] = *currentSave;
    strcpy(globalMessage, "Score saved!!");
    sortSaves(allsaves, allSaveVectorSize);

    return 1;
}
