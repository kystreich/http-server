#include "psocket.h"
#include <format>
#include <utility>

const int MSG_BUF_SIZE = 256;

std::string resolveWsaErr() {
  std::array<char, MSG_BUF_SIZE> errMsgBuf{};
  errMsgBuf[MSG_BUF_SIZE - 1] = '\0';

  int wsaErrCode = WSAGetLastError();

  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,

                nullptr, wsaErrCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                errMsgBuf.data(), errMsgBuf.size(), nullptr);

  if (errMsgBuf.empty()) {
    return std::format("Unable to resolve Windows error code %d", wsaErrCode);
  }

  return std::string{errMsgBuf.data()};
}

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
