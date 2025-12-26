#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>   // 必ず winsock2.h の後
#include <stdio.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

unsigned __stdcall MultiThreadFunc(void* pArguments);

int main()
{
	HANDLE handles[1];
	unsigned threadID[1];

	handles[0] = (HANDLE)_beginthreadex(NULL, 0, &MultiThreadFunc, (void*)0, 0, &threadID[0]);
	WaitForSingleObject(handles[0], INFINITE);
	CloseHandle(handles[0]);

	return 0;
}

unsigned __stdcall MultiThreadFunc(void* pArguments)
{
	printf("ThreadNo.%d\n", (int)pArguments);

	WSADATA wsaData;
	SOCKET sock0;
	struct sockaddr_in addr;
	char buffersend[256] = { 0 };
	char bufferrecv[256] = { 0 };

	WSAStartup(MAKEWORD(2, 0), &wsaData);

	sock0 = socket(AF_INET, SOCK_STREAM, 0);

	// ノンブロッキング
	u_long mode = 1;
	ioctlsocket(sock0, FIONBIO, &mode);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(6000);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	// --- 非ブロッキング connect ---
	int ret = connect(sock0, (struct sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			fd_set wfds;
			FD_ZERO(&wfds);
			FD_SET(sock0, &wfds);

			// ← ここを TIMEVAL に変更
			TIMEVAL tv;
			tv.tv_sec = 5;   // 5秒
			tv.tv_usec = 0;

			printf("Connecting...\n");
			ret = select(0, NULL, &wfds, NULL, &tv);
			if (ret <= 0)
			{
				printf("Connect timeout or error\n");
				closesocket(sock0);
				WSACleanup();
				return 0;
			}
		}
		else
		{
			printf("Connect error\n");
			closesocket(sock0);
			WSACleanup();
			return 0;
		}
	}

	printf("Connected!\n");


	// --- 受信 ---

	int recvcheck = recv(sock0, bufferrecv, sizeof(bufferrecv), 0);
	if (recvcheck > 0)
	{
		printf("RECV: %s\n", bufferrecv);
	}

	// --- 送信 ---
	strcpy(buffersend, "FROM CLIENT");
	send(sock0, buffersend, strlen(buffersend), 0);
	printf("SEND SUCCESS\n");
	Sleep(20000);
	closesocket(sock0);
	WSACleanup();

	return 0;
}
