#pragma once

#include <cstdint>
#include <memory>
#include "../socket/socket.h"

namespace kystreich::http {
	class TcpServer {
		private:
			std::unique_ptr<PlatformSocket> sock_;
		
		public:
			TcpServer();
			~TcpServer()=default;
			TcpServer(const TcpServer&)=delete;
			TcpServer(const TcpServer&& other) noexcept;
			TcpServer& operator=(const TcpServer&)=delete;
			TcpServer& operator=(const TcpServer&& other) noexcept;


			void listen(uint16_t port);
	};
}
