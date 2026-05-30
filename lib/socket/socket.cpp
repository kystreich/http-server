#include <cstdint>
#include <cstring>
#include <format>
#include <assert.h>
#include <unistd.h>
#include "platform.h"
#include "socket.h"

namespace kystreich::http::psocket {
	#if IS_WINDOWS
	std::string getLastWsaError() {
		
		char errMsgBuf[256];
		errMsgBuf[0] = '\0'; // microsoft doesn't guarantee null terminated err message https://stackoverflow.com/a/46104456	

		int wsaErrCode = WSAGetLastError();
		FormatMessage(
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				wsaErrCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				errMsgBuf,
				sizeof(errMsgBuf),
				NULL
		);

		if (!*errMsgBuf) {
			return std::format("Unable to resolve WSA error code %d", wsaErrCode);
		}

		return errMsgBuf;
	}

	void initWinsock() {
		assert(IS_WINDOWS);

		int err;
		WSADATA wsaData;

		WORD wsaVers = MAKEWORD(2, 2);
		err = WSAStartup(wsaVers, &wsaData);

		if (err != 0) {
			std::string errString = getLastWsaError();
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

	PlatformSocket::PlatformSocket(std::uint16_t port) {
		#if IS_WINDOWS 
			initWinsock();
		#endif

		addrinfo hints;
		memset(&hints, 0, sizeof(hints));

		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

		rawSock_ = socket(AF_INET, SOCK_STREAM, 0);

		if (rawSock_ == P_INVALID_SOCKET) {
			throw std::runtime_error("Failed to initialize socket");
		}

		sockaddr_in sockAddr;
		sockAddr.sin_family = hints.ai_family;
		sockAddr.sin_port = htons(port);
		sockAddr.sin_addr.s_addr = INADDR_ANY;
		sockAddr_ = sockAddr;
	}

	bool PlatformSocket::bind() {
		sockaddr newAddr;
		// TODO: technically undefined behavior, this relies on these two structs sharing identical memory layout, which is poor form.
		// TODO: Is there a reason I can't just construct the member as a sockaddr instead of a sockaddr_in?
		std::memcpy(&newAddr, &sockAddr_, sizeof(newAddr));
		return ::bind(rawSock_, &newAddr, sizeof(sockAddr_));
	}

	bool PlatformSocket::listen() {
		return ::listen(rawSock_, 10);
	}

	socket_p PlatformSocket::accept() {
		return ::accept(rawSock_, nullptr, nullptr);
	}

	PlatformSocket::~PlatformSocket() {		
		#if IS_WINDOWS
			tidyWinsock();
			closesocket(rawSock_);
			return;
		#endif

		close(rawSock_);
	}
}
