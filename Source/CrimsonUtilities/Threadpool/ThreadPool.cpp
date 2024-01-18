#include "ThreadPool.h"

namespace Crimson
{
	ThreadPool::ThreadPool(size_t aThreadCount) :
		myIsRunning(true)
	{
		for (size_t i = 0; i < aThreadCount; ++i)
		{
			myThreads.emplace_back([this]
			{
				while (myIsRunning)
				{
					std::function<void()> task;

					{
						std::unique_lock<std::mutex> lock(this->myTaskMutex);
						this->myCondition.wait(lock, [this] { return !this->myIsRunning || !this->myTasks.empty(); });

						if (this->myTasks.empty())
						{
							continue;
						}

						task = std::move(this->myTasks.front());
						this->myTasks.pop();
					}

					task();
				}
			}
			);
		}
	}

	ThreadPool::~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(myTaskMutex);
			myIsRunning = false;
		}
		myCondition.notify_all();
		for (std::thread& worker : myThreads)
		{
			worker.join();
		}
	}
}