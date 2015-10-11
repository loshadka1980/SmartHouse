#pragma once
#include "BaseConnector.h"
#include "LampState.h"



class OneSwitcherOneLamp : public BaseConnector
{
public:
	OneSwitcherOneLamp(int connectors, int lamp) : /*lampState(false),*/ switchId(connectors), lampId(lamp)
	{

	}

	bool processIncomingSignal(int id, bool isTurnOn, CommandDesc& command)
	{
		if (id == switchId)
		{
			bool lampState = LampState::GetLamp()[lampId];

			command.addSingleCommand(lampId, !lampState);

			return true;
		}
		else
			return false;
	}

protected:

	// bool state;

	int switchId;

	int lampId;
};