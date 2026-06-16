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
			socket_p      rawSock_;
			sockaddr_in   sockAddr_;
			std::uint16_t backlog_;

			static socket_p createSock();
		public:
			PlatformSocket(std::uint16_t port, std::uint16_t backlog);
			~PlatformSocket();
			PlatformSocket(const PlatformSocket&)=delete;
			PlatformSocket(PlatformSocket&& other)=default;
			PlatformSocket& operator=(const PlatformSocket&)=delete;
			PlatformSocket& operator=(PlatformSocket&& other)=default;

			bool     bind();
			bool     listen();
			socket_p accept();
			void     close();
	};
}
