#pragma once
#include "ahidwrapper.h"
#include <string>
#include <map>

class Sender : public AhidWrapper
{
public:
	Sender() : AhidWrapper(vidSender, pidSender) {};

	void issueCommandByID(const int channel, const bool turnOn);
	void issueCommand(const std::string &channelName, const bool turnOn);

	void check(void);

	std::map < std::string, int> names;

	static const int timeoutSender = 250;

private:
	static const unsigned int vidSender = 5824;
	static const unsigned int pidSender = 1503;

	static const int checkWait = 1000;

	void processSendFailure(int r);
};