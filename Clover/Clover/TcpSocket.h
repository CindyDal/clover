#ifndef TCPSOCKET_H_
#define TCPSOCKET_H_

#include <string>
#include "ITcpSocket.h"

#define DEFAULT_BUFLEN 512

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

class TcpSocket : public ITcpSocket{
private:
	WSADATA wsaData;
	SOCKET tcpSocket;
	SOCKET clientSocket;
public :
	virtual void SendMessage(std::string message)
	{
		int recvbuflen = DEFAULT_BUFLEN;
		char recvbuf[DEFAULT_BUFLEN];

		int iResult;

		// Send an initial buffer
		iResult = send(tcpSocket, message.c_str(), (int)strlen(message.c_str()), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed: %d\n", WSAGetLastError());
			closesocket(tcpSocket);
			WSACleanup();
			//TODO : throw
		}

		printf("Bytes Sent: %ld\n", iResult);
	}

	virtual void Initialize()
	{
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
			//TODO : throw exception
		}
	}

	virtual void Create(std::string serverAdress, std::string serverPort)
	{
		tcpSocket = INVALID_SOCKET;
		struct addrinfo *result = NULL,
			*ptr = NULL,
			hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		int iResult = getaddrinfo(serverAdress.c_str(), serverPort.c_str(), &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			//TODO: throw
		}
		
		tcpSocket = socket(ptr->ai_family, ptr->ai_socktype,ptr->ai_protocol);
		if (tcpSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			//TODO: throw
		}
	}

	virtual void Create(std::string port)
	{
		tcpSocket = INVALID_SOCKET;

		struct addrinfo *result = NULL;
		struct addrinfo hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		int iResult = getaddrinfo(NULL, port.c_str(), &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			//TODO: throw
		}

		// Create a SOCKET for connecting to server
		tcpSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (tcpSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			//TODO: throw
		}
	}

	virtual void Bind()
	{
		struct addrinfo *result = NULL;
		int iResult = bind(tcpSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(tcpSocket);
			WSACleanup();
			//TODO : throw
		}

		freeaddrinfo(result);
	}

	virtual void Listen()
	{
		int iResult = listen(tcpSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			printf("listen failed with error: %d\n", WSAGetLastError());
			closesocket(tcpSocket);
			WSACleanup();
			//TODO : throw
		}

	}

	virtual void Accept()
	{
		clientSocket = INVALID_SOCKET;

		// Accept a client socket
		clientSocket = accept(tcpSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			printf("accept failed: %d\n", WSAGetLastError());
			closesocket(tcpSocket);
			WSACleanup();
			//TODO : throw
		}

		closesocket(tcpSocket);
	}

	virtual void ConnectToServer()
	{
		struct addrinfo *result = NULL,
			*ptr = NULL;
		int iResult = connect(tcpSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(tcpSocket);
			tcpSocket = INVALID_SOCKET;
		}

		freeaddrinfo(result);

		if (tcpSocket == INVALID_SOCKET) {
			printf("Unable to connect to server!\n");
			WSACleanup();
			//TODO : throw
		}
	}

	virtual void Close()
	{
		int iResult = shutdown(tcpSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed: %d\n", WSAGetLastError());			
		}

		closesocket(tcpSocket);
		WSACleanup();
	}

	virtual void CloseClient()
	{
		int iResult = shutdown(clientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed: %d\n", WSAGetLastError());
		}

		closesocket(clientSocket);
		WSACleanup();
	}
};

#endif // !TCPSOCKET_H_