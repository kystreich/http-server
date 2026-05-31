#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>
#include "../threading/threadPool.h"
#include "../socket/socket.h"

namespace kystreich::http {

	using Headers = std::unordered_map<std::string, std::string>;

	enum class HttpMethod {
		_GET,
		_HEAD,
		_POST,
		_PUT,
		_DELETE, // delete is a reserved word, can't think of a better workaround.
		_CONNECT,
		_OPTIONS,
		_TRACE,
		_PATCH 
	};

	enum class Protocol {
		HTTP,
		HTTPS
	};

	struct URI {
		Protocol protocol;
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

	struct RouteHandler {
		HttpMethod method;
		std::string path;
		std::function<void(HttpMessage, HttpMessage)> handler;
	};

	class HttpServer {
		private:
			std::unique_ptr<psocket::PlatformSocket> sock_;
			std::uint16_t maxConns_;
			threading::ThreadPool threadPool_;

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
