#pragma once
#include "BaseConnector.h"
#include "LampState.h"

class MotionDetectorOneLamp : public BaseConnector
{
public:
	MotionDetectorOneLamp(int connectors, int lamp) : switchId(connectors), lampId(lamp)
	{
		d_inactiveDayTime = true;
	}

	bool processIncomingSignal(int id, bool isTurnOn, CommandDesc& command);

	void setInactiveDaytime(bool inactive = true)
	{
		d_inactiveDayTime = inactive;
		// possibly reset timers
	}

protected:

	bool isDayTime(void);

	bool d_inactiveDayTime;

	int switchId;

	int lampId;
};