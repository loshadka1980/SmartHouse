#pragma once
#include <chrono>
#include <map>
#include "Commands.h"

class DelayedSingleAction
{
public:
	DelayedSingleAction();

	void triggerDelayedEvent(int _channel, bool _action, int _targetDelay);
	void cancelDelayedEvent(void);
	bool monitor(CommandDesc& acmds);


private:
	std::chrono::system_clock::time_point delayedEventInitialization;
	bool isInitialized;
	int targetDelay;
	int channel;
	bool action;
};

class DelayedActionsManager
{
public:
	DelayedActionsManager();

	void triggerDelayedEvent(int index, int _channel, bool _action, int _targetDelay);
	void cancelDelayedEvent(int index);
	bool monitor(CommandDesc& acmds);


private:
	std::map<int, DelayedSingleAction> da;

};

//******************************