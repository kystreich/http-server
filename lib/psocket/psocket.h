#pragma once

#include <cstdint>

#if defined(_WIN32) || defined(__MINGW32__)
#define PSOCKET_WIN
#else
#define PSOCKET_POSIX
#endif

#ifdef PSOCKET_WIN
#define PSOCKET_INVALID_SOCKET INVALID_SOCKET
#include <winsock2.h>

using psocket_t = SOCKET;
#endif

#ifdef PSOCKET_POSIX
#define PSOCKET_INVALID_SOCKET -1
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

using psocket_t = int;
#endif

enum class PSocketTypes : std::uint8_t {
  TCP,
  TCP_IPV4,
  TCP_IPV6,

  UDP,
  UDP_IPV4,
  UDP_IPV6,

  PACKET,
  PACKET_IPV4,
  PACKET_IPV6,
};

psocket_t psocketInit();
psocket_t psocketCleanup();

psocket_t pbind(psocket_t socket);
psocket_t plisten(psocket_t socket);
psocket_t pconnect(psocket_t socket);
psocket_t paccept(psocket_t socket);

template <typename T>
psocket_t preceive(psocket_t socket, T *buffer, std::uint32_t flags,
                   size_t size = sizeof(T));
template <typename T>
psocket_t psend(psocket_t socket, T *buffer, std::uint32_t flags,
                size_t = sizeof(T));
