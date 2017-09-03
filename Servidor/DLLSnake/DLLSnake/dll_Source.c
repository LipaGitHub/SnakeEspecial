#include "dll_Header.h"
Jogo *dadosJ;
HANDLE EventoSnake, EventoTecla;

TCHAR snake[] = TEXT("mpSnake"), nEvento[] = TEXT("eventoSnake"), nTecla[] = TEXT("Evento Tecla");

void imprimeMapa() {
	system("cls");
	int i, j;
	for (i = 0; i < dadosJ->linha; i++) {
		for (j = 0; j < dadosJ->coluna; j++) {
			if (dadosJ->mapaSerp[i][j].caracter == '1')
				_tprintf(TEXT("-"));
			else if (dadosJ->mapaSerp[i][j].caracter == '0')
				_tprintf(TEXT("*"));
			else if (dadosJ->mapaSerp[i][j].caracter == ' ')
				_tprintf(TEXT(" "));
			else if (dadosJ->mapaSerp[i][j].caracter == '3')
				_tprintf(TEXT("s"));
		}
		_tprintf(TEXT("\n"));
	}
}