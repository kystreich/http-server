#include "lib/psocket/psocket.h"
#include "lib/psocket/utils.h"
#include <cstring>
#include <iostream>

int main() {
  psocketInit();

  addrinfo hints{};
  addrinfo *res{};
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  auto gai = getaddrinfo(nullptr, "8080", &hints, &res);

  auto psock = psocket(res->ai_family, res->ai_socktype, res->ai_protocol);

  std::cout << std::format("Is invalid: {}", psock == PSOCKET_INVALID_SOCKET)
            << "\n";
  std::cout << std::format("Server Socket: {}\n", psock);
  // std::cout << resolveWsaErr() << "\n";

  pbind(psock, res->ai_addr,
        static_cast<socklen_t>(
            res->ai_addrlen)); // clang seems to think ai_addrlen will be a
                               // size_t, for some reason.

  plisten(psock, 10);

  while (true) {
    auto client = paccept(psock, std::nullopt, std::nullopt);
    std::cout << "Client: " << client << "\n";

    std::string message{
        "\n\n\n=============\nHello client!\n=============\n\n\n"};
    auto result = psend(client, message);
    // std::cout << "WS2 Error: " << resolveWsaErr() << "\n";
    psclose(client);
  }
  psocketCleanup();
}
