#pragma once
#include <vector>
#include <memory>
#include "Observer.hpp"

namespace CommonUtilities
{
	class Subject
	{
	public:
		Subject();
		virtual ~Subject() = default;

		virtual void Attach(std::shared_ptr<Observer> anObserver);
		virtual void Detach(std::shared_ptr<Observer> anObserver);
		virtual void Notify()=0;
	protected:
		std::vector<std::shared_ptr<Observer>> myObservers;
	};

	inline Subject::Subject() : myObservers()
	{
	}

	inline void Subject::Attach(std::shared_ptr<Observer> anObserver)
	{
		myObservers.emplace_back(anObserver);
	}

	inline void Subject::Detach(std::shared_ptr<Observer> anObserver)
	{
		auto iter = std::find(myObservers.begin(), myObservers.end(), anObserver);
		myObservers.erase(iter);
	}

	void Subject::Notify()
	{
		for (auto& observer : myObservers)
		{
			observer->Update();
		}
	}
}