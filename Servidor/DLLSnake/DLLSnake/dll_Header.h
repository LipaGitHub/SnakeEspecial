

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#define ficheiro "mapa.txt"
#define NJogadores 20
#define L 100
#define C 100
//Este .h deve ser incluído no projeto que o vai usar (modo implícito)
#define TAM 256
//Esta macro é definida pelo sistema caso estejamos na DLL (<DLL_IMP>_EXPORTS definida)
//ou na app (<DLL_IMP>_EXPORTS não definida) onde DLL_IMP é o nome deste projeto

#ifdef DLLSNAKE_EXPORTS

#define TP_SO2_DLL_API __declspec(dllexport) 
#else
#define TP_SO2_DLL_API __declspec(dllimport)
#endif



extern TP_SO2_DLL_API HANDLE EventoSnake, EventoTecla;

extern TP_SO2_DLL_API TCHAR snake[], nEvento[], nColuna[], nLinha[];


typedef struct {
	int x, y;
} Posicao;

typedef struct {
	int y, x;
	char caracter;
}Celula;

typedef struct {
	Celula conteudo[L][C];
	int nLinhas, nColunas;
}Mapa;

typedef struct {
	int tamanho, velocidade, durEfeito, dono;
	char direcao;
	bool inverte, morte;
	Posicao *conteudo;
}Serpente;

typedef struct {
	int pid, pontuacao;
	char user[50];
}Jogador;

typedef struct {
	int pid;
	TCHAR user[50];
}Ident;

typedef struct {
	TCHAR username[50];
	LPTSTR tecla;
	int idSerp;
}Envio;

typedef struct {
	int id, duracao, probabilidade;
	Posicao localizacao;
}Obj;

typedef struct {
	Mapa mapaJogo;
	Serpente aSerpente[NJogadores];
	Jogador jog[NJogadores];
	Obj *objetos; // S estiver em memoria partilhada pode ser acedido se for malloc não

}Jogo;

extern TP_SO2_DLL_API Mapa *vistaFinal;

TP_SO2_DLL_API void imprimeMapa();

