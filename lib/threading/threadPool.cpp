#include <chrono>
#include <format>
#include <mutex>
#include <optional>
#include <iostream>
#include <queue>
#include "threadPool.h"
#include "lib/socket/platform.h"

namespace kystreich::http::threading {

	ThreadPool::ThreadPool(size_t maxThreads)
	: maxThreads_(maxThreads)
	, connQueue_()
	, queueCv_{}
	{
		threads_.reserve(maxThreads);
		for (auto i{0uz}; i < maxThreads; ++i) {
			threads_.emplace_back(
				[this](std::stop_token sToken)
					{
						std::stop_callback stopCallback(sToken, [this] {
                					queueCv_.notify_all(); 
            					});
						
						while (!sToken.stop_requested()) {
							auto conn = INVALID_SOCKET;

							{
								std::unique_lock<std::mutex> lock(queueMutex_);

								queueCv_.wait(lock, [this, &sToken] {
									return !connQueue_.empty() || sToken.stop_requested();
								});

								if (sToken.stop_requested() && connQueue_.empty()) {
									break;
								}

								if (!connQueue_.empty()) {
									conn = connQueue_.front();
									connQueue_.pop();
								}


							}

							if (conn != INVALID_SOCKET) {
								auto id = std::this_thread::get_id();
								std::cout << std::format("(Thread {}) Acquired client {}\n", id, conn);

							    	const char* msg = "HTTP/1.1 204 No Content\r\n\r\n";
							    	send(conn, msg, static_cast<int>(strlen(msg)), 0);
							    	shutdown(conn, 1);
								closesocket(conn);
							}	
						}
					}
			);
		}
	};
	//ThreadPool::~ThreadPool(){};
	//ThreadPool::ThreadPool(ThreadPool&& other) noexcept {};
	//ThreadPool& ThreadPool::operator=(ThreadPool&& other) noexcept{};


	[[nodiscard]] size_t ThreadPool::queueSize() const {
		return connQueue_.size();
	};
	[[nodiscard]] size_t ThreadPool::workerCount() const {
		return threads_.size();
	};

	void ThreadPool::enqueueClient(const socket_p& conn) {
		{
			std::lock_guard lock(queueMutex_);
			connQueue_.push(conn);
		}

		queueCv_.notify_one();
	};
}
