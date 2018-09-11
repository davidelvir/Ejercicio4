#include <iostream>
#include <thread>
#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <tchar.h>


#define STRING_SIZE 1024

struct Segmento {
	bool servidorFlag;
	bool clienteFlag;
	char mensajeS[STRING_SIZE];
	char mensajeC[STRING_SIZE];
};

Segmento *segmento;

using namespace std;

LPCTSTR bufS;
HANDLE shmemS = INVALID_HANDLE_VALUE;
int message;

char text[STRING_SIZE];

void read() {
	while (true) {
		
		segmento = (Segmento *)MapViewOfFile(shmemS, // handle to map object
			FILE_MAP_ALL_ACCESS,  // read/write permission
			0,
			0,
			STRING_SIZE);
		segmento->clienteFlag = true;
		while (segmento->clienteFlag);
		cout << segmento->mensajeC << endl;
	}
}

void write() {
	while (true) {
		cout << "Cliente" << endl;
		cout << ":> " << endl;
		cin.getline(text, STRING_SIZE);
		strcpy_s(segmento->mensajeS, text);
		segmento->servidorFlag = false;
		CopyMemory((PVOID)bufS, segmento, (sizeof(Segmento)));
		_getch();
	}
}

int main() {

	shmemS = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		STRING_SIZE,
		"share_memory_comunicationS"
	);

	

	bufS = (LPTSTR)::MapViewOfFile(shmemS, FILE_MAP_ALL_ACCESS, 0, 0, STRING_SIZE);
	
	thread writeM(write);
	thread readM(read);

	writeM.join();
	readM.join();

	
	::UnmapViewOfFile(bufS);
	::CloseHandle(shmemS);


	return 0;
}