#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"

void inicioJogo(FILE *dados, PLAYER *lista, int numPlayers, int maxPontos);

int main(void)
{
	FILE *database;
	PLAYER *jogadores;
	int numJogadores, pontuacaoMaxima;
	
	numJogadores = quantPlayers();
	
	jogadores = (PLAYER *) malloc(numJogadores*sizeof(PLAYER));
	if(!jogadores) return 1;
	
	nomePlayers(jogadores, numJogadores);
	
	pontuacaoMaxima = gameLength();
	
	ordemJogo(jogadores, numJogadores);
	
	database = fopen("bancoDeDados", "r");
	if(!database) return 1;
	
	inicioJogo(database, jogadores, numJogadores, pontuacaoMaxima);
	
	fclose(database);
	return 0;
}

void inicioJogo(FILE *dados, PLAYER *lista, int numPlayers, int maxPontos)
{
	int i, qUsadas[DBSIZE], qAtual, dAtual, jAtual = -1, jTurno, resposta, gameOver;
	PLAYER computador;char nome[SIZE] = "Computador\n";
	
	for(i = 0; i < DBSIZE; i++)
		qUsadas[i] = -1;
	
	for(i = 0; i < numPlayers; i++)
		(lista+i)->pontuacao = 0;
	computador.pontuacao = 0;
	
	for( ; ; ) {
		escolheJogador(&jAtual, numPlayers);
		jTurno = jAtual;
		escolheQuestao(dados, qUsadas, &qAtual);
		exibeCategoria(dados, qAtual);
		dAtual = 0;
		do {
			printf("\nVez de %s", (lista+jTurno)->nome);
			exibeDica(dados, dAtual);
			resposta = confereResposta(dados);
			if(resposta == 1)
				break;
			else if(dAtual == 19) {
				dAtual++;
				exibeResposta(dados);
				break;
			}
			escolheJogador(&jTurno, numPlayers);
			dAtual++;
		} while(1);	
		gameOver = contaPontos(lista, &computador, dAtual, jTurno, numPlayers, maxPontos);
		if(gameOver != -1) {
			declaraVencedor(lista, &computador, numPlayers, maxPontos, gameOver);
			break;
		}
	}
}
