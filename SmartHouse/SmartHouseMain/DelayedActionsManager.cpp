#include "stdafx.h"
#include "DelayedActionsManager.h"


DelayedSingleAction::DelayedSingleAction()
{
	isInitialized = false;
	channel = 0;
	action = false;
	targetDelay = 0;
}

void DelayedSingleAction::triggerDelayedEvent(int _channel, bool _action, int _targetDelay)
{
	delayedEventInitialization = std::chrono::system_clock::now();
	isInitialized = true;
	channel = _channel;
	action = _action;
	targetDelay = _targetDelay;
}

void DelayedSingleAction::cancelDelayedEvent(void)
{
	isInitialized = false;
}

bool DelayedSingleAction::monitor(CommandDesc& acmds)
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



DelayedActionsManager::DelayedActionsManager()
{

}

void DelayedActionsManager::triggerDelayedEvent(int index, int _channel, bool _action, int _targetDelay)
{
	auto i = da.find(index);
	if (i == da.end())
	{
		da.insert(std::make_pair(index, DelayedSingleAction()));
		i = da.find(index);
	}

	i->second.triggerDelayedEvent(_channel, _action, _targetDelay);
}

void DelayedActionsManager::cancelDelayedEvent(int index)
{
	auto i = da.find(index);
	if (i != da.end())
	{
		i->second.cancelDelayedEvent();
	}
}

bool DelayedActionsManager::monitor(CommandDesc& acmds)
{
	bool res = false;
	for (auto i = da.begin(); i != da.end(); ++i)
	{
		res = res || i->second.monitor(acmds);
	}
	return res;
}

