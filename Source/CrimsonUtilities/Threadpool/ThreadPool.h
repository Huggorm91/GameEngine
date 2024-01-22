#pragma once
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <type_traits>

namespace Crimson
{
	class ThreadPool
	{
	public:
		ThreadPool(size_t aThreadCount);
		~ThreadPool();

		template<class F, class... Args>
		void EnqueueNoReturn(F&& aFunction, Args&&... someArgs);

		template<class F, class... Args>
		auto EnqueueFutureReturn(F&& aFunction, Args&&... someArgs) -> std::future<typename std::invoke_result<F, Args...>::type>;

	private:
		std::vector<std::thread> myThreads;
		std::queue<std::function<void()>> myTasks;

		std::mutex myTaskMutex;
		std::condition_variable myCondition;
		bool myIsRunning;
	};

	template<class F, class... Args>
	void ThreadPool::EnqueueNoReturn(F&& aFunction, Args&&... someArgs)
	{
		auto task = std::make_shared<std::packaged_task<void()>>(
			std::bind(std::forward<F>(aFunction), std::forward<Args>(someArgs)...)
		);

		{
			std::unique_lock<std::mutex> lock(myTaskMutex);

			if (!myIsRunning)
			{
				throw std::runtime_error("Calling Enqueue on stopped ThreadPool");
			}

			myTasks.emplace([task]()
			{
				(*task)();
			});
		}
		myCondition.notify_one();
	}

	template<class F, class... Args>
	auto ThreadPool::EnqueueFutureReturn(F&& aFunction, Args&&... someArgs)
		-> std::future<typename std::invoke_result<F, Args...>::type>
	{
		using return_type = typename std::invoke_result<F, Args...>::type;

		auto task = std::make_shared<std::packaged_task<return_type()>>(
			std::bind(std::forward<F>(aFunction), std::forward<Args>(someArgs)...)
		);

		std::future<return_type> result = task->get_future();
		{
			std::unique_lock<std::mutex> lock(myTaskMutex);

			if (!myIsRunning)
			{
				throw std::runtime_error("Calling Enqueue on stopped ThreadPool");
			}

			myTasks.emplace([task]() { (*task)(); });
		}
		myCondition.notify_one();
		return result;
	}
}