#include "./lib/socket/socket.h"
#include <iostream>

using namespace kystreich::http;

int main() {
	psocket::PlatformSocket pSock = psocket::PlatformSocket(8080);

	auto bindRes = pSock.bind();
	auto listenRes = pSock.listen();
	auto client = pSock.accept();

	std::cout << client << "\n";
}
