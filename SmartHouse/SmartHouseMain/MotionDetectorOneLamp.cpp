#include "stdafx.h"
#include "MotionDetectorOneLamp.h"
#include <chrono>
#include <ctime>
#include <iostream>

bool MotionDetectorOneLamp::processIncomingSignal(int id, bool isTurnOn, CommandDesc& command)
{
	bool lampState = LampState::GetLamp()[lampId];

	if (d_inactiveDayTime && isDayTime())
	{
		if (lampState)
		{
			// turn it off, it is day now
			command.addSingleCommand(lampId, false);
			return true;
		}
		else // do nothing
			return false;
	}
		

	if (id != switchId)
		return false;


	
	if (!lampState && isTurnOn)
	{
		command.addSingleCommand(lampId, true);
		return true;
	}

	if (lampState && !isTurnOn)
	{
		command.addSingleCommand(lampId, false);
		return true;
		// later on - todo - launch timer to turn it off
	}

		return false;

}

bool MotionDetectorOneLamp::isDayTime(void)
{
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	
	std::time_t t = std::chrono::system_clock::to_time_t(now);
//	std::string tStr = std::ctime(&t);// convert to calendar time
	struct tm ts;
	localtime_s(&ts, &t);

	if (ts.tm_hour > 6 && ts.tm_hour < 18)
		return true;
	else
		return false;
}