
#include "DLLSnake\DLLSnake\dll_Header.h"


Jogo dadosJ;
int iLinhas, iColunas, iSerpente, nObj, dObj, pObj, nAuto;
HANDLE objMap2, objMap3;
void iniciaVarJogo() {

	dadosJ.mapaJogo.nLinhas = iLinhas;
	dadosJ.mapaJogo.nColunas = iColunas;
	for (int i = 0; i < NJogadores; i++) {
		dadosJ.aSerpente[i].tamanho = iSerpente;
		dadosJ.jog[i].pid = -1;
		dadosJ.aSerpente[i].dono = -1;
		dadosJ.aSerpente[i].inverte = false;
		dadosJ.aSerpente[i].morte = false;
	}
	dadosJ.objetos = (Obj *)malloc(sizeof(Obj*)*nObj);
	for (int i = 0; i < nObj; i++) {
		dadosJ.objetos[i].duracao = dObj;
		dadosJ.objetos[i].id = i;
		dadosJ.objetos[i].probabilidade = pObj;
	}
}

void imprimeMapa_local() {
	int i, j;
	char auxC;

	//system(TEXT("cls"));
	
	for (i = 0; i < dadosJ.mapaJogo.nLinhas; i++) {
		for (j = 0; j < dadosJ.mapaJogo.nColunas; j++) {
			if (dadosJ.mapaJogo.conteudo[i][j].caracter == '1')
				_tprintf(TEXT("-"));
			else if (dadosJ.mapaJogo.conteudo[i][j].caracter == '0')
				_tprintf(TEXT("*"));
			else if (dadosJ.mapaJogo.conteudo[i][j].caracter == '2')
				_tprintf(TEXT(" "));
			else {
				auxC = dadosJ.mapaJogo.conteudo[i][j].caracter;
				_tprintf(TEXT("%c"),auxC);
			}
		}
		_tprintf(TEXT("\n"));
	}
}

void iniciaSerpente() {
	/*
	dadosJ->aSerpente[0].direcao = 's';

	dadosJ->aSerpente[0].tamanho = 5;
	dadosJ->aSerpente[0].conteudo = (Posicao *)malloc(sizeof(Posicao)*(dadosJ->aSerpente[0]).tamanho);
	dadosJ->aSerpente[0].morte = false;
	dadosJ->aSerpente[0].conteudo[0].x = 2; dadosJ->aSerpente[0].conteudo[0].y = 2;
	dadosJ->aSerpente[0].conteudo[1].x = 3; dadosJ->aSerpente[0].conteudo[1].y = 2;
	dadosJ->aSerpente[0].conteudo[2].x = 4; dadosJ->aSerpente[0].conteudo[2].y = 2;
	dadosJ->aSerpente[0].conteudo[3].x = 4; dadosJ->aSerpente[0].conteudo[3].y = 3;
	dadosJ->aSerpente[0].conteudo[4].x = 5; dadosJ->aSerpente[0].conteudo[4].y = 3;*/
}
/*
bool verSerpente(Serpente s) {
	int i = 0;
	Posicao aux, aux2;
	for (i = 1; i < s.tamanho; i++)
	{
		aux = s.conteudo[i - 1];
		aux2 = s.conteudo[i];
		if (aux.x - aux2.x < -1 || aux.x - aux2.x >1) {
			return false;
		}
		if (aux.y - aux2.y < -1 || aux.y - aux2.y>1) {
			return false;
		}
		if (aux.x - aux2.x == -1 && aux.y - aux2.y == -1 || aux.x - aux2.x == 1 && aux.y - aux2.y == -1 || aux.x - aux2.x == -1 && aux.y - aux2.y == 1 || aux.x - aux2.x == 1 && aux.y - aux2.y == 1) {
			return false;
		}
	}
	return true;
}
*/
bool moveDir(Serpente * s) {
	Posicao * aux;
	aux = (Posicao *)malloc(sizeof(Posicao)* s->tamanho);
	aux[0].x = s->conteudo[0].x + 1;
	aux[0].y = s->conteudo[0].y;
	for (int i = 1; i < s->tamanho; i++)
	{
		aux[i].x = s->conteudo[i - 1].x;
		aux[i].y = s->conteudo[i - 1].y;
	}

	for (int i = 0; i < s->tamanho; i++) {
		s->conteudo[i].x = aux[i].x;
		s->conteudo[i].y = aux[i].y;
	}
	return true;
}
bool moveEsq(Serpente* s) {
	Posicao * aux;
	aux = (Posicao *)malloc(sizeof(Posicao)* s->tamanho);
	aux[0].x = s->conteudo[0].x - 1;
	aux[0].y = s->conteudo[0].y;
	for (int i = 1; i < s->tamanho; i++)
	{
		aux[i].x = s->conteudo[i - 1].x;
		aux[i].y = s->conteudo[i - 1].y;
	}

	for (int i = 0; i < s->tamanho; i++) {
		s->conteudo[i].x = aux[i].x;
		s->conteudo[i].y = aux[i].y;
	}
	return true;
}
bool moveCima(Serpente* s) {
	Posicao * aux;
	aux = (Posicao *)malloc(sizeof(Posicao)* s->tamanho);
	aux[0].x = s->conteudo[0].x;
	aux[0].y = s->conteudo[0].y - 1;
	for (int i = 1; i < s->tamanho; i++) {
		aux[i].x = s->conteudo[i - 1].x;
		aux[i].y = s->conteudo[i - 1].y;
	}

	for (int i = 0; i < s->tamanho; i++) {
		s->conteudo[i].x = aux[i].x;
		s->conteudo[i].y = aux[i].y;
	}
	return true;
}
bool moveBaixo(Serpente* s) {
	Posicao * aux;
	aux = (Posicao *)malloc(sizeof(Posicao)* s->tamanho);
	aux[0].x = s->conteudo[0].x;
	aux[0].y = s->conteudo[0].y + 1;
	for (int i = 1; i < s->tamanho; i++) {
		aux[i].x = s->conteudo[i - 1].x;
		aux[i].y = s->conteudo[i - 1].y;
	}

	for (int i = 0; i < s->tamanho; i++) {
		s->conteudo[i].x = aux[i].x;
		s->conteudo[i].y = aux[i].y;
	}
	return true;
}
/*
void mostraCobra(Serpente sAux) {
	bool ver;
	for (int i = 0; i < C; i++) {
		for (int j = 0; j < L; j++) {
			ver = false;
			for (int k = 0; k < sAux.tamanho; k++) {
				if (sAux.conteudo[k].x == j && sAux.conteudo[k].y == i) {
					ver = true;
				}
			}
			if (ver) {
				_tprintf(TEXT("O"));
			}
			else {
				_tprintf(TEXT(" "));
			}
		}
		_tprintf(TEXT("\n"));
	}
}
*/
void mover(Serpente * s) { // Alterar pra JOGO!!!!!!!

	int px, py;

	if (s->direcao == 'w') {
		py = s->conteudo->y;
		px = s->conteudo->x;
		py--;
		if (dadosJ.mapaJogo.conteudo[py][px].caracter == '1' || dadosJ.mapaJogo.conteudo[py][px].caracter == '0') {
			_tprintf(TEXT("Parede"));

			s->morte = true;
		}
		else {
			moveCima(s);
		}
	}
	else if (s->direcao == 's') {
		py = s->conteudo->y;
		px = s->conteudo->x;
		py++;
		if (dadosJ.mapaJogo.conteudo[py][px].caracter == '1' || dadosJ.mapaJogo.conteudo[py][px].caracter == '0') {
			s->morte = true;
		}
		else {
			moveBaixo(s);
		}
	}
	else if (s->direcao == 'a') {
		py = s->conteudo->y;
		px = s->conteudo->x;
		px--;
		if (dadosJ.mapaJogo.conteudo[py][px].caracter == '1' || dadosJ.mapaJogo.conteudo[py][px].caracter == '0') {
			s->morte = true;
		}
		else {
			moveEsq(s);
		}
	}
	else if (s->direcao == 'd') {
		py = s->conteudo->y;
		px = s->conteudo->x;
		px++;

		if (dadosJ.mapaJogo.conteudo[py][px].caracter == '1' || dadosJ.mapaJogo.conteudo[py][px].caracter == '0') {
			s->morte = true;
		}
		else {
			moveDir(s);
		}
	}
	SetEvent(EventoSnake);
}

char inverte(char t) {
	if (t == 'w') {
		return 's';
	}
	else if (t == 's') {
		return 'w';
	}
	else if (t == 'a') {
		return 'd';
	}
	else if (t == 'd') {
		return 'a';
	}
}
bool moverDirecao(Serpente *s, char t) {
	t = tolower(t);
	if (s->direcao == inverte(t)) {
		return false;
	}
	s->direcao = t;
	return true;
}


void LeMapa() {
	FILE *f;
	int i, j;

	f = fopen(ficheiro, "rt");

	if (f == NULL) {
		printf("Erro!\n");
		fclose(f);
	}

	for (i = 0; i < L; i++) {
		for (j = 0; j < C; j++) {
			dadosJ.mapaJogo.conteudo[i][j].y = i;
			dadosJ.mapaJogo.conteudo[i][j].x = j;
			fscanf_s(f, "%c", &(dadosJ.mapaJogo.conteudo[i][j].caracter));

		}
	}

	fclose(f);

	return;
}

void criaMapa() {
	int x, y;
	char c = 'a';
	srand(time(NULL));   // should only be called once
	int r = rand();
	int i, j, calcX, calcY, auxX, dir;
	double areaDisp;

	for (i = 0; i < dadosJ.mapaJogo.nLinhas; i++) {
		for (j = 0; j < dadosJ.mapaJogo.nColunas; j++) {
			if (i == 0 || i == (dadosJ.mapaJogo.nLinhas - 1)) {
				dadosJ.mapaJogo.conteudo[i][j].caracter = '1';
			}
			else if (j == 0 || j == (dadosJ.mapaJogo.nColunas - 1)) {
				dadosJ.mapaJogo.conteudo[i][j].caracter = '0';
			}
			else
				dadosJ.mapaJogo.conteudo[i][j].caracter = '2';
		}
	}
	if (nAuto != 0) {

		calcX = dadosJ.mapaJogo.nColunas - 2;
		calcY = dadosJ.mapaJogo.nLinhas - 2;

		areaDisp = (calcY - 2) / nAuto;
		areaDisp = trunc(areaDisp);

		for (i = 0; i < nAuto; i++) {
			r = rand() % (calcX - 2 - iSerpente) + 2;
			dadosJ.aSerpente[i].conteudo = (Posicao*)malloc(sizeof(Posicao)*iSerpente);
			dadosJ.aSerpente[i].dono = -2;

			dir = rand() * 4;
			if (dir == 0) {
				dadosJ.aSerpente[i].direcao = "w";
			}
			else if (dir == 1) {
				dadosJ.aSerpente[i].direcao = "a";
			}
			else if (dir == 2) {
				dadosJ.aSerpente[i].direcao = "s";
			}
			else if (dir == 3) {
				dadosJ.aSerpente[i].direcao = "d";
			}

			auxX = areaDisp;
			for (j = 0; j < iSerpente; j++) {
				dadosJ.aSerpente[i].conteudo[j].x = r;
				dadosJ.aSerpente[i].conteudo[j].y = areaDisp*(i + 1); // erradoooooooo
				r++;
				_tprintf(TEXT("(%d,%d)"),dadosJ.aSerpente[i].conteudo[j].x, dadosJ.aSerpente[i].conteudo[j].y);
			}

		}
	}

	for (i = 0; i < nAuto; i++) {
		
		for (j = 0; j < iSerpente; j++) {
			x = dadosJ.aSerpente[i].conteudo[j].x;
			y = dadosJ.aSerpente[i].conteudo[j].y;
			dadosJ.mapaJogo.conteudo[x][y].caracter = c;

		}
		c++;
	}
	return;
}


void addSerpente() {
	/*
	for (int i = 0; i < dadosJ.mapaSerp.nLinhas; i++) {
		for (int j = 0; j < dadosJ.mapaSerp.nColunas; j++) {
			dadosJ->mapaSerp.conteudo[i][j].caracter = dadosJ->mapaJogo.conteudo[i][j].caracter;
		}
	}
	for (int i = 0; i < NJogadores; i++) {
		if (dadosJ->aSerpente[i].tamanho > 0)
			if ((dadosJ->aSerpente[i]).morte == false) {
				for (int k = 0; k < dadosJ->aSerpente[i].tamanho; k++) {
					dadosJ->mapaSerp.conteudo[dadosJ->aSerpente[i].conteudo[k].y][dadosJ->aSerpente[i].conteudo[k].x].caracter = '3';
				}
			}
	}*/

}







