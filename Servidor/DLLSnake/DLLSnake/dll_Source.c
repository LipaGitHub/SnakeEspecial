#include "dll_Header.h"
Mapa * vistaFinal;
HANDLE EventoSnake, EventoTecla;

TCHAR snake[] = TEXT("mpSnake"), nEvento[] = TEXT("eventoSnake");

void imprimeMapa() {
	system("cls");
	int i, j;
	for (i = 0; i < vistaFinal->nLinhas; i++) {
		for (j = 0; j < vistaFinal->nColunas; j++) {
			if (vistaFinal->conteudo[i][j].caracter == '1')//parede superior e inferior
				_tprintf(TEXT("-"));
			else if (vistaFinal->conteudo[i][j].caracter == '0')// parede lateral
				_tprintf(TEXT("*"));
			else if (vistaFinal->conteudo[i][j].caracter == '2') //espaço vazio
				_tprintf(TEXT(" "));
			else if (vistaFinal->conteudo[i][j].caracter == '3')//Comida
				_tprintf(TEXT("0"));
			else if (vistaFinal->conteudo[i][j].caracter == '4')//Gelo
				_tprintf(TEXT("1"));
			else if (vistaFinal->conteudo[i][j].caracter == '5')//Granada
				_tprintf(TEXT("2"));
			else if (vistaFinal->conteudo[i][j].caracter == '6')//Vodka
				_tprintf(TEXT("3"));
			else if (vistaFinal->conteudo[i][j].caracter == '7')//Óleo
				_tprintf(TEXT("4"));
			else if (vistaFinal->conteudo[i][j].caracter == '8')//Cola
				_tprintf(TEXT("5"));
			else if (vistaFinal->conteudo[i][j].caracter == '9')//O-Vodka
				_tprintf(TEXT("6"));
			else if (vistaFinal->conteudo[i][j].caracter == 'v')//O-Óleo
				_tprintf(TEXT("7"));
			else if (vistaFinal->conteudo[i][j].caracter == 'u')//O-Cola
				_tprintf(TEXT("8"));
			else if (vistaFinal->conteudo[i][j].caracter == 'w')//Café
				_tprintf(TEXT("9"));
			else
				_tprintf(vistaFinal->conteudo[i][j].caracter);//Cobra
		}
		_tprintf(TEXT("\n"));
	}
}