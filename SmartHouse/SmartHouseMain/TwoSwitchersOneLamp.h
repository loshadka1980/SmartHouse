#pragma once
#include "BaseConnector.h"
#include "LampState.h"


class TwoSwitchersOneLamp : public BaseConnector
{
public:
	TwoSwitchersOneLamp(int swA, int swB, int lamp) : switchAId(swA), switchBId(swB), lampId(lamp)
	{

	}

	bool processIncomingSignal(int id, bool isTurnOn, CommandDesc& command)
	{
		if (id == switchAId || id == switchBId)
		{
			bool lampState = LampState::GetLamp()[lampId];
			lampState = !lampState;
			LampState::GetLamp()[lampId] = lampState;


			command.addSingleCommand(lampId, lampState);

			// command.first = lampId;
			// command.second = lampState;
			return true;
		}
		else
			return false;
	}

private:

	int switchAId;
	int switchBId;
	int lampId;
};