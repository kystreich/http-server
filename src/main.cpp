#include <iostream>
#include "./lib/socket/socket.h"
#include "lib/threading/threadPool.h"

using namespace kystreich::http;

int main() {
	threading::ThreadPool tPool = threading::ThreadPool{};
	psocket::PlatformSocket pSock = psocket::PlatformSocket{8080, 10};

	auto bindRes = pSock.bind();
	auto listenRes = pSock.listen();

	while (true) {
		auto client = pSock.accept();
		tPool.enqueueClient(client);
	}
		
	auto error = psocket::getLastWsaErr();

	std::cout << error << "\n";
}
