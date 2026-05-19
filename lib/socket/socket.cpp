#include <format>
#include <assert.h>
#include "platform.h"
#include "socket.h"

namespace kystreich::http {
	std::string getLastWsaError() {
		assert(IS_WINDOWS);
		
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
}
