#pragma once

#include <condition_variable>
#include <queue>
#include <thread>
#include "lib/socket/platform.h"

namespace kystreich::http::threading { 

	struct RouteHandler {
		std::function<void()> handler;
	};

	class ThreadPool {
		private:
			std::size_t maxThreads_;
			std::queue<socket_p> connQueue_;
			std::condition_variable mutexCon_;
			std::mutex queueMutex_;
			std::vector<std::jthread> threads_;
			bool shouldShutdown_;

		public:
			ThreadPool();
			~ThreadPool();
			ThreadPool(const ThreadPool& other)=delete;
			ThreadPool(const ThreadPool&& other) noexcept;
			ThreadPool& operator=(const ThreadPool&)=delete;
			ThreadPool& operator=(ThreadPool&& other) noexcept;
		
			[[nodiscard]] ssize_t queueSize() const;
			[[nodiscard]] ssize_t workerCount() const;
			[[nodiscard]] bool shouldShutdown() const;

			void enqueueClient();
			[[nodiscard]] socket_p dequeueClient();
			void shutdown();
	};
}
