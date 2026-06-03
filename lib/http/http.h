#pragma once

#include <cstddef>
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

	constexpr std::size_t MAX_CONNS_DEFAULT = 10;

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
			std::unique_ptr<threading::ThreadPool>        threadPool_;
			std::uint16_t                                 maxConns_;
			std::unordered_map<std::string, RouteHandler> routes_;

			[[nodiscard]] RouteHandler resolveRoute(HttpRequest req) const; 
			void handleRequest();

		public:
			HttpServer(
				std::uint16_t maxConns,
				std::uint16_t port,
				std::uint16_t backlog=MAX_CONNS_DEFAULT,
				std::size_t maxThreads=threading::MAX_THREADS_DEFAULT
			);
			~HttpServer()=default;
			HttpServer(const HttpServer&)=delete;
			HttpServer(HttpServer&& other) noexcept;
			HttpServer& operator=(const HttpServer&)=delete;
			HttpServer& operator=(HttpServer&& other) noexcept;

			void route(RouteHandler route);
			void listen(uint16_t port);
	};
}
