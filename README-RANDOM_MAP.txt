BIBLIOTECA RANDOM_MAP

CONSTANTES DA BLIBLIOTECA:
#define LARGURA_TOTAL 240
#define ALTURA_TOTAL 12
#define LARGURA_TRECHO 30
#define TRECHOS 8

FUNÇÃO: divideMapa(int numero, char trecho[TRECHOS][ALTURA_TOTAL][LARGURA_TRECHO]);
1) Essa função recebe um número correspondente a um arquivo texto que contenha o mapa da fase no tamanho 12x240. Com esse número, ela procura o nome do mapa nos arquivos, o qual deve ser: mapa1.txt, mapa2.txt ou mapa3.txt.
    	- Exemplo: Se é passado numero = 1, ela busca o nome mapa1.txt.
    	OBS: Por enquanto, essa função aceita no máximo 3 mapas diferentes, mas podemos alterar essa quantidade conforme o número de fases que vamos implementar.
2) Com o mapa completo selecionado, a função lê os caracteres e salva em matrizes [12][30] que representa um trecho do mapa, no total são 8 dessas matrizes, as quais são armazenadas em um array tridimensional do tipo char com dimensões [8][12][30].
	- Essa função já salva as matrizes no array tridimensional de forma aleatória. 

Parâmetros:
	int numero: um numero de 1 à 3.
	char trecho: um array [8][12][30], onde serão salvos as matrizes 12x30.
		- Montei a função para receber o endereço do array, então ele tem que estar já declarado na nossa main.


FUNÇÃO: imprimeMap(char trecho[TRECHOS][ALTURA_TOTAL][LARGURA_TRECHO])
1) Basicamente imprime as matrizes contida no array tridimensional no terminal, fiz essa pra verificar o funcionamento da divideMapa.
		 
