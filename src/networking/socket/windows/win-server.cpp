#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "core/common-defs.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <future>

static void ClientThread()
{

}

void PlatformRunServer()
{
    LEEP_CORE_INFO("Starting server...");
    int32_t result;
    addrinfo hints;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo *info;
    result = getaddrinfo(nullptr, "8888", &hints, &info);
    if (result != 0)
    {
        LEEP_CORE_ERROR("Getaddrinfo failed: {0}", result);
        WSACleanup();
        return;
    }

    // Create socket
    SOCKET listen_socket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    if (listen_socket == INVALID_SOCKET)
    {
        LEEP_CORE_ERROR("Unable to create server socket: {0}", WSAGetLastError());
        freeaddrinfo(info);
        WSACleanup();
        return;
    }

     // Bind socket
    result = bind(listen_socket, info->ai_addr, (int)info->ai_addrlen);
    if (result == SOCKET_ERROR)
    {
        LEEP_CORE_ERROR("Bind server socket failed: {0}", WSAGetLastError());
        freeaddrinfo(info);
        closesocket(listen_socket);
        WSACleanup();
        return;
    }
    else
    {
        freeaddrinfo(info);
    }

    while (1)
    {
        // Listen for connection
        result = listen(listen_socket, 1);
        if (result == SOCKET_ERROR)
        {
            LEEP_CORE_ERROR("Listen failed with error: {0}", WSAGetLastError());
            closesocket(listen_socket);
            WSACleanup();
            return;
        }

        // Accept client
        SOCKET client_socket = accept(listen_socket, nullptr, nullptr);
        if (client_socket == INVALID_SOCKET)
        {
            LEEP_CORE_ERROR("accept failed with error: {0}", WSAGetLastError());
            closesocket(listen_socket);
            WSACleanup();
            return;
        }

        LEEP_CORE_INFO("Client connected");
        std::async(ClientThread);
    }
}
