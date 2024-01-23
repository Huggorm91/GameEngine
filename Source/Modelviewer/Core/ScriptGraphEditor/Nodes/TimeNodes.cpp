#include "Modelviewer.pch.h"
#include "TimeNodes.h"
#include "PostMaster/PostMaster.h"
#include "Threadpool/ThreadPool.h"

int MVNode_Timer::ourIDCount = 0;

void NodeWait(float aDuration, int anID)
{
	auto end = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(static_cast<int>(aDuration * 1000.f));
	do
	{
		std::this_thread::yield();
	} while (std::chrono::high_resolution_clock::now() < end);
	Engine::GetPostMaster().AddMessage(Crimson::Message(Crimson::eMessageType::NodeTimerEnded, anID));
}

void MVNode_Timer::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateDataPin<float>("Duration", PinDirection::Input);
	CreateDataPin<bool>("Trigger once", PinDirection::Input);

	CreateExecPin("Passthrough", PinDirection::Output);
	CreateDataPin<int>("Timer ID", PinDirection::Output);
	SetPinData("Timer ID", myID);
	CreateExecPin("Timer End", PinDirection::Output);

	Engine::GetPostMaster().Subscribe(this, Crimson::eMessageType::NodeTimerEnded);
	Engine::GetPostMaster().Subscribe(this, Crimson::eMessageType::NodeTimerStopped);
	Engine::GetPostMaster().Subscribe(this, Crimson::eMessageType::NodeTimerStopAll);
}

size_t MVNode_Timer::DoOperation()
{
	bool triggerOnce = false;
	float duration = 0.f;
	if (GetPinData("Trigger once", triggerOnce) && GetPinData("Duration", duration))
	{
		if (!triggerOnce || myTriggerCount == 0)
		{
			++myTriggerCount;
			Engine::GetThreadPool().EnqueueNoReturn(NodeWait, duration, myID);
		}
		
		return ExitViaPin("Passthrough");
	}

	return ExitWithError("Invalid Input!");
}

void MVNode_Timer::RecieveMessage(const Crimson::Message& aMessage)
{
	const int* messageID = aMessage.GetDataAsInt();
	if (messageID && *messageID == myID)
	{
		if (aMessage.GetMessageType() == Crimson::eMessageType::NodeTimerStopped)
		{
			++myStopCount;
		}
		else if (aMessage.GetMessageType() == Crimson::eMessageType::NodeTimerStopAll)
		{
			myStopCount = myTriggerCount;
		}
		else  // aMessage.GetMessageType() == Crimson::eMessageType::NodeTimerEnded
		{
			--myTriggerCount;
			if (myStopCount > 0)
			{
				--myStopCount;
			}
			else
			{
				ExitViaPin("Timer End");
			}
		}
	}
}

void MVNode_TimerStop::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateDataPin<int>("Timer ID", PinDirection::Input);
	CreateDataPin<bool>("Stop all triggered instances", PinDirection::Input);

	CreateExecPin("Out", PinDirection::Output, true);
}

size_t MVNode_TimerStop::DoOperation()
{
	bool stopAll = false;
	int id = 0;
	if (GetPinData("Timer ID", id) && GetPinData("Stop all triggered instances", stopAll))
	{
		if (stopAll)
		{
			Engine::GetPostMaster().SendInstantMessage(Crimson::Message(Crimson::eMessageType::NodeTimerStopAll, id));
		}
		else
		{
			Engine::GetPostMaster().SendInstantMessage(Crimson::Message(Crimson::eMessageType::NodeTimerStopped, id));
		}
		
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid Input!");
}
