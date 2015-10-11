#pragma once
#include "BaseConnector.h"
#include "LampState.h"

class OneSwitcherTwoLamps : public BaseConnector
{
public:
	OneSwitcherTwoLamps(int connectors, int lampA, int lampB) : switchId(connectors), lampAId(lampA), lampBId(lampB)
	{

	}

	bool processIncomingSignal(int id, bool isTurnOn, CommandDesc& command)
	{
		if (id == switchId)
		{
			LampState& l = LampState::GetLamp();
			bool lampAState = l[lampAId];
			bool lampBState = l[lampBId];

			if (lampAState || lampBState)
			{
				// switch off both
				lampAState = false;
				lampBState = false;
			}
			else
			{
				lampAState = true;
				lampBState = true;
				// switch on both
			}

			command.addSingleCommand(lampAId, lampAState);
			command.addSingleCommand(lampBId, lampBState);

			return true;
		}
		else
			return false;
	}

private:

	int switchId;
	int lampAId;
	int lampBId;
};
