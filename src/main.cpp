#include "lib/psocket/psocket.h"
#include "lib/psocket/utils.h"
#include <iostream>

int main() {
  psocketInit();

  addrinfo hints{};
  addrinfo *res{};
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  auto gai = getaddrinfo(nullptr, "3490", &hints, &res);

  auto psock = psocket(res->ai_family, res->ai_socktype, res->ai_protocol);

  std::cout << std::format("Is invalid: {}", psock == INVALID_SOCKET) << "\n";
  std::cout << psock << "\n";
  std::cout << resolveWsaErr() << "\n";

  pbind(psock, res->ai_addr,
        static_cast<socklen_t>(
            res->ai_addrlen)); // clang seems to think ai_addrlen will be a
                               // size_t, for some reason.

  psocketCleanup();
}
