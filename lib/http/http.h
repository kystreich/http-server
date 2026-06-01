#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <unordered_map>
#include "../threading/threadPool.h"
#include "../socket/socket.h"

#ifdef DELETE
#undef DELETE // windows 
#endif

namespace kystreich::http {

	using StandardMap = std::unordered_map<std::string, std::string>;
	using Headers = StandardMap;

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

	struct HttpRequest {
		HttpMethod  method;
		URI         reqTarget;
		std::int8_t version;
		Headers     headers;
		Headers     trailers;
		std::string body;
	};

	class HttpResponse {
		private:
			Headers     headers_;
			Headers     trailers_;
			int         status_;
			std::string body_;
		
		public:
			HttpResponse& status(int code);
			HttpResponse& body(std::string body="");
			HttpResponse& redirect(URI);
			HttpResponse& json(StandardMap);

			void          send();
			void          sendFile(std::filesystem::path file);
	};
	
	using HandlerFunction = std::function<void(HttpRequest, HttpResponse)>;

	struct RouteParams {
		StandardMap queryParams;
		StandardMap bodyParams;
	};

	struct RouteHandler {
		HttpMethod      method;
		std::string     path;
		HandlerFunction handler;
		RouteParams     params;
	};

	class HttpServer {
		private:
			std::unique_ptr<psocket::PlatformSocket>      sock_;
			std::uint16_t                                 maxConns_;
			threading::ThreadPool                         threadPool_;
			std::unordered_map<std::string, RouteHandler> routes_;

			[[nodiscard]] RouteHandler resolveRoute(HttpRequest req) const; 
			void handleRequest();

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
