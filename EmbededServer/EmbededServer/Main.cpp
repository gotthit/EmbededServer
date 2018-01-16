#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#pragma warning(disable:4996)

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include "Controller.h"
#include "SiteState.h"

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

int main()
{
    //----------------------
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = 0;

    SOCKET ListenSocket = INVALID_SOCKET;
    sockaddr_in service;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) 
    {
        // wprintf(L"WSAStartup() failed with error: %d\n", iResult);
        return 1;
    }
    //----------------------
    // Create a SOCKET for listening for incoming connection requests.
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) 
    {
        // wprintf(L"socket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.

    IN_ADDR aa;

    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_pton(AF_INET, "localhost", &aa);
    service.sin_port = htons(80);

    iResult = bind(ListenSocket, (SOCKADDR *)& service, sizeof(service));
    if (iResult == SOCKET_ERROR) 
    {
        // wprintf(L"bind function failed with error %d\n", WSAGetLastError());
        iResult = closesocket(ListenSocket);
        if (iResult == SOCKET_ERROR)
        {
            // wprintf(L"closesocket function failed with error %d\n", WSAGetLastError());
        }
        WSACleanup();
        return 1;
    }
    //----------------------
    // Listen for incoming connection requests 
    // on the created socket
    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        // wprintf(L"listen function failed with error: %d\n", WSAGetLastError());
    }

    //----------------------
    // Create a SOCKET for accepting incoming requests.
    SOCKET AcceptSocket;
    // wprintf(L"Waiting for client to connect...\n");

    //----------------------
    // Accept the connection.
    AcceptSocket = accept(ListenSocket, NULL, NULL);
    if (AcceptSocket == INVALID_SOCKET) 
    {
        // wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    else
    {
        // wprintf(L"Client connected.\n");
    }

    // wprintf(L"Listening on socket...\n");

    SiteState state;

    Controller controller(&state);

    char recvbuf[1024];
    int recvbuflen = sizeof(recvbuf);

    int lengthOfRequest = 0;

    do {
        lengthOfRequest = recv(AcceptSocket, recvbuf, recvbuflen, 0);
        if (lengthOfRequest > 0)
        {
            // printf("Bytes received: %d\n", lengthOfRequest);

            char header_buffer[256];
            int content_length;

            recvbuf[lengthOfRequest] = '\0';

            char * content_buffer = controller.HandleRequest(recvbuf);
            content_length = strlen(content_buffer);

            sprintf(header_buffer, "HTTP/1.1 200 OK\r\nCache-Control: no-cache, private\r\nContent-Length: %d\r\n" \
                "Access-Control-Allow-Origin: *\r\nAccess-Control-Allow-Credentials: true\r\n" \
                "Content-Type: text/html; charset=windows-1251\r\n\r\n", content_length);

            send(AcceptSocket, header_buffer, (int)strlen(header_buffer), 0);
            send(AcceptSocket, content_buffer, content_length, 0);
        }
        else if (lengthOfRequest == 0)
        {
            // printf("Connection closed\n");
        }
        else
        {
            // printf("recv failed: %d\n", WSAGetLastError());
        }

    } while (lengthOfRequest > 0);


    iResult = closesocket(ListenSocket);
    if (iResult == SOCKET_ERROR) 
    {
        // wprintf(L"closesocket function failed with error %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    WSACleanup();
    return 0;
}
