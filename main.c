#include <stdio.h>
#include <process.h>
#include <winsock2.h>
#include <ws2tcpip.h>

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
	SOCKET sock = INVALID_SOCKET;
	char buffersend[256];
	char bufferrecv[256];
	int recvcheck;
	int sendcheck;


	WSAStartup(MAKEWORD(2, 0), &wsaData);

	sock0 = socket(AF_INET, SOCK_STREAM, 0);
	// --- ノンブロッキングモードに設定 ---
	u_long mode = 1;
	ioctlsocket(sock0, FIONBIO, &mode);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(5321);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	while (1)
	{

		Sleep(10);

		sock = connect(sock0, (struct sockaddr*)&client, &len);

		if (sock != SOCKET_ERROR)
		{
			break;
		}








	}



	printf("*************************************************");

	while (1) {
		len = sizeof(client);
		recvcheck = recv(sock, bufferrecv, strlen(bufferrecv), 0);
		if (recvcheck != SOCKET_ERROR)
		{
			printf("%s\n", bufferrecv);
			break;
		}



	}
	strcpy(bufferrecv, "FROM CLIENT");

	while (1) {
		len = sizeof(client);
		sendcheck = send(sock, bufferrecv, strlen(bufferrecv), 0);
		if (sendcheck != SOCKET_ERROR)
		{
			printf("SEND SUCCESS\n");
			break;
		}


	}



	closesocket(sock);
	WSACleanup();



	return 0;
}
