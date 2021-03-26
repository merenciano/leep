#include "networking/socket/connection.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace leep {

void Connection::platformConnection()
{
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    assert(result == 0);

    addrinfo hints;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* info;
    result = getaddrinfo("localhost", "8888", &hints, &info);
    if (result != 0)
    {
        LEEP_CORE_ERROR("getaddrinfo failed: {0}", result);
        WSACleanup();
        return;
    }

    // Create socket
    SOCKET connect_socket =
        socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    if (connect_socket == INVALID_SOCKET)
    {
        LEEP_CORE_ERROR("Unable to create server socket: {0}", WSAGetLastError());
        freeaddrinfo(info);
        WSACleanup();
        return;
    }

    result = connect(connect_socket, info->ai_addr, (int)info->ai_addrlen);
    if (result == SOCKET_ERROR)
    {
        LEEP_CORE_ERROR("Unable to connect to server.");
        freeaddrinfo(info);
        closesocket(connect_socket);
        WSACleanup();
        return;
    }
    else
    {
        freeaddrinfo(info);
    }

    while (true)
    {

    }

    LEEP_CORE_INFO("Client disconnect");
    closesocket(connect_socket);
    return;
}

} // namespace leep