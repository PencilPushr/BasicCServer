#pragma once

#ifdef _WIN32

#include <WinSock2.h>
#include <ws2tcpip.h>
using socket_t = SOCKET;
inline int closeSocket(socket_t socket) { return closesocket(socket); }
#pragma comment(lib, "Ws2_32.lib")

#else // POSIX

#include <sys/types.h>
#include <sys/socket.h>
using socket_t = int;
inline int closeSocket(socket_t socket) { return close(socket); }

#endif

template<typename T>
struct SocketType
{

};