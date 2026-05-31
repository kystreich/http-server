#include <cstdint>
#include <cstring>
#include <format>
#include <assert.h>
#include <unistd.h>
#include <winsock2.h>
#include "platform.h"
#include "socket.h"

namespace kystreich::http::psocket {
	#if IS_WINDOWS
	std::string getLastWsaErr() {
		std::array<char, 256> errMsgBuf{};
		errMsgBuf[0] = '\0'; // microsoft doesn't guarantee null terminated err message https://stackoverflow.com/a/46104456	

		int wsaErrCode = WSAGetLastError();
		FormatMessage(
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				wsaErrCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				errMsgBuf.data(),
				sizeof(errMsgBuf),
				NULL
		);

		if (errMsgBuf.empty()) {
			return std::format("Unable to resolve WSA error code %d", wsaErrCode);
		}
		
		// Kind of hate all of this
		return std::string{errMsgBuf.data()};
	}

	void initWinsock() {
		assert(IS_WINDOWS);

		int err{};
		WSADATA wsaData;

		WORD wsaVers = MAKEWORD(2, 2);
		err = WSAStartup(wsaVers, &wsaData);

		if (err != 0) {
			std::string errString = getLastWsaErr();
			throw std::runtime_error(
					std::format("Failed to start up winsock | %s", errString)
			);
		}
	}

	void tidyWinsock() {
		assert(IS_WINDOWS);

		WSACleanup();
	}
	#endif

	PlatformSocket::PlatformSocket(const std::uint16_t port, const std::uint16_t backlog)
	: backlog_(backlog)
	, sockAddr_{}
	, rawSock_{socket(AF_INET, SOCK_STREAM, 0)}
	{
		#if IS_WINDOWS 
		initWinsock();
		#endif

		addrinfo hints{};
		memset(&hints, 0, sizeof(hints));

		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;


		if (rawSock_ == P_INVALID_SOCKET) {
			throw std::runtime_error("Failed to initialize socket");
		}

		sockaddr_in sockAddr{};
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(port);
		sockAddr.sin_addr.s_addr = INADDR_ANY;
		sockAddr_ = sockAddr;
	}

	PlatformSocket::~PlatformSocket() {		
		close();
	}	

	bool PlatformSocket::bind() {
		sockaddr newAddr{};
		// TODO: technically undefined behavior, this relies on these two structs sharing identical memory layout, which is poor form.
		// TODO: Is there a reason I can't just construct the member as a sockaddr instead of a sockaddr_in?
		std::memcpy(&newAddr, &sockAddr_, sizeof(newAddr));
		return ::bind(rawSock_, &newAddr, sizeof(sockAddr_));
	}

	bool PlatformSocket::listen() {
		return ::listen(rawSock_, backlog_);
	}

	socket_p PlatformSocket::accept() {
		return ::accept(rawSock_, nullptr, nullptr);
	}	

	void PlatformSocket::close() {
		#if IS_WINDOWS
		tidyWinsock();
		closesocket(rawSock_);
		return;
		#endif

		::close(rawSock_);
	}
}
