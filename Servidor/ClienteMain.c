#include "ServidorHeader.h"

#define BUFSIZE 512

HHOOK hook;
KBDLLHOOKSTRUCT kbdStruct;
int tser, ob, d, autom;
Serpente *sJogo;
Obj *oJogo;
Jogador cJogo[NJogadores];
HANDLE objMap1, f1;
HANDLE hPipe;
LPTSTR lpvMessage;
TCHAR chBuff[BUFSIZE];
BOOL fSucess = FALSE;
DWORD cbRead, cbToWrite, cbWriten, dwMode;
LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\pipe20");
Envio msg1, msg2;
bool ver = true;
TCHAR nome1[50], nome2[50];
int numj;

DWORD WINAPI ThreadTecla(LPVOID param);

void dadosInicio() {
	_tprintf(TEXT("OLÁ BEM VINDO AO SNAKE MULTIPLAYER!"));

	_tprintf(TEXT("Quantos Jogadores para este PC? [1/2]\n"));
	_tscanf_s(TEXT("%d"), &numj);
	_tprintf(TEXT("Introduza identificação\n"));
	_tprintf(TEXT("[JOGADOR 1 - SETAS] Qual o seu nome?\n"));
	_fgetts(nome1, 50, stdin);
	if (numj == 2) {
		_tprintf(TEXT("[JOGADOR 2 - LETRAS] Qual o seu nome?\n"));
		_fgetts(nome2, 50, stdin);
	}
}

void criaRecursos() {
	LARGE_INTEGER tam;
	//Abrir Ficheiro f1 leitura
	/*f1 = CreateFile(ficheiro, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (f1 == INVALID_HANDLE_VALUE) {
	_tprintf(TEXT("[Erro] Abrir ficheiro (%d)\n"), GetLastError());
	return -1;
	}*/
	//Saber o tamanho do ficheiro 1 em bytes
	//GetFileSizeEx(f1, &tam);
	//Criar um objecto para o mapeamento para o ficheiro aberto

	objMap1 = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, snake);

	if (objMap1 == NULL) {
		_tprintf(TEXT("[Erro]Criar objectos mapeamentos(%d)\n"), GetLastError());

	}

	dadosJ = (Jogo *)MapViewOfFile(objMap1, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(Jogo));
	if (dadosJ == NULL) {
		_tprintf(TEXT("[Erro]Mapear para memória(%d)\n"), GetLastError());
		CloseHandle(f1);
	}

	while (1) {

		hPipe = CreateFile(lpszPipename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hPipe != INVALID_HANDLE_VALUE) {
			break;
		}
		if (GetLastError() != ERROR_PIPE_BUSY) {
			_tprintf(TEXT("Não foi possivel abrir o pipe, ERROR= %d", GetLastError()));
			exit(-1);
		}

		dwMode = PIPE_READMODE_MESSAGE;
		fSucess = SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL);
		if (!fSucess) {
			_tprintf(TEXT("SetNamedPipeHandleState Falhou, erro %d"), GetLastError());
			exit(-1);
		}

		//pipe ocupado, esperar 30segundos.
		if (!WaitNamedPipe(lpszPipename, 30000)) {
			_tprintf("Esperei por uma instancia durante 30 segundos. Sair");
			exit(-1);
		}


	}


	return 0;

}

int _tmain(int argc, TCHAR ** argv[]) {
	DWORD threadId;
	HANDLE hThreadTecla;

	DWORD id;
	TCHAR u[50];
#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif
	dadosInicio();
	criaRecursos();

	EventoSnake = OpenEvent(EVENT_ALL_ACCESS, false, nEvento);

	//EventoTecla = CreateEvent(NULL, TRUE, FALSE, nTecla);
	id = GetCurrentProcessId();
	msg1.pid = id;
	_tcscpy(msg1.username, nome1);
	cbToWrite = sizeof(msg1);
	_tprintf(TEXT("A ENVIAR %d bytes: \" %d \" \n"), cbToWrite, msg1.pid);

	if (!WriteFile(hPipe, &msg1, cbToWrite, &cbWriten, NULL)) {
		_tprintf(TEXT("WriteFile falhou.Erro =%d"), GetLastError());
		//exit(-1);
	}
	else {
		_tprintf(TEXT("cbWriten =%d"), cbWriten);

	}
	if (numj == 2) {
		msg2.pid = id;
		_tcscpy(msg2.username, nome2);
		cbToWrite = sizeof(msg2);
		_tprintf(TEXT("A ENVIAR %d bytes: \" %d \" \n"), cbToWrite, msg2.pid);

		if (!WriteFile(hPipe, &msg2, cbToWrite, &cbWriten, NULL)) {
			_tprintf(TEXT("WriteFile falhou.Erro =%d"), GetLastError());
			//exit(-1);
		}
		else {
			_tprintf(TEXT("cbWriten =%d"), cbWriten);

		}

	}

	hThreadTecla = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadTecla, NULL, 0, &threadId);
	if (hThreadTecla == NULL) {
		_tprintf(TEXT("Erro ao criar Thread\n"));
		return -1;
	}

	while (1) {
		imprimeMapa();
		WaitForSingleObject(EventoSnake, INFINITE);
		ResetEvent(EventoSnake);
	}

	WaitForSingleObject(hThreadTecla, INFINITE);
	system("pause");
	return 0;
}


LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam) {

	system("cls");
	if ((nCode >= 0) && wParam == WM_KEYDOWN) {
		kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
		if (kbdStruct.vkCode == VK_LEFT) {
			lpvMessage = TEXT("a");
		}
		else if (kbdStruct.vkCode == VK_RIGHT) {
			lpvMessage = TEXT("d");
		}
		else if (kbdStruct.vkCode == VK_DOWN) {
			lpvMessage = TEXT("s");
		}
		else if (kbdStruct.vkCode == VK_UP) {
			lpvMessage = TEXT("w");

		}
		else {
		}
		if (numj == 2) {
			if (kbdStruct.vkCode == 0x41) {
				lpvMessage = TEXT("a");
			}
			else if (kbdStruct.vkCode == 0x44) {
				lpvMessage = TEXT("d");
			}
			else if (kbdStruct.vkCode == 0x53) {
				lpvMessage = TEXT("s");
			}
			else if (kbdStruct.vkCode == 0x57) {
				lpvMessage = TEXT("w");

			}
			else {
			}
		}

		cbToWrite = (lstrlen(lpvMessage) + 1) * sizeof(TCHAR);
		_tprintf(TEXT("A ENVIAR %d bytes: \" %s \" \n"), cbToWrite, lpvMessage);
		if (!WriteFile(hPipe, lpvMessage, cbToWrite, &cbWriten, NULL)) {
			_tprintf(TEXT("WriteFile falhou.Erro =%d"), GetLastError());
			exit(-1);
		}
		else {
			_tprintf(TEXT("Sucesso"));
		}

	}



	return CallNextHookEx(hook, nCode, wParam, lParam);
}

void SetHook() {
	if (!(hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0))) {
		// Erro ao iniciar o processo de escuta
	}
}

DWORD WINAPI ThreadTecla(LPVOID param) {
	MSG msg;
	//valInicial();
	SetHook(); // Instala a escuta
	while (GetMessage(&msg, NULL, 0, 0)) {}

	UnhookWindowsHookEx(hook); // Desinstala a escuta
}
