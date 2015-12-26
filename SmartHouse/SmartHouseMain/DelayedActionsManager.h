#pragma once
#include <chrono>

//******************************
class DelayedActionsManager
{
public:
	DelayedActionsManager()
	{
		isInitialized = false;
		channel = 0;
		action = false;
		targetDelay = 0;
	}

	void triggerDelayedEvent(int _channel, bool _action, int _targetDelay)
	{
		delayedEventInitialization = std::chrono::system_clock::now();
		isInitialized = true;
		channel = _channel;
		action = _action;
		targetDelay = _targetDelay;
	}

	void cancelDelayedEvent(void)
	{
		isInitialized = false;
	}

	bool monitor(CommandDesc& acmds)
	{
		if (!isInitialized)
			return false;
		std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(tp - delayedEventInitialization).count() > targetDelay)
		{
			isInitialized = false;
			acmds.addSingleCommand(channel, action);
			return true;
		}
		else
			return false;
	}

private:
	std::chrono::system_clock::time_point delayedEventInitialization;
	bool isInitialized;
	int targetDelay;
	int channel;
	bool action;
};
//******************************