#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "constants.h"

typedef struct Save {
    char name[MAX_INPUT_CHARS + 1];
    int points;
    int timestamp;
} Save;

void initializeSave (Save *save) {
    save->name[0] = '\0';
    save->points = 0;
    save->timestamp = 0;
}

void points(Save *save, Player *player) {
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

int sortSaves(Save *save, Save allSave[]) {
    //Implementação do algoritmo de ordenamaento.
}

//função que abre o binario.

//função que salva o vetor no binario.

void saveGame(Save *save, char *globalMessage, Save *allsaves[MAX_SAVES]) {

    int result = verifyName(save);
    Save allSaves[MAX_SAVES];

    if(result) {
            //1) Colocar o save atual no vetor
            //2) Ordenar o vetor allSaves por pontuação.
            //5) Se tudo der certo, atribui a mensagem de sucesso na mensagem global. Se não, atribui a mensagem de erro.
    }else {
        strncpy(globalMessage, "Write a name to save game!", 100);

    }



}


/*


void saveGame(char *name, Player *player) {
    int result = verifyName(name, player);

    switch(result) {

        case 0:
            DrawText("Error opening bin file!",
                     (SCREEN_WIDTH - MeasureText("Error opening bin file!", 40)) / 2,
                     680,
                     40,
                     DARKGREEN);
        break;

        case 1:
            FILE *file = fopen("save/saves.bin", "a");
            if(fwrite(&player, sizeof(Player), 1, file)) {
                DrawText("Saved successfuly!",
                         (SCREEN_WIDTH - MeasureText("Saved successfuly!", 40)) / 2,
                         680,
                         40,
                         DARKGREEN);
            }
            fclose(file);
        break;

        case 2:
            DrawText("This name alredy has been saved! Choose another!",
                     (SCREEN_WIDTH - MeasureText("This name alredy has been saved! Choose another!", 40)) / 2,
                     680,
                     40,
                     RED);
        break;

        case 3:
            DrawText("You must write a name to save game!",
                     (SCREEN_WIDTH - MeasureText("You must write a name to save game!", 40)) / 2,
                     680,
                     40,
                     RED);
        break;

        default: break;
    }
}*/

