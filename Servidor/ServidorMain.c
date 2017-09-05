#include "ServidorHeader.h"
#define BUFSIZE 512
HHOOK hook;
KBDLLHOOKSTRUCT kbdStruct;

HANDLE objMap1, f1;
BOOL fConnected = FALSE;
DWORD dwThreadId = 0,sbToWrite, sbWriten;
HANDLE hPipe = INVALID_HANDLE_VALUE, hThread = NULL;
LPSTR lpszPipename = (TEXT("\\\\.\\pipe\\pipe20"));

DWORD WINAPI ThreadMoveSerpente(LPVOID param);
DWORD WINAPI ThreadAtendeCliente(LPVOID param);

void valInicial() {
	_tprintf(TEXT("Qual o tamanho de Linhas do Mapa?\n"));
	_tscanf_s(TEXT("%d"), &iLinhas);
	_tprintf(TEXT("Qual o tamanho de Colunas do Mapa?\n"));
	_tscanf_s(TEXT("%d"), &iColunas);
	_tprintf(TEXT("Qual o tamanho inicial das Serpentes?\n"));
	_tscanf_s(TEXT("%d"), &iSerpente);
	_tprintf(TEXT("Qual o numero de Serpentes Automáticas?\n"));
	_tscanf_s(TEXT("%d"), &nAuto);
	_tprintf(TEXT("Quantos Objetos Pretende?\n"));
	_tscanf_s(TEXT("%d"), &nObj);
	_tprintf(TEXT("Qual a duração dos Objetos?\n"));
	_tscanf_s(TEXT("%d"), &dObj);
	_tprintf(TEXT("Qual a Probabilidade de Ocorrência?\n"));
	_tscanf_s(TEXT("%d"), &pObj);

}

int logar(Ident * msg) {
	for (int i = 0; i < NJogadores; i++) {
		if (dadosJ.jog[i].pid == -1) {
			dadosJ.jog[i].pid = msg->pid;
			dadosJ.aSerpente[i].dono = i;
			_tcscpy(dadosJ.jog[i].user, msg->user);
			return i;
			break;
		}
	}
	return -1;

}

void criaRecursos() {
	LARGE_INTEGER tam;
	EventoSnake = CreateEvent(NULL, TRUE, FALSE, nEvento);

	//EventoTecla= OpenEvent(EVENT_ALL_ACCESS, false, nTecla);
	//Abrir Ficheiro f1 leitura
	/*f1 = CreateFile(ficheiro, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (f1 == INVALID_HANDLE_VALUE) {
	_tprintf(TEXT("[Erro] Abrir ficheiro (%d)\n"), GetLastError());
	return -1;
	}*/
	//Saber o tamanho do ficheiro 1 em bytes
	//GetFileSizeEx(f1, &tam);
	//Criar um objecto para o mapeamento para o ficheiro aberto
	/* Memoria Partilhada
	objMap1 = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Jogo), snake);

	if (objMap1 == NULL) {
		_tprintf(TEXT("[Erro]Criar objectos mapeamentos(%d)\n"), GetLastError());

	}

	dadosJ = (Jogo *)MapViewOfFile(objMap1, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(Jogo));

	if (dadosJ == NULL) {
		_tprintf(TEXT("[Erro]Mapear para memória(%d)\n"), GetLastError());
		CloseHandle(objMap1);

	}
	_tprintf(TEXT("Criação do Pipe Para os CLientes"));


	*/
	/*

	objMap2 = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Mapa)*(dadosJ->mapaSerp.nLinhas), TEXT("nLinhas"));

	if (objMap2 == NULL) {
	_tprintf(TEXT("[Erro]Criar objectos mapeamentos(%d)\n"), GetLastError());

	}

	dadosJ->mapaSerp = (mapa *)MapViewOfFile(objMap2, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(mapa)*(dadosJ->linha));

	if (dadosJ->mapaSerp == NULL) {
	_tprintf(TEXT("[Erro]Mapear para memória(%d)\n"), GetLastError());
	CloseHandle(objMap2);

	}
	dadosJ->mapaJogo = (mapa *)malloc(sizeof(mapa)*(dadosJ->linha));

	for (i = 0; i < dadosJ->linha; i++) {
	dadosJ->mapaJogo[i] = (mapa *)malloc(sizeof(mapa)*(dadosJ->coluna));
	objMap3 = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(mapa)*(dadosJ->coluna), TEXT("nColunas"));

	if (objMap3 == NULL) {
	_tprintf(TEXT("[Erro]Criar objectos mapeamentos(%d)\n"), GetLastError());

	}

	dadosJ->mapaSerp[i] = (mapa *)MapViewOfFile(objMap3, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(mapa)*(dadosJ->coluna));

	if (dadosJ->mapaSerp[i] == NULL) {
	_tprintf(TEXT("[Erro]Mapear para memória(%d)\n"), GetLastError());
	CloseHandle(objMap3);

	}
	}
	*/
	iniciaVarJogo();

	return 0;

}

//Funçao para libertar recursos

/*
//FREE
UnmapViewOfFile(map1);

//Apagar Objetos
CloseHandle(objMap1);
CloseHandle(f1);
*/

int _tmain(int argc, TCHAR ** argv[]) {
	DWORD threadId;
	HANDLE hThreadMoveSerp, hThreadRecebe;
#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif
	int i = 0, j = 0, k = 0;
	bool ver;

	FILE *f;
	/*
		//Mapeamento();

		f = fopen(ficheiro, "rt");

		if (f == NULL) {
			_tprintf(TEXT("Erro na abertura do ficheiro de texto"));
			return -1;
		}
		*/
	valInicial();
	//Criar Recursos de SO2
	criaRecursos();
	criaMapa();
	iniciaSerpente();



	hThreadMoveSerp = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadMoveSerpente, NULL, 0, &threadId);

	if (hThreadMoveSerp == NULL) {
		_tprintf(TEXT("Erro ao criar Thread\n"));
		return -1;
	}



	while (1) {

		
		hPipe = CreateNamedPipe(lpszPipename, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, BUFSIZE, BUFSIZE, 0, NULL);
		if (hPipe == INVALID_HANDLE_VALUE) {
			_tprintf(TEXT("Criação do namedPipe Falhou erro %d"), GetLastError());
			return -1;
		}
		fConnected = ConnectNamedPipe(hPipe, NULL);
		if (fConnected) {
			_tprintf(TEXT("Vou criar uma thread para o cliente"));
			hThread = CreateThread(NULL, 0, ThreadAtendeCliente, (LPVOID)hPipe, 0, &dwThreadId);
			if (hThread == NULL) {
				_tprintf(TEXT("ERROR NA CRIAÇÂO DA THREAD ERRO %d\n"), GetLastError());
				return -1;
			}
			else
				CloseHandle(hThread);

		}
		else
			CloseHandle(hPipe);

	}

	

	WaitForSingleObject(hThreadMoveSerp, INFINITE);
	return 0;
}

DWORD WINAPI ThreadMoveSerpente(LPVOID param) {
	do {
		for (int i = 0; i < NJogadores; i++)
			if (dadosJ.aSerpente[i].tamanho > 0) {
				mover(&dadosJ.aSerpente[i]);
			}

		addSerpente();
		imprimeMapa();
		Sleep(1000);
	} while (1);
}

DWORD WINAPI ThreadAtendeCliente(LPVOID param) {
	char direcaoTeste;
	HANDLE hHeap = GetProcessHeap();
	DWORD cbToWrite = sizeof(Envio);
	DWORD cbToRead = sizeof(Ident);
	Envio * pchRequest = (Envio *)HeapAlloc(hHeap, 0, cbToWrite);
	Ident * msg = (Envio*)HeapAlloc(hHeap, 0, cbToRead);
	DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWriten = 0;
	BOOL fSucess = FALSE;
	HANDLE hPipe_1 = NULL;
	int numJ, idSerpCli;
	bool startGame = true;
	if (param == NULL) {
		_tprintf(TEXT("\nERRO - o Handle enviado no param é nulo \n"));
		if (pchRequest != NULL)
			HeapFree(hHeap, 0, pchRequest);
		return (DWORD)-1;
	}
	if (pchRequest == NULL) {
		_tprintf("Não houve Memória");
		return (DWORD)-1;
	}

	_tprintf(TEXT("THREAD do servidor - a receber Mensagens\n\n\n"));
	hPipe_1 = (HANDLE)param;
	while (1) {
		if (startGame) {

			fSucess = ReadFile(hPipe_1, numJ, sizeof(numJ), &cbBytesRead, NULL);
			if (!fSucess || cbBytesRead == 0) {
				if (GetLastError() == ERROR_BROKEN_PIPE)
					_tprintf(TEXT("Cliente Desligou-se. ERRO = %d"), GetLastError());
				else
					_tprintf(TEXT("ReadFile falhou. ERRO = %d"), GetLastError());
				break;
			}

			fSucess = ReadFile(hPipe_1, msg, cbToRead, &cbBytesRead, NULL);

			if (!fSucess || cbBytesRead == 0) {
				if (GetLastError() == ERROR_BROKEN_PIPE)
					_tprintf(TEXT("Cliente Desligou-se. ERRO = %d"), GetLastError());
				else
					_tprintf(TEXT("ReadFile falhou. ERRO = %d"), GetLastError());
				break;
			}
			idSerpCli = logar(msg);
			if (idSerpCli != -1) {
				/* Envia Cliente*/
				sbToWrite = sizeof(idSerpCli);
				if (!WriteFile(hPipe_1, &idSerpCli, sbToWrite, &sbWriten, NULL)) {
					_tprintf(TEXT("WriteFile falhou.Erro =%d"), GetLastError());
					//exit(-1);
				}
				else {
					_tprintf(TEXT("cbWriten =%d"), cbWriten);
				}
			}
			if (numJ == 2) {
				fSucess = ReadFile(hPipe_1, msg, cbToRead, &cbBytesRead, NULL);

				if (!fSucess || cbBytesRead == 0) {
					if (GetLastError() == ERROR_BROKEN_PIPE)
						_tprintf(TEXT("Cliente Desligou-se. ERRO = %d"), GetLastError());
					else
						_tprintf(TEXT("ReadFile falhou. ERRO = %d"), GetLastError());
					break;
				}
				idSerpCli = logar(msg);
				if (idSerpCli != -1) {
					/* Envia Cliente*/
				}
			}
			startGame = false;
		}
		else {
			fSucess = ReadFile(hPipe_1, pchRequest, cbToWrite, &cbBytesRead, NULL);
			if (!fSucess || cbBytesRead == 0) {
				if (GetLastError() == ERROR_BROKEN_PIPE)
					_tprintf(TEXT("Cliente Desligou-se. ERRO = %d"), GetLastError());
				else
					_tprintf(TEXT("ReadFile falhou. ERRO = %d"), GetLastError());
				break;
			}
			//_tprintf(TEXT("%d Read \n %d Writen"), cbBytesRead, cbToWrite);

			//_tprintf(TEXT("%s"), pchRequest);
			//aqui processar os dados

			if (!_tcscmp(pchRequest->tecla, _T("a"))) {
				direcaoTeste = 'a';
			}
			else if (!_tcscmp(pchRequest->tecla, _T("s"))) {
				direcaoTeste = 's';
			}
			else if (!_tcscmp(pchRequest->tecla, _T("d"))) {
				direcaoTeste = 'd';
			}
			else if (!_tcscmp(pchRequest->tecla, _T("w"))) {
				direcaoTeste = 'w';
			}
			
					moverDirecao(&dadosJ.aSerpente[pchRequest->idSerp], direcaoTeste);
			
			//_tprintf(TEXT("%s"),pchRequest);

			//PROCESSAR OS DADOS ENVIADOS
			/*fSucess = WriteFile(hPipe, pchReply, cbReplyBytes, &cbWriten, NULL);
			if (!fSucess || cbReplyBytes != cbWriten) {
			_tprintf(TEXT("WRITEFILE FALHOU,ERRO= %d"),GetLastError());
			break;
			}*/
			FlushFileBuffers(hPipe_1);
		}
	}
	DisconnectNamedPipe(hPipe_1);
	CloseHandle(hPipe_1);

	HeapFree(hHeap, 0, pchRequest);
	_tprintf(TEXT("Thread a termninar \n"));
	return 1;
}