#include "randomMap.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int divideMapa(int n, char mapa[TRECHOS][ALTURA_TOTAL][LARGURA_TRECHO])
{
    int inicio;
    int l,c,t;
    int inicios[8] = {0,30,60,90,120,150,180,210};
    char carac;
    FILE *arq = NULL;
    srand(time(NULL));

    switch(n)
    {
    case 1:
        arq = fopen("mapa1.txt", "r");
        if(arq == NULL){printf("Erro ao abrir o mapa!"); return 1;}
        break;
    case 2:
        arq = fopen("mapa2.txt", "r");
        if(arq == NULL){printf("Erro ao abrir o mapa!"); return 1;}
        break;
    case 3:
        arq = fopen("mapa3.txt", "r");
        if(arq == NULL){printf("Erro ao abrir o mapa!"); return 1;}
        break;
    default:
        printf("Mapa nao encontrado!");
    }

    for( t = 0; t < TRECHOS; t++)
    {
    inicio = inicios[rand() % TRECHOS];
    fseek(arq, inicio, SEEK_SET);

        for( l =0; l < ALTURA_TOTAL; l++)
        {
            for(c=0; c < LARGURA_TOTAL; c++)
            {
                carac = fgetc(arq);
                if( carac != '\n' && carac != '\0' )
                {
                    mapa[t][l][c] = carac;
                }else{c--;};
            }
        }
    }
    return 1;
}

void imprimeMap(char mapa[TRECHOS][ALTURA_TOTAL][LARGURA_TRECHO])
{
    int c,l,t;
    for(t = 0; t < TRECHOS; t++)
    {
        for(l = 0; l < ALTURA_TOTAL; l++)
        {
            for(c = 0; c < LARGURA_TRECHO; c++)
            {
                printf("%c", mapa[t][l][c]);
            }
            printf("\n");
        }
    }
}
