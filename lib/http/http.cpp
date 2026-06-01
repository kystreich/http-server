#include <cstdint>
#include <string>
#include "http.h"

namespace kystreich::http {
	HttpResponse& HttpResponse::status(int code){};
	HttpResponse& HttpResponse::body(std::string body){};
	HttpResponse& HttpResponse::redirect(URI){};
	HttpResponse& HttpResponse::json(StandardMap){};

	void HttpResponse::send(){};
	void HttpResponse::sendFile(std::filesystem::path file){};

	HttpServer::HttpServer(){};
	HttpServer::HttpServer(const HttpServer&& other) noexcept {};
	HttpServer& HttpServer::operator=(HttpServer&& other) noexcept {};

	void HttpServer::route(RouteHandler route){};
	void HttpServer::listen(std::uint16_t port);
}
