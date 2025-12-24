#include <stdio.h>
#include <process.h>
#include <winsock2.h>

#define CLIENTNUM 1


#pragma comment(lib, "ws2_32.lib")

unsigned __stdcall MultiThreadFunc(void* pArguments);

int main()
{

	HANDLE handles[CLIENTNUM] = { NULL };
	unsigned threadID[CLIENTNUM] = { 0 };

	for (int i = 0; i < CLIENTNUM; ++i)
	{
		handles[i] = (HANDLE)_beginthreadex(NULL, 0, &MultiThreadFunc, (void*)i, 0, &threadID);
		Sleep(1);
	}

	for (int i = 0; i < CLIENTNUM; ++i)
	{

		if (handles[i] != 0)
		{
			WaitForSingleObject(handles[i], INFINITE);
		}

	}

	for (int i = 0; i < CLIENTNUM; ++i)
	{

		if (handles[i] != 0)
		{
			CloseHandle(handles[i]);
		}

	}


	return 0;
}




unsigned __stdcall MultiThreadFunc(void* pArguments)
{
	printf("ThreadNo.%d\n", (int)pArguments);

	WSADATA wsaData;
	SOCKET sock0;
	struct sockaddr_in addr;
	struct sockaddr_in client;
	int len;
	SOCKET sock;
	char buffer[256];
	int recvcheck;
	int sendcheck;


	WSAStartup(MAKEWORD(2, 0), &wsaData);

	sock0 = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(5321);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	while ((sock = connect(sock0, (struct sockaddr*)&client, &len)) == INVALID_SOCKET)
	{
		closesocket(sock);

	}
	printf("******************");

	while (1) {
		len = sizeof(client);
		recvcheck = recv(sock, buffer, strlen(buffer), 0);
		if (recvcheck != SOCKET_ERROR)
		{
			printf("%s\n", buffer);
			break;
		}
		

		closesocket(sock);
	}
	strcpy(buffer, "FROM CLIENT");

	while (1) {
		len = sizeof(client);
		sendcheck = send(sock, buffer, strlen(buffer), 0);
		if (sendcheck != SOCKET_ERROR)
		{
			printf("SEND SUCCESS\n");
			break;
		}


		closesocket(sock);
	}
	


	WSACleanup();



	return 0;
}