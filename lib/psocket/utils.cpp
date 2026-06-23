#include "utils.h"
#include "psocket.h"
#include <array>
#include <format>

const int MSG_BUF_SIZE = 256;

std::string resolveWsaErr() {

#ifdef PSOCKET_WIN
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

#endif
  return std::string{"Unsupported"};
}
