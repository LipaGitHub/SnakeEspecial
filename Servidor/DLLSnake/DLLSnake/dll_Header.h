

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
#define L 20
#define C 25
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

	extern TP_SO2_DLL_API TCHAR snake[], nEvento[] , nTecla[] ;


	typedef struct {
		int x, y;
	} Posicao;

	typedef struct {
		int y, x;
		char caracter;
	}mapa;

	typedef struct {
		int id, tamanho, velocidade, durEfeito;
		char direcao;
		bool inverte, morte;
		Posicao *conteudo;
	}Serpente;



	typedef struct {
		int pid, pontuacao, id_serpente;
		char user[50];

	}Jogador;

	typedef struct {
		DWORD pid;
		TCHAR username[50];
		LPTSTR tecla;
	}Envio;

	typedef struct {
		int id, duracao, probabilidade;
		Posicao localizacao;
	}Obj;

	typedef struct {
		int linha, coluna;
		mapa **mapaJogo, **mapaSerp;
		Serpente aSerpente[NJogadores];
		Jogador jog[NJogadores];
		Obj * objetos;

	}Jogo;

	extern TP_SO2_DLL_API Jogo *dadosJ;

	TP_SO2_DLL_API void imprimeMapa();

