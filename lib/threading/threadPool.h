#pragma once

#include <condition_variable>
#include <queue>
#include <thread>
#include "lib/socket/platform.h"

namespace kystreich::http::threading { 

	const std::size_t MAX_THREADS_DEFAULT = std::thread::hardware_concurrency();

	class ThreadPool {
		friend std::jthread;

		private:
			std::size_t maxThreads_;
			std::queue<socket_p> connQueue_;
			std::mutex queueMutex_;
			std::vector<std::jthread> threads_;
			std::condition_variable queueCv_;
		
		public:
			ThreadPool(size_t maxThreads=MAX_THREADS_DEFAULT);
			// ~ThreadPool();
			// ThreadPool(const ThreadPool& other)=delete;
			// ThreadPool(ThreadPool&& other) noexcept;
			// ThreadPool& operator=(const ThreadPool&)=delete;
			// ThreadPool& operator=(ThreadPool&& other) noexcept;
		
			[[nodiscard]] size_t queueSize() const;
			[[nodiscard]] size_t workerCount() const;

			void enqueueClient(const socket_p& conn);
	};
}
