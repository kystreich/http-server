#pragma once

#ifdef __WIN32
#include <ws2tcpip.h>
#include <windows.h>
#include <winsock2.h>
#include <winsock.h>

#elif __linux__
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#else
#error "System not supported by socket wrapper, try compiling on Windows or Linux"
#endif

namespace kystreich::http {
#ifdef __WIN32
using socket_p = SOCKET;
#define P_INVALID_SOCKET INVALID_SOCKET
#define IS_WINDOWS true

#elif __linux__
using socket_p = int;
#define P_INVALID_SOCKET -1
#define IS_WINDOWS false
#endif
}
