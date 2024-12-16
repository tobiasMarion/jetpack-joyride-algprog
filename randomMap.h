#ifndef RANDO_MAP
#define RANDO_MAP

#define LARGURA_TOTAL 240
#define ALTURA_TOTAL 12
#define LARGURA_TRECHO 30
#define TRECHOS 8


int divideMapa(int n, char mapa[TRECHOS][ALTURA_TOTAL][LARGURA_TRECHO]);
void imprimeMap(char mapa[TRECHOS][ALTURA_TOTAL][LARGURA_TRECHO]);

#endif // RANDO_MAP
