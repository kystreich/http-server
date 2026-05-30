#pragma once
#include <cstdint>
#include <string>
#include "platform.h"

namespace kystreich::http::psocket {
	std::string getLastWsaErr();
	void initWinsock();
	void tidyWinsock();

	class PlatformSocket {
		private:
			socket_p rawSock_;
			sockaddr_in sockAddr_;

		public:
			PlatformSocket(std::uint16_t port);
			~PlatformSocket();

			bool bind();
			bool listen();
			socket_p accept();

			// ssize_t read();
			// ssize_t send();
	};
}
