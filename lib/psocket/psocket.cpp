#include "psocket.h"
#include "utils.h"
#include <stdexcept>
#include <utility>

void psocketInit() {
  static_assert(PSOCKET_WIN, "Windows only function (psocketInit)");

  int err{};
  WSADATA wsaData{};

  WORD wsaVersion = MAKEWORD(2, 2);
  err = WSAStartup(wsaVersion, &wsaData);

  if (err != 0) {
    throw std::runtime_error(resolveWsaErr());
  }
}

void psocketCleanup() {
  static_assert(PSOCKET_WIN, "Windows only function (psocketCleanup)");

  WSACleanup();
}

psocket_t psocket(PSocketDomain domain, PSocketType type,
                  PSocketProtocol proto) {
  return socket(std::to_underlying(domain), std::to_underlying(type),
                std::to_underlying(proto));
}

psocket_t psocket(int domain, int type, int proto) {
  return socket(domain, type, proto);
}

psocket_t pbind(psocket_t socket, sockaddr *addr, socklen_t addrlen) {
  return bind(socket, addr, addrlen);
}
