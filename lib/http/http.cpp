#include <cstdint>
#include <memory>
#include <string>
#include "http.h"
#include "lib/socket/socket.h"

namespace kystreich::http {
	HttpResponse& HttpResponse::status(int code){};
	HttpResponse& HttpResponse::body(std::string body){};
	HttpResponse& HttpResponse::redirect(URI){};
	HttpResponse& HttpResponse::json(StandardMap){};

	void HttpResponse::send(){};
	void HttpResponse::sendFile(std::filesystem::path file){};

	HttpServer::HttpServer(std::uint16_t maxConns, const std::uint16_t port, const std::uint16_t backlog, std::size_t maxThreads) 
		: sock_(std::make_unique<psocket::PlatformSocket>(port, backlog)) 
		, threadPool_(std::make_unique<threading::ThreadPool>(maxThreads))
		, maxConns_(maxConns)
		, routes_(std::unordered_map<std::string, RouteHandler>{})
		{};

	HttpServer::HttpServer(HttpServer&& other) noexcept {
		other.sock_ = std::move(sock_);
		other.threadPool_ = std::move(threadPool_);
		other.maxConns_ = maxConns_;
		other.routes_.merge(routes_); // i believe this doesnt allocate?
	};
	HttpServer& HttpServer::operator=(HttpServer&& other) noexcept {};

	void HttpServer::route(RouteHandler route){};
	void HttpServer::listen(std::uint16_t port){};
}
