#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

#if defined(_WIN32) || defined(__MINGW32__)
#define PSOCKET_WIN
#else
#define PSOCKET_POSIX
#endif

#ifdef PSOCKET_WIN
#define PSOCKET_INVALID_SOCKET INVALID_SOCKET
#include <winsock2.h>
#include <ws2tcpip.h>

using psocket_t = SOCKET;

#endif

#ifdef PSOCKET_POSIX
#define PSOCKET_INVALID_SOCKET -1
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using psocket_t = int;
#endif

void psocketInit();
void psocketCleanup();

const int ADDR_DATA_LEN = 14;
const int ADDRESS_LENGTH_V6 = 16;

enum class PSocketDomain : std::uint8_t {
  ANY = AF_UNSPEC,
  IPV4 = AF_INET,
  IPV6 = AF_INET6,
};

enum class PSocketType : std::uint8_t {
  TCP = SOCK_STREAM,
  UDP = SOCK_DGRAM,
  SOCKET = SOCK_RAW
};

enum class PSocketProtocol : std::uint8_t {
  ANY = 0,
  TCP = 6,
  UDP = 17,
  RAW = 3,
};

psocket_t psocket(PSocketDomain domain, PSocketType type,
                  PSocketProtocol proto);

psocket_t psocket(int domain, int type, int proto);

psocket_t pbind(psocket_t socket, sockaddr *addr, socklen_t addrlen);

psocket_t plisten(psocket_t socket, int backlog);

psocket_t pconnect(psocket_t socket, sockaddr *addr, socklen_t addrlen);

psocket_t paccept(psocket_t socket, std::optional<sockaddr> addr,
                  std::optional<socklen_t> addrlen);

void psclose(psocket_t socket);

psocket_t psetopt(psocket_t socket, int level, int optionName,
                  const void *optionValue, socklen_t optionLen);

psocket_t psend(psocket_t socket, const std::string &buffer, int flags = 0);

template <typename T>
psocket_t preceive(psocket_t socket, T *buffer, std::uint32_t flags,
                   size_t size);

class PSocketBuilder {
public:
  // NOTE: use static_assert to make sure invalid methods arent used (i.e
  // calling tcp() after calling udp()) ?
  PSocketBuilder &tcp();
  PSocketBuilder &tcpv6();
  PSocketBuilder &tcpv4();

  PSocketBuilder &udp();
  PSocketBuilder &udpv6();
  PSocketBuilder &udpv4();

  PSocketBuilder &raw();
  PSocketBuilder &rawv6();
  PSocketBuilder &rawv4();

  psocket_t build();
};
