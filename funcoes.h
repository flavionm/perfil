#ifndef FUNCOES_H
#define FUNCOES_H

#define SIZE 256
#define NUMDICAS 20
#define DBSIZE 23
#define CLEAN while(getchar() != '\n')

typedef struct {
	char nome[SIZE];
	int vez;
	int pontuacao;
} PLAYER;

int quantPlayers(void);
void nomePlayers(PLAYER *lista, int numPlayers);
int gameLength(void);
void ordemJogo(PLAYER *lista, int numPlayers);
void escolheJogador(int *atual, int total);
void escolheQuestao(FILE *dados, int *usadas, int *atual);
void exibeCategoria(FILE *dados, int atual);
void exibeDica(FILE *dados, int dica);
int confereResposta(FILE *dados);
void exibeResposta(FILE *dados);
int contaPontos(PLAYER *lista, PLAYER *maquina, int dica, int jogador, int nPlayers, int mPontos);
void declaraVencedor(PLAYER *lista, PLAYER *maquina, int nPlayers, int mPontos, int vencedor);

#endif
