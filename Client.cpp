#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <string.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define KEYMAX 26
#define TEXTLEN 512

int main()
{
	system("Color A");

	char key[KEYMAX] = "SING";
	char newKey[KEYMAX] = "";
	char matrix[5][5];

	int i;
	int j;
	int l;
	int n;
	int c;

	char k;

	int flag = 0;
	int count = 0;


	/////////////// Deo za izdvajanje elemenata sa 1 ponavljanjem ///////////////
	for (i = 0; i < strlen(key); i++)
	{
		for (j = 0; j < strlen(newKey); j++)
		{
			if (key[i] == newKey[j])
			{
				flag = 1;
			}
		}
		if (flag == 0)
		{
			newKey[j++] = key[i];
		}
		flag = 0;
	}
	

	/////////////// Popunjavanje matrice A - Z \ {J} /////////////// *(NE TREBA)*
	
	/*k = 65;
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			if (k == 'J')
				k = k + 1;
			matrix[i][j] = k;
			k++;
		}
	}*/

	/////////////// Niz: A - Z \ {J} ///////////////
	k = 65;
	char alpha[KEYMAX] = "";
	for (i = 0; i < 25; i++)
	{
		if (k == 'J')
			k = k + 1;
		alpha[i] = k;
		k++;
	}
	alpha[25] = '\0';

	/////////////// Niz: A - Z \ {J} i bez elemenata kljuca ///////////////
	char newAlpha[KEYMAX] = "";
	flag = 0;
	n = strlen(alpha);
	k = 65;
	l = 0;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < strlen(newKey); j++)
		{
			if (alpha[i] == newKey[j])
			{

				flag = 1;

			}
		}
		if (flag == 0)
		{
			newAlpha[l] = alpha[i];
			l++;
		}			
		flag = 0;
	}

	/////////////// Ubacivanje kljuca i niza u matricu ///////////////
	flag = 0;
	l = 0;
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			matrix[i][j] = newKey[l];		
			l++;
			if (l == strlen(newKey))
			{
				flag = 1;
				break;
			}			
		}
		if (flag == 1)
			break;
	}

	l = 0; flag = 0;
	j = j + 1;

	for (i; i < 5; i++)
	{
		for (j; j < 5; j++)
		{
			matrix[i][j] = newAlpha[l];
			l++;
		}
		
		j = 0;
		flag = 1;
	}

	/////////////// Ispis svega ulozenog ///////////////
	printf("key: %s %d\n", key, strlen(key));

	printf("newKey: %s  %d\n", newKey, strlen(newKey));

	printf("Array for matrix: %s %d\n\n", newAlpha, strlen(newAlpha));

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			printf("%c ", matrix[i][j]);
		}
		printf("\n");
	}
	
	/////////////// KRIPTOVANJE ///////////////
	printf("\nKRIPTOVANJE\n");
	char openText[TEXTLEN] = "RTRK";
	char cryptText[TEXTLEN] = "";

	printf("\nopenText:");
	for (i = 0; i < strlen(openText); i++)
	{
		printf("%c", openText[i]);
		if (i % 2 != 0)
			printf(" ");
	}

	printf("\n");

	int i1, j1;
	int i2, j2;
	int l2;
	char c1, c2;
	flag = 0;
	for (l = 0; l < strlen(openText); l+=2)
	{
		l2 = l + 1;
		//printf("Kriptujem: %c%c\n", openText[l], openText[l2]);
		flag = 0;

		// za prvo slovo, prve koordinate vadim
		for (i = 0; i < 5; i++)
		{		
			for (j = 0; j < 5; j++)
			{
				
				if (openText[l] == matrix[i][j])
				{
					//printf("nasao prvu: [%d][%d]:\n", i, j);
					i1 = i; j1 = j;
					flag = 1;
					break;
				}
			}
			if (flag == 1)
				break;
		}
		flag = 0;
		// za drugo slovo, druge koordinate vadim
		for (i = 0; i < 5; i++)
		{
			for (j = 0; j < 5; j++)
			{

				if (openText[l2] == matrix[i][j])
				{
					//printf("nasao drugu: [%d][%d]:\n", i, j);
					i2 = i; j2 = j;
					flag = 1;
					break;
				}
			}
			if (flag == 1)
				break;
		}

		/*if ((strlen(openText) % 2 != 0) || l2 == strlen(openText))
			printf("%c:[%d][%d]\n\n", openText[l], i1, j1);
		else
			printf("%c:[%d][%d] %c:[%d][%d]\n", openText[l], i1, j1, openText[l2], i2, j2);*/

		// KRIPTOVANJE, postoji tri glavna slucaja: (i1=i2, j1!=j2) ; (i1=!i2, j1=j2) ; (i1!=i2, j1!=j2)

		if (i1 == i2 && j1 != j2) // printf("ista vrsta razlicita kolona\n");
		{
			if (j1 == 4)
			{
				c1 = matrix[i1][0];
				c2 = matrix[i1][++j2];
			}
			else if (j2 == 4)
			{
				c1 = matrix[i1][++j1];
				c2 = matrix[i1][0];
			}
			else
			{
				c1 = matrix[i1][++j1];
				c2 = matrix[i1][++j2];
			}
			
		}
		else if (i1 != i2 && j1 == j2) // printf("razlicita vrsta ista kolona\n");
		{
			if (i1 == 4)
			{
				c1 = matrix[0][j1];
				c2 = matrix[++i2][j2];
			}
			else if (i2 == 4)
			{
				c1 = matrix[++i1][j1];
				c2 = matrix[0][j2];
			}
			else
			{
				c1 = matrix[++i1][j1];
				c2 = matrix[++i2][j2];
			}
			
		}
		else if (i1 != i2 && j1 != j2) // printf("razlicita vrsta razlicita kolona\n");
		{
			
			c1 = matrix[i1][j2];
			c2 = matrix[i2][j1];
		}
		//printf("Zakriptovano: ");
		//printf("%c%c\n\n",c1,c2);
		cryptText[l] = c1;
		cryptText[l2] = c2;
	}

	
	printf("cryptText: %s\n", cryptText);

	/////////////// <DE>KRIPTOVANJE ///////////////
	printf("\n<DE>KRIPTOVANJE\n\n");
	char decryptText[TEXTLEN] = "";

	printf("\cryptText:");
	for (i = 0; i < strlen(cryptText); i++)
	{
		printf("%c", cryptText[i]);
		if (i % 2 != 0)
			printf(" ");
	}
	printf("\n");
	i1 = 0, j1 = 0;
	i2 = 0, j2 = 0;
	l2 = 0;
	flag = 0;

	for (l = 0; l < strlen(cryptText); l += 2)
	{
		l2 = l + 1;
		//printf("DEkriptujem: %c%c\n", cryptText[l], cryptText[l2]);
		flag = 0;

		// za prvo slovo, prve koordinate vadim
		for (i = 0; i < 5; i++)
		{
			for (j = 0; j < 5; j++)
			{

				if (cryptText[l] == matrix[i][j])
				{
					//printf("nasao prvu: [%d][%d]:\n", i, j);
					i1 = i; j1 = j;
					flag = 1;
					break;
				}
			}
			if (flag == 1)
				break;
		}
		flag = 0;
		// za drugo slovo, druge koordinate vadim
		for (i = 0; i < 5; i++)
		{
			for (j = 0; j < 5; j++)
			{

				if (cryptText[l2] == matrix[i][j])
				{
					//printf("nasao drugu: [%d][%d]:\n", i, j);
					i2 = i; j2 = j;
					flag = 1;
					break;
				}
			}
			if (flag == 1)
				break;
		}
		/*if ((strlen(cryptText) % 2 != 0) || l2 == strlen(cryptText))
			printf("%c:[%d][%d]\n\n", cryptText[l], i1, j1);
		else
			printf("%c:[%d][%d] %c:[%d][%d]\n", cryptText[l], i1, j1, cryptText[l2], i2, j2);*/

		// DEKRIPTOVANJE, postoji tri glavna slucaja: (i1=i2, j1!=j2) ; (i1=!i2, j1=j2) ; (i1!=i2, j1!=j2)

		if (i1 == i2 && j1 != j2) // printf("ista vrsta razlicita kolona\n");
		{
			if (j1 == 0)
			{
				c1 = matrix[i1][0];
				c2 = matrix[i1][--j2];
			}
			else if (j2 == 0)
			{
				c1 = matrix[i1][--j1];
				c2 = matrix[i1][0];
			}
			else
			{
				c1 = matrix[i1][--j1];
				c2 = matrix[i1][--j2];
			}

		}
		else if (i1 != i2 && j1 == j2) // printf("razlicita vrsta ista kolona\n");
		{
			if (i1 == 0)
			{
				c1 = matrix[0][j1];
				c2 = matrix[--i2][j2];
			}
			else if (i2 == 0)
			{
				c1 = matrix[--i1][j1];
				c2 = matrix[0][j2];
			}
			else
			{
				c1 = matrix[--i1][j1];
				c2 = matrix[--i2][j2];
			}

		}
		else if (i1 != i2 && j1 != j2) // printf("razlicita vrsta razlicita kolona\n");
		{

			c1 = matrix[i1][j2];
			c2 = matrix[i2][j1];
		}
		//printf("Dekriptovano: ");
		//printf("%c%c\n\n", c1, c2);
		decryptText[l] = c1;
		decryptText[l2] = c2;
	}

	printf("decryptText: %s\n", decryptText);


	

	printf("\n");

	

	

	return 0;
}




/*WSADATA wsaData;
	int iResult;
	sockaddr_in addr;
	SOCKET serverSock;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(27015);
	InetPton(AF_INET, _T("192.168.1.7"), &addr.sin_addr.s_addr);

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult)
	{
		printf("WSA startup failed\n");
		return 1;
	}

	serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (serverSock == INVALID_SOCKET)
	{
		printf("Invalid socket\n");
		return 2;
	}

	iResult = connect(serverSock, (SOCKADDR *)&addr, sizeof(sockaddr_in));

	if (iResult)
	{
		printf("Connect failed %u\n", WSAGetLastError());
	}

	
	char buff[512];
	char import[512];
	FILE* file;
	char fileName[20] = "download";
	char ID[100];

	// Klijent prima svoj ID
	recv(serverSock, ID, sizeof(ID), 0);
	printf("Vas ID je: %s\n", ID);

	// Konstrukcija imena fajla + ekstenzija
	strcat(fileName, ID);
	strcat(fileName, ".txt");

	while (1)
	{
		printf("text: ");
		scanf_s("%s", buff, sizeof(buff));
		send(serverSock, buff, sizeof(buff), 0);

		// Izlaz sa servera
		if (strcmp(buff, "exit") == 0)
			break;

		// Korisnik preuzima datoteku sa servera
		recv(serverSock, import, sizeof(import), 0);	
		file = fopen(fileName, "w");
		fprintf(file, "%s", import);
		fclose(file);

		
	}
	

	closesocket(serverSock);*/