#pragma once
#include "BaseConnector.h"
#include "LampState.h"

class OneSwitcherThreeLamps : public BaseConnector
{
public:
	OneSwitcherThreeLamps(int connectors, int lampA, int lampB, int lampC) : switchId(connectors), lampAId(lampA), lampBId(lampB), lampCId(lampC)
	{

	}

	bool processIncomingSignal(int id, bool isTurnOn, CommandDesc& command)
	{
		if (id == switchId)
		{
			LampState& l = LampState::GetLamp();
			bool lampAState = l[lampAId];
			bool lampBState = l[lampBId];
			bool lampCState = l[lampCId];

			if (lampAState || lampBState || lampCState)
			{
				// switch off both
				lampAState = false;
				lampBState = false;
				lampCState = false;
			}
			else
			{
				lampAState = true;
				lampBState = true;
				lampCState = true;
				// switch on both
			}
			l[lampAId] = lampAState;
			l[lampBId] = lampBState;
			l[lampCId] = lampCState;

			//command.first = lampId;
			//command.second = lampState;
			command.addSingleCommand(lampAId, lampAState);
			command.addSingleCommand(lampBId, lampBState);
			command.addSingleCommand(lampCId, lampCState);
			return true;
		}
		else
			return false;
	}

private:

	int switchId;
	int lampAId;
	int lampBId;
	int lampCId;
};