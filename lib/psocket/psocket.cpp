#include "psocket.h"
#include "utils.h"
#include <cassert>
#include <stdexcept>
#include <utility>

void psocketInit() {
#ifdef PSOCKET_WIN
  int err{};
  WSADATA wsaData{};

  WORD wsaVersion = MAKEWORD(2, 2);
  err = WSAStartup(wsaVersion, &wsaData);

  if (err != 0) {
    throw std::runtime_error(resolveWsaErr());
  }
#endif
}

void psocketCleanup() {
#ifdef PSOCKET_WIN
  WSACleanup();
#endif
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

psocket_t plisten(psocket_t socket, int backlog) {
  return listen(socket, backlog);
}

psocket_t pconnect(psocket_t socket, sockaddr *addr, socklen_t addrlen) {
  return connect(socket, addr, addrlen);
}

psocket_t paccept(psocket_t socket, std::optional<sockaddr> addr,
                  std::optional<socklen_t> addrlen) {
  return accept(socket, addr.has_value() ? &addr.value() : nullptr,
                addrlen.has_value() ? &addrlen.value() : nullptr);
}

void psclose(psocket_t socket) {
#ifdef PSOCKET_WIN
  closesocket(socket);
#else
  close(socket);
#endif
}

psocket_t psend(psocket_t socket, const std::string &buffer, int flags) {
  // mandatory (?) narrowing conversion from size_t -> int
  return send(socket, buffer.c_str(), static_cast<int>(buffer.size()), flags);
}

template <typename T>
psocket_t preceive(psocket_t socket, T *buffer, std::uint32_t flags,
                   size_t size) {
  return recv(socket, buffer, size, flags);
}
