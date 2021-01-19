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
	sockaddr_in6 addr;
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
	addr.sin6_family = AF_INET6;
	InetPton(AF_INET6, _T("fe80::28e6:baa4:2138:8f79%18"), &addr.sin6_addr.u);
	addr.sin6_port = htons(27015);
	addr.sin6_flowinfo = 0;


	//////////////////////////////////// Socket ////////////////////////////////////
	sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		printf("Invalid socket\n");
		return 2;
	}
	

	//////////////////////////////////// Connect ////////////////////////////////////
	iResult = connect(sock, (SOCKADDR *)&addr, sizeof(addr));

	if (iResult)
	{
		printf("Connect failed %u\n", WSAGetLastError());
		return 2;
	}

	printf("Client IPv6 connected\n");
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