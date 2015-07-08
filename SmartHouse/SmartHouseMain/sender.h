#pragma once
#include "ahidwrapper.h"

class Sender : public AhidWrapper
{
public:
	Sender() : AhidWrapper(vidSender, pidSender) {};

	void issueCommand(const int channel, const bool turnOn);

private:
	static const unsigned int vidSender = 5824;
	static const unsigned int pidSender = 1503;

	void processSendFailure(int r);
};