#include "dll_Header.h"
Mapa * vistaFinal;
HANDLE EventoSnake, EventoTecla;

TCHAR snake[] = TEXT("mpSnake"), nEvento[] = TEXT("eventoSnake");

void imprimeMapa() {
	system("cls");
	int i, j;
	for (i = 0; i < vistaFinal->nLinhas; i++) {
		for (j = 0; j < vistaFinal->nColunas; j++) {
			if (vistaFinal->conteudo[i][j].caracter == '1')
				_tprintf(TEXT("-"));
			else if (vistaFinal->conteudo[i][j].caracter == '0')
				_tprintf(TEXT("*"));
			else if (vistaFinal->conteudo[i][j].caracter == ' ')
				_tprintf(TEXT(" "));
			else if (vistaFinal->conteudo[i][j].caracter == '3')
				_tprintf(TEXT("s"));
		}
		_tprintf(TEXT("\n"));
	}
}