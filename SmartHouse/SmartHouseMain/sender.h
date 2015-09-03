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

	std::map < std::string, int> names;

private:
	static const unsigned int vidSender = 5824;
	static const unsigned int pidSender = 1503;

	void processSendFailure(int r);
};