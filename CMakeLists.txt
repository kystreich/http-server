cmake_minimum_required(VERSION 4.3.0)

project(
    HttpServer
    VERSION 0.0.1
    DESCRIPTION "A simple Http2 server"
    LANGUAGES CXX)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED on)
set (CMAKE_CXX_EXTENSIONS off)
set (CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES OFF)

set (OPENSSL_ROOT_DIR "C:/msys64/ucrt64/")

find_package(OpenSSL REQUIRED)

set (SOURCES
src/main.cpp
lib/tcp/tcp.cpp
)

set (HEADERS
lib/tcp/tcp.h
)

add_executable(HttpServer ${SOURCES})

target_link_libraries(HttpServer
	PRIVATE
	OpenSSL::SSL
	OpenSSL::Crypto
	ws2_32
)

target_include_directories(HttpServer PRIVATE
    ${CMAKE_SOURCE_DIR}
)
