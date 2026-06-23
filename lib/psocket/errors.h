#include "psocket.h"
#include <iostream>

int hi() {
  std::cout << "hi\n";

  auto tcpSocket = PSocketBuilder{}.tcp().build();
  auto udpSocket = PSocketBuilder{}.udp().build();
  auto rawSocket = PSocketBuilder{}.raw().build();
}
