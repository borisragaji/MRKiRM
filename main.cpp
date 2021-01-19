#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <WS2tcpip.h>
#include <process.h>
#include <tchar.h>
#include <string.h>
#include <windows.h>
#include <in6addr.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

typedef struct Adresa {
	SOCKET sock;
	sockaddr_in6 addr;
}Adresa;

unsigned int __stdcall ServClient(void *data);
bool is_ipV4_address(sockaddr_in6 address);

int main()
{
	system("Color A");
	WSADATA wsaData;
	sockaddr_in6 serverAddr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return 1;
	}

	//////////////////////////////////// Declare and initialize client address ////////////////////////////////////
	memset((char*)&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin6_family = AF_INET6;
	//InetPton(AF_INET6, _T("fe80::28e6:baa4:2138:8f79%18"), &serverAddr.sin6_addr.u);
	serverAddr.sin6_addr = in6addr_any;
	serverAddr.sin6_port = htons(27015);
	serverAddr.sin6_flowinfo = 0;


	//////////////////////////////////// Sockets ////////////////////////////////////
	SOCKET listenSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	SOCKET clientSocket;

	if (listenSocket == INVALID_SOCKET)
	{
		printf("Creating socket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	char no[4] = { 0 };
	int ip6Result = setsockopt(listenSocket, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)no, sizeof(no));

	if (ip6Result == SOCKET_ERROR)
	{
		printf("failed with error: %u\n", WSAGetLastError());
	}

	//////////////////////////////////// Bind ////////////////////////////////////
	ip6Result = bind(listenSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr));

	if (ip6Result == SOCKET_ERROR)
	{
		printf("Socket bind failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	//////////////////////////////////// Listen ////////////////////////////////////
	ip6Result = listen(listenSocket, SOMAXCONN);

	if (ip6Result == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}


	//////////////////////////////////// Accept ////////////////////////////////////
	sockaddr_in6 clientAddr;
	int sockAddrLen = sizeof(clientAddr);
	Adresa adresa;
	
	while (clientSocket = accept(listenSocket, (SOCKADDR *)&clientAddr, &sockAddrLen))
	{
		if (clientSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}
		else
		{	
			adresa.sock = clientSocket;
			adresa.addr = clientAddr;
			_beginthreadex(0, 0, ServClient, (void*)&adresa, 0, 0);
		}
	}
	


	return 0;
}

unsigned int __stdcall ServClient(void *data)
{
	Adresa* adresa = (Adresa *)data;
	Adresa clAdresa = *adresa;
	SOCKET ClientSocket = clAdresa.sock;
	sockaddr_in6 ClientAddres = clAdresa.addr;

	FILE* file;
	char fileName[20];

	char buff[512];

	int id = GetCurrentThreadId();
	
	char ID[10];
	sprintf_s(ID, "%d", id);
	send(ClientSocket, ID, sizeof(ID), 0);

	////////////////// Convert IPv4 and IPv6 //////////////////
	char ipAddress[INET6_ADDRSTRLEN];
	// Copy client ip to local char[]
	inet_ntop(AF_INET6, &ClientAddres.sin6_addr, ipAddress, sizeof(ipAddress));
	// Convert port number from network byte order to host byte order
	unsigned short ClientPort = ntohs(ClientAddres.sin6_port);
	//true for IPv4 and false for IPv6
	bool isIPv4 = is_ipV4_address(ClientAddres);

	if (isIPv4)
	{
		char ipAddress4[15];
		//struct in_addr *ipv4 = (struct in_addr*)&((char*)&ClientAddres.sin6_addr.u)[12];
		inet_ntop(AF_INET, &ClientAddres, ipAddress4, sizeof(ipAddress4));
		printf("[IPv4] Client connected from --- PORT:[%d]\t IP:[%s]  ID:[%d]\n\n", ClientPort, ipAddress4, id);

	}
	else
	{
		printf("[IPv6] Client connected from --- PORT:[%d]\t IP:[%s]  ID:[%d]\n\n", ClientPort, ipAddress, id);
	}


	while (recv(ClientSocket, buff, sizeof(buff), 0))
	{
		if (strcmp(buff, "exit") == 0)
		{
			printf("\nClient [%d] disconnect\n\n", id);
		}
		else
		{
			printf("> [%d]: %s\n", id, buff);

			sprintf_s(fileName, "%d", id);
			strcat_s(fileName, ".txt");
			fopen_s(&file, fileName, "w");
			fprintf(file, "%s", buff);
			fclose(file);

			// Server salje klijentu datoteku
			send(ClientSocket, buff, sizeof(buff), 0);
			
		}
		
	}

	return 0;
}

bool is_ipV4_address(sockaddr_in6 address)
{
	char *check = (char*)&address.sin6_addr.u;

	for (int i = 0; i < 10; i++)
		if (check[i] != 0)
			return false;

	if (check[10] != -1 || check[11] != -1)
		return false;

	return true;
}