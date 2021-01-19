#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <string.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

int main()
{
	WSADATA wsaData;
	int iResult;
	sockaddr_in addr;
	int addrLen = sizeof(addr);
	SOCKET sock;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult)
	{
		printf("WSA startup failed\n");
		return 1;
	}


	//////////////////////////////////// Declare and initialize client address ////////////////////////////////////
	memset((char*)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	InetPton(AF_INET, _T("192.168.1.7"), &addr.sin_addr.s_addr);
	addr.sin_port = htons(27015);


	//////////////////////////////////// Socket ////////////////////////////////////
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		printf("Creating socket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	//////////////////////////////////// Connect ////////////////////////////////////
	iResult = connect(sock, (SOCKADDR *)&addr, sizeof(addr));
	if (iResult)
	{
		printf("Connect failed %u\n", WSAGetLastError());
	}

	printf("Client IPv4 connected\n");
	char buff[512];
	char import[512];
	FILE* file;
	char fileName[20] = "download";
	char ID[100];

	// Client get ID from Server
	recv(sock, ID, sizeof(ID), 0);
	printf("Vas ID je: [%s]\n", ID);

	// Make "file.txt"
	strcat_s(fileName, ID);
	strcat_s(fileName, ".txt");

	//////////////////////////////////// main loop ////////////////////////////////////
	while (1)
	{
		printf("Poruka: ");
		gets_s(buff, sizeof(buff));

		send(sock, buff, sizeof(buff), 0);

		if (strcmp(buff, "exit") == 0)
			break;

		// Korisnik preuzima datoteku sa servera
		recv(sock, import, sizeof(import), 0);
		fopen_s(&file, fileName, "w");
		fprintf(file, "%s", import);
		fclose(file);
	}

	closesocket(sock);

	return 0;
}