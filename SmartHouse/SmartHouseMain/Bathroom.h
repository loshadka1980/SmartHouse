#pragma once
#include "BaseConnector.h"
#include "LampState.h"


class Bathroom : public BaseConnector
{
public:
	Bathroom(int swBR, int swWC, int drBR, int drWC, int motDet, int lampB1, int lampB2, int lampWC, int vent)
		: switchBR(swBR), doorBR(drBR), switchWC(swWC), doorWC(drWC), motionDetector(motDet),
		  lampB1Id(lampB1), lampB2Id(lampB2), lampWCId(lampWC), ventId(vent)
	{

	}

	bool processIncomingSignal(int id, bool isTurnOn, CommandDesc& command);

private:

	int switchBR;
	int doorBR;
	int switchWC;
	int doorWC;
	int motionDetector;

	int lampB1Id;
	int lampB2Id;
	int lampWCId;
	int ventId;

	bool isLightOnInBathroom(void) const
	{
		LampState& l = LampState::GetLamp();
		return l[lampB2Id] || l[lampB2Id];

	}

	bool isLightOnInWC(void) const
	{
		LampState& l = LampState::GetLamp();
		return l[lampWCId];
	}
};