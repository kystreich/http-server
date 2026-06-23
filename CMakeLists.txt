cmake_minimum_required(VERSION 4.3.0)

project(
    HttpServer
    VERSION 0.0.1
    DESCRIPTION "A simple Http server"
    LANGUAGES CXX)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED on)
set (CMAKE_CXX_EXTENSIONS off)
set (CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES OFF)

set (SOURCES
src/main.cpp
lib/psocket/psocket.cpp
lib/psocket/utils.cpp
#lib/socket/socket.cpp
#lib/threading/threadpool.cpp
)

set (HEADERS
lib/psocket/psocket.h
lib/psocket/utils.h
#lib/socket/platform.h
#lib/socket/socket.h
#lib/threading/threadpool.h
)

add_executable(HttpServer ${SOURCES})

if(WIN32)
	set (OPTIONAL_INCLUDE
		ws2_32
	)

	set (OPENSSL_ROOT_DIR "C:/msys64/ucrt64/")
endif (WIN32)

find_package(OpenSSL REQUIRED)

target_link_libraries(HttpServer
	PRIVATE
	OpenSSL::SSL
	OpenSSL::Crypto
	${OPTIONAL_INCLUDE}
)

target_include_directories(HttpServer PRIVATE
    ${CMAKE_SOURCE_DIR}
)
