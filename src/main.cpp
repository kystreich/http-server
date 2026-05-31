#include "./lib/socket/socket.h"
#include <iostream>

using namespace kystreich::http;

int main() {
	psocket::PlatformSocket pSock = psocket::PlatformSocket(8080);

	auto bindRes = pSock.bind();
	auto listenRes = pSock.listen();

	while (true) {
		auto client = pSock.accept();
		std::cout << "Client (" << client << ") connected.\n";
		const char* msg = "Hi from http server\n\0";
		send(client, msg, strlen(msg), 0);
		closesocket(client);
	}
		
	auto error = psocket::getLastWsaErr();

	std::cout << error << "\n";
}
