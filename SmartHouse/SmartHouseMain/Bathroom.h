#pragma once
#include "BaseConnector.h"
#include "LampState.h"
#include "DelayedActionsManager.h"


class Bathroom : public BaseConnector
{
public:
	Bathroom(int swBR, int swWC, int drBR, int drWC, int motDetB, int motDetT, int lampB1, int lampB2, int lampWC, int vent, DelayedActionsManager& i_dam)
		: switchBR(swBR), doorBR(drBR), switchWC(swWC), doorWC(drWC), motionDetectorBathroom(motDetB), motionDetectorToilet(motDetT),
		lampB1Id(lampB1), lampB2Id(lampB2), lampWCId(lampWC), ventId(vent), dam(i_dam)
	{

	}

	bool processIncomingSignal(int id, bool isTurnOn, CommandDesc& command);

private:

	DelayedActionsManager& dam;

	int switchBR;
	int doorBR;
	int switchWC;
	int doorWC;
	int motionDetectorBathroom;
	int motionDetectorToilet;

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

	void turnLightandVentOn(CommandDesc& cd);
};