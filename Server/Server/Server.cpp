/*
	Server file
	Coded by: Miras Temirbay
	All rights copyrighted (c)
*/

#include "stdafx.h"
#include "iostream"
#include <string.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

SOCKET connections[5];
int connectionCounter = 0, mistakes = 0;
int playersSocks[2];

void ClientHandlerThread(int clientSocket)
{
	char start_message2[] = "Wait for opponent!\n";
	char start_message1[] = "Start the game.";
	int read_size, mainPlayer, dest_socket;
	char client_message[1024];

	while (connectionCounter == 1) {}

	if (clientSocket == playersSocks[0])
	{
		dest_socket = playersSocks[1];
		send(clientSocket, start_message1, sizeof(start_message1), NULL);
		mainPlayer = playersSocks[0];
	}
	else {
		dest_socket = playersSocks[0];
		send(clientSocket, start_message2, sizeof(start_message2), NULL);
	}

	while ((read_size = recv(clientSocket, client_message, 1024, 0)) > 0)
	{
		send(dest_socket, client_message, 1024, 0);
	}

	if (read_size == 0)
	{
		cout << "Client #" << clientSocket << ": disconnected" << endl;
		fflush(stdout);
	}
	else if (read_size == -1) {
		cout << "Receive failure " << endl;
	}

	closesocket(clientSocket);
}

int main()
{
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);
	inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr.s_addr));
	//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sSock = socket(AF_INET, SOCK_STREAM, NULL);
	if (sSock == INVALID_SOCKET)
	{
		MessageBoxA(NULL, "Winsock socket failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	if (bind(sSock, (SOCKADDR*)&addr, sizeof(addr)))
	{
		MessageBoxA(NULL, "Winsock bind failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	if (listen(sSock, SOMAXCONN))
	{
		MessageBoxA(NULL, "Winsock listen failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	SOCKET newConnection;
	while (1)
	{
		newConnection = accept(sSock, (SOCKADDR*)&addr, &addrlen);
		if (newConnection == 0)
		{
			cout << "Failed to accept the client's connection." << endl;
			return 0;
		}
		else {
			cout << "Connection accepted" << endl;
			connectionCounter += 1;
			if (connectionCounter == 1)
			{
				playersSocks[0] = newConnection;
			}
			if (connectionCounter == 2)
			{
				playersSocks[1] = newConnection;
			}

			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(newConnection), NULL, NULL);
		}
	}

	system("pause");
	return 0;
}

