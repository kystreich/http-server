#pragma once
#include <string>
#include "platform.h"

namespace kystreich::http {
	std::string getLastWsaErr();
	void initWinsock();
	void tidyWinsock();

	class PlatformSocket {
		private:
			
				
		public:
			~PlatformSocket();
	};
}
