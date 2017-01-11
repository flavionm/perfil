#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcoes.h"

int quantPlayers(void)
{
	int quant, check;
	
	printf("Insira a quantidade de jogadores participantes\nEntre 1 a 4 jogadores\n");
	check = scanf("%d", &quant);
	if(quant < 1 || quant > 4)
		check = 0;
	CLEAN;
	while(!check) {
		printf("Entrada invalida\n");
		printf("Entre 1 a 4 jogadores\n");
		check = scanf("%d", &quant);
		if(quant < 1 || quant > 4)
			check = 0;
		CLEAN;
	}
	
	return quant;
}

void nomePlayers(PLAYER *lista, int numPlayers)
{
	int i;
	
	for(i = 0; i < numPlayers; i++) {
		printf("\nInsira o nome do jogador %d\n", i + 1);
		fgets((lista+i)->nome, SIZE, stdin);
		if((lista+i)->nome[SIZE-2] != '\0') {
	 		(lista+i)->nome[SIZE-2] = '\n';
			(lista+i)->nome[SIZE-1] = '\0';
		}
	}
}

int gameLength(void)
{
	int length, check;
	
	printf("\nEscolha a duração do jogo\nRapido: 50 pontos para vencer\nNormal: 100 pontos para vencer\nLongo: 200 pontos para vencer\nEntre 1 para Rapido, 2 para Normal, 3 para Longo\n");
	check = scanf("%d", &length);
	if(length < 1 || length > 3)
		check = 0;
	CLEAN;
	while(!check) {
		printf("Entrada invalida\n");
		printf("Entre 1 para Rapido, 2 para Normal, 3 para Longo\n");
		check = scanf("%d", &length);
		if(length < 1 || length > 3)
			check = 0;
		CLEAN;
	}
	
	switch(length) {
		case 1:
			return 50;
		case 2:
			return 100;
		case 3:
			return 200;
	}
}

void ordemJogo(PLAYER *lista, int numPlayers)
{
	int i, j;
	PLAYER temp;
	
	srand(time(NULL));
	for(i = 0; i < numPlayers; i++)
		(lista+i)->vez = rand();
	
	for(i = 0; i < numPlayers; i++)
		for(j = 0; j < numPlayers - 1 - i; j++)
			if((lista+j)->vez < (lista+j+1)->vez) {
				temp = *(lista+j);
				*(lista+j) = *(lista+j+1);
				*(lista+j+1) = temp;
			}
	
	printf("\nOrdem de jogo\n");
	for(i = 0; i < numPlayers; i++) {
		printf("%s", (lista+i)->nome);
	}
}

void escolheJogador(int *atual, int total)
{
	(*atual)++;
	if(*atual == total)
		*atual = 0;
}

void escolheQuestao(FILE *dados, int *usadas, int *atual)
{
	int new = 1, i;
	
	do {
		srand(time(NULL));
		*atual = rand() % DBSIZE;
		for(i = 0; i < DBSIZE; i++)
			if(*(usadas+i) == *atual)
				new = 0;
	} while(!new);
	
	for(i = 0; i < DBSIZE; i++)
		if(*(usadas+i) == -1) {
			*(usadas+i) = *atual;
			break;
		}
	
	if(i == DBSIZE)
		for(i = 0; i < DBSIZE; i++)
			*(usadas+i) = -1;
}

void exibeCategoria(FILE *dados, int atual)
{
	int i;
	char *string, c;
	
	string = (char *) malloc(SIZE);
	if(!malloc) exit(1);
	
	fseek(dados, 23*SIZE*atual, SEEK_SET);
	
	fread(string, 1, SIZE, dados);

	printf("\nSou um(a) ");
	for(i = 0; ; i++) {
		if(*(string+i) == 13)
			break;
		putchar(*(string+i)-32);
	}
	printf("\n");
}

void exibeDica(FILE *dados, int dica)
{
	char string[SIZE];
	
	fseek(dados, dica*SIZE, SEEK_CUR);
	
	fread(string, 1, SIZE, dados);
	printf("\nDica n %d: ", dica+1);
	printf("%s", string);
	
	fseek(dados, -(dica+1)*SIZE, SEEK_CUR);
}

int confereResposta(FILE *dados)
{
	int i;
	char resposta[SIZE], string[SIZE];
	
	printf("\nResposta: ");
	fgets(resposta, SIZE, stdin);
	
	for(i = 0; i < SIZE; i++)
		if(resposta[i] > 96 && resposta[i] < 123)
			resposta[i] = resposta[i] - 32;
			
	fseek(dados, SIZE*NUMDICAS, SEEK_CUR);
	fread(string, 1, SIZE, dados);
	fseek(dados, -SIZE*(NUMDICAS+1), SEEK_CUR);
	
	for(i = 0; ; i++)
		if(*(string+i) == 13) {
			*(string+i) = '\n';
			*(string+i+1) = '\0';
			break;
		}
	
	if(strcmp(resposta, string) == 0) {
		printf("Resposta correta\n");
		return 1;
	}
	
	printf("Resposta errada\n");
	return 0;
}

void exibeResposta(FILE *dados)
{
	char string[SIZE];
	
	fseek(dados, SIZE*NUMDICAS, SEEK_CUR);
	fread(string, 1, SIZE, dados);
	fseek(dados, -SIZE*(NUMDICAS+1), SEEK_CUR);
	
	printf("\nResposta: %s", string);
}

int contaPontos(PLAYER *lista, PLAYER *maquina, int dica, int jogador, int nPlayers, int mPontos)
{
	int i, fim = -1;
	
	(lista+jogador)->pontuacao += 20 - dica;
	maquina->pontuacao += dica;
	
	printf("\nPlacar:\n");
	printf("Computador\n%d\n", maquina->pontuacao);
	if(maquina->pontuacao >= mPontos)
		fim = 4;
	for(i = 0; i < nPlayers; i++) {
		if((lista+i)->pontuacao >= mPontos)
			fim = i;
		printf("%s%d\n", (lista+i)->nome, (lista+i)->pontuacao);
	}
	
	return fim;
}

void declaraVencedor(PLAYER *lista, PLAYER *maquina, int nPlayers, int mPontos, int vencedor)
{
	if(vencedor == 4)
		printf("\nO vencedor é o Computador\n");
	else
		printf("\nO(A) vencedor(a) é %sParabens, %s", (lista+vencedor)->nome, (lista+vencedor)->nome);
}
