#include "ServidorHeader.h"
#define BUFSIZE 512
HHOOK hook;
KBDLLHOOKSTRUCT kbdStruct;

Serpente *sJogo;
Obj *oJogo;
Jogador cJogo[NJogadores];
HANDLE objMap1, f1;
BOOL fConnected = FALSE;
DWORD dwThreadId = 0;
HANDLE hPipe = INVALID_HANDLE_VALUE, hThread = NULL;
LPSTR lpszPipename = (TEXT("\\\\.\\pipe\\pipe20"));

DWORD WINAPI ThreadMoveSerpente(LPVOID param);
DWORD WINAPI ThreadAtendeCliente(LPVOID param);

void valInicial() {
	_tprintf(TEXT("Qual o tamanho de Linhas\n"));
	_tscanf_s(TEXT("%d"), &iLinhas);
	_tprintf(TEXT("Qual o tamanho de Colunas\n"));
	_tscanf_s(TEXT("%d"), &iColunas);

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

	objMap1 = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Jogo), snake);

	if (objMap1 == NULL) {
		_tprintf(TEXT("[Erro]Criar objectos mapeamentos(%d)\n"), GetLastError());

	}

	dadosJ = (Jogo *)MapViewOfFile(objMap1, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(Jogo));
	
	if (dadosJ == NULL) {
		_tprintf(TEXT("[Erro]Mapear para mem�ria(%d)\n"), GetLastError());
		CloseHandle(f1);

	}
	_tprintf(TEXT("Cria��o do Pipe Para os CLientes"));



	iniciaVarJogo();

	return 0;

}

//Fun�ao para libertar recursos

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

	iniciaSerpente();
	criaMapa();


	hThreadMoveSerp = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadMoveSerpente, NULL, 0, &threadId);

	if (hThreadMoveSerp == NULL) {
		_tprintf(TEXT("Erro ao criar Thread\n"));
		return -1;
	}



	while (1) {

		hPipe = CreateNamedPipe(lpszPipename, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, BUFSIZE, BUFSIZE, 0, NULL);
		if (hPipe == INVALID_HANDLE_VALUE) {
			_tprintf(TEXT("Cria��o do namedPipe Falhou erro %d"), GetLastError());
			return -1;
		}
		fConnected = ConnectNamedPipe(hPipe, NULL);
		if (fConnected) {
			_tprintf(TEXT("Vou criar uma thread para o cliente"));
			hThread = CreateThread(NULL, 0, ThreadAtendeCliente, (LPVOID)hPipe, 0, &dwThreadId);
			if (hThread == NULL) {
				_tprintf(TEXT("ERROR NA CRIA��O DA THREAD ERRO %d\n"), GetLastError());
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
			if (dadosJ->aSerpente[i].tamanho > 0) {
				mover(&dadosJ->aSerpente[i]);
			}

		addSerpente();
		imprimeMapa();
		Sleep(1000);
	} while (1);
}

DWORD WINAPI ThreadAtendeCliente(LPVOID param) {
	char direcaoTeste;
	HANDLE hHeap = GetProcessHeap();
	DWORD cbToWrite = (lstrlen(TEXT("a")) + 1) * sizeof(TCHAR);
	DWORD cbToRead = sizeof(Envio);
	TCHAR * pchRequest = (TCHAR *)HeapAlloc(hHeap, 0, cbToWrite);
	TCHAR * pchReply = (TCHAR*)HeapAlloc(hHeap, 0, cbToWrite);
	Envio * msg = (Envio*)HeapAlloc(hHeap, 0, cbToRead);
	DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWriten = 0;
	BOOL fSucess = FALSE;
	HANDLE hPipe_1 = NULL;
	bool startGame = true;
	if (param == NULL) {
		_tprintf(TEXT("\nERRO - o Handle enviado no param � nulo \n"));
		if (pchReply != NULL)
			HeapFree(hHeap, 0, pchReply);
		if (pchRequest != NULL)
			HeapFree(hHeap, 0, pchRequest);
		return (DWORD)-1;
	}
	if (pchRequest == NULL) {
		_tprintf("N�o houve Mem�ria");
		if (pchReply != NULL)
			HeapFree(hHeap, 0, pchReply);
		return (DWORD)-1;
	}
	if (pchReply == NULL) {
		_tprintf("N�o houve Mem�ria");
		if (pchRequest != NULL)
			HeapFree(hHeap, 0, pchRequest);
		return (DWORD)-1;
	}
	_tprintf(TEXT("THREAD do servidor - a receber Mensagens\n\n\n"));
	hPipe_1 = (HANDLE)param;
	while (1) {
		if (startGame) {
		fSucess = ReadFile(hPipe_1, msg, cbToRead, &cbBytesRead, NULL);

		if (!fSucess || cbBytesRead == 0) {
			if (GetLastError() == ERROR_BROKEN_PIPE)
				_tprintf(TEXT("Cliente Desligou-se. ERRO = %d"), GetLastError());
			else
				_tprintf(TEXT("ReadFile falhou. ERRO = %d"), GetLastError());
			break;
		}

		_tprintf(TEXT("idRead= %d"), msg->pid);
		//_tprintf(TEXT("idRead= %s"), msg->username);

		
			for (int i = 0; i < NJogadores; i++) {
				if (dadosJ->jog[i].pid == -1) {
					dadosJ->jog[i].pid = msg->pid;
					//_tcscpy(dadosJ->jog[i].user, msg->username);
					break;
				}
			}
			startGame = false;
		}else{
			fSucess = ReadFile(hPipe_1, pchRequest, cbToWrite, &cbBytesRead, NULL);
			if (!fSucess || cbBytesRead == 0) {
				if (GetLastError() == ERROR_BROKEN_PIPE)
					_tprintf(TEXT("Cliente Desligou-se. ERRO = %d"), GetLastError());
				else
					_tprintf(TEXT("ReadFile falhou. ERRO = %d"), GetLastError());
				break;
			}
			_tprintf(TEXT("%d Read \n %d Writen"), cbBytesRead, cbToWrite);

			_tprintf(TEXT("%s"), pchRequest);
			//aqui processar os dados
			if (!_tcscmp(pchRequest, _T("a"))) {
				direcaoTeste = 'a';
			}
			else if (!_tcscmp(pchRequest, _T("s"))) {
				direcaoTeste = 's';
			}
			else if (!_tcscmp(pchRequest, _T("d"))) {
				direcaoTeste = 'd';
			}
			else if (!_tcscmp(pchRequest, _T("w"))) {
				direcaoTeste = 'w';
			}
			moverDirecao(&dadosJ->aSerpente[0], direcaoTeste);
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
	HeapFree(hHeap, 0, pchReply);
	_tprintf(TEXT("Thread a termninar \n"));
	return 1;
}