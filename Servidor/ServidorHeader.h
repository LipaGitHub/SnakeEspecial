
#include "DLLSnake\DLLSnake\dll_Header.h"
int iLinhas, iColunas;
void iniciaVarJogo() {
	dadosJ->linha = iLinhas;
	dadosJ->coluna = iColunas;
	for (int i = 0; i < NJogadores; i++) {
		dadosJ->aSerpente->id = -1;
		dadosJ->jog->pid = -1;
		dadosJ->jog->id_serpente = -1;
	}
}


void iniciaSerpente() {
	
	dadosJ->aSerpente[0].direcao = 's';

	dadosJ->aSerpente[0].tamanho = 5;
	dadosJ->aSerpente[0].conteudo = (Posicao *) malloc(sizeof(Posicao)*(dadosJ->aSerpente[0]).tamanho);
	dadosJ->aSerpente[0].morte = false;
	dadosJ->aSerpente[0].conteudo[0].x = 2; dadosJ->aSerpente[0].conteudo[0].y = 2;
	dadosJ->aSerpente[0].conteudo[1].x = 3; dadosJ->aSerpente[0].conteudo[1].y = 2;
	dadosJ->aSerpente[0].conteudo[2].x = 4; dadosJ->aSerpente[0].conteudo[2].y = 2;
	dadosJ->aSerpente[0].conteudo[3].x = 4; dadosJ->aSerpente[0].conteudo[3].y = 3;
	dadosJ->aSerpente[0].conteudo[4].x = 5; dadosJ->aSerpente[0].conteudo[4].y = 3;
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
	for (int i = 1; i < s->tamanho; i++){
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
	for (int i = 1; i < s->tamanho; i++){
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
	
	int px,py;
	
	if (s->direcao == 'w') {
		py = s->conteudo->y;
		px = s->conteudo->x;
		py--;
		if (dadosJ->mapaJogo[py][px].caracter == '1' || dadosJ->mapaJogo[py][px].caracter == '0') {
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
		if (dadosJ->mapaJogo[py][px].caracter == '1' || dadosJ->mapaJogo[py][px].caracter == '0') {
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
		if (dadosJ->mapaJogo[py][px].caracter == '1' || dadosJ->mapaJogo[py][px].caracter == '0') {
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
		
		if (dadosJ->mapaJogo[py][px].caracter == '1' || dadosJ->mapaJogo[py][px].caracter == '0') {
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
bool moverDirecao(Serpente *s,char t) {
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

	for (i = 0; i<L; i++) {
		for (j = 0; j<C; j++) {
			dadosJ->mapaJogo[i][j].y = i;
			dadosJ->mapaJogo[i][j].x = j;
			fscanf_s(f, "%c", &(dadosJ->mapaJogo[i][j].caracter));

		}
	}

	fclose(f);

	return;
}

void criaMapa() {
			int i, j;

		dadosJ->mapaJogo = (mapa *)malloc(sizeof(mapa)*(dadosJ->linha));
		dadosJ->mapaSerp = (mapa *)malloc(sizeof(mapa)*(dadosJ->linha));

		for (i = 0; i < dadosJ->linha; i++) {
			dadosJ->mapaJogo[i] = (mapa *)malloc(sizeof(mapa)*(dadosJ->coluna));
			dadosJ->mapaSerp[i] = (mapa *)malloc(sizeof(mapa)*(dadosJ->coluna));
		}


		for (i = 0; i < dadosJ->linha; i++) {
			for (j = 0; j < dadosJ->coluna; j++) {
				if (i == 0 || i == (dadosJ->linha - 1)) {
					dadosJ->mapaJogo[i][j].caracter = '1';
				}
				else if (j==0 || j ==(dadosJ->coluna -1)){
					dadosJ->mapaJogo[i][j].caracter = '0';
				}
				else
					dadosJ->mapaJogo[i][j].caracter = ' ';
				
			}
		}


		return;
	}


void addSerpente() {
	
	for (int i = 0; i < dadosJ->linha; i++) {
		for (int j = 0; j < dadosJ->coluna; j++) {
			dadosJ->mapaSerp[i][j].caracter = dadosJ->mapaJogo[i][j].caracter;
		}
	}
	for (int i = 0; i < NJogadores; i++) {
		if(dadosJ->aSerpente[i].tamanho>0)
			if ((dadosJ->aSerpente[i]).morte == false) {
				for (int k = 0; k < dadosJ->aSerpente[i].tamanho; k++) {
					dadosJ->mapaSerp[dadosJ->aSerpente[i].conteudo[k].y][dadosJ->aSerpente[i].conteudo[k].x].caracter = '3';
				}
			}
	}
	
}







