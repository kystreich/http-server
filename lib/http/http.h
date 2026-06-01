#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>
#include "../threading/threadPool.h"
#include "../socket/socket.h"

#ifdef DELETE
#undef DELETE // windows 
#endif

namespace kystreich::http {

	using Headers = std::unordered_map<std::string, std::string>;

	enum class HttpMethod {
		GET,
		HEAD,
		POST,
		PUT,
		DELETE,
		CONNECT,
		OPTIONS,
		TRACE,
		PATCH 
	};	

	struct URI {
		std::string address;
		std::string path;
		std::string params;
	};

	struct HttpMessage {
		HttpMethod method;
		URI reqTarget;
		std::int8_t version;
		Headers headers;
		Headers trailers;
		std::string body;
	};

	using HandlerFunction = std::function<void(HttpMessage, HttpMessage)>;

	struct RouteHandler {
		HttpMethod method;
		std::string path;
		HandlerFunction handler;
	};

	class HttpServer {
		private:
			std::unique_ptr<psocket::PlatformSocket> sock_;
			std::uint16_t maxConns_;
			threading::ThreadPool threadPool_;
			std::unordered_map<std::string, RouteHandler> routes_;

			[[nodiscard]] RouteHandler resolveRoute(HttpMessage req) const; 

		public:
			HttpServer();
			~HttpServer()=default;
			HttpServer(const HttpServer&)=delete;
			HttpServer(const HttpServer&& other) noexcept;
			HttpServer& operator=(const HttpServer&)=delete;
			HttpServer& operator=(HttpServer&& other) noexcept;


			void route(RouteHandler route);
			void listen(uint16_t port);
	};
}
