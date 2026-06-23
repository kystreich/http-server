#include "lib/psocket/psocket.h"
#include <iostream>

int main() {
  psocketInit();
  auto psock =
      psocket(PSocketDomain::IPV4, PSocketType::TCP, PSocketProtocol::ANY);

  std::cout << std::format("Is invalid: {}", psock == INVALID_SOCKET) << "\n";
  std::cout << psock << "\n";
  std::cout << resolveWsaErr() << "\n";

  psocketCleanup();
}
