#pragma once
#include "ahidwrapper.h"
#include <string>
#include <map>

class SenderAPI
{
public:

	virtual void issueCommandByID(const int channel, const bool turnOn) = 0;
	virtual void issueCommand(const std::string &channelName, const bool turnOn);
	virtual void updateLampState(int channel, bool turnOn);

	virtual void check(void) = 0;

	std::map < std::string, int> names;

};

class TestSender: public SenderAPI
{
public:

	virtual void issueCommandByID(const int channel, const bool turnOn);

	virtual void check(void);

	//	std::map < std::string, int> names;

};


class Sender : public AhidWrapper, public SenderAPI
{
public:
	Sender() : AhidWrapper(vidSender, pidSender) {};

	virtual void issueCommandByID(const int channel, const bool turnOn);


	virtual void check(void);

//	std::map < std::string, int> names;

	static const int timeoutSender = 250;

private:
	static const unsigned int vidSender = 5824;
	static const unsigned int pidSender = 1503;

	static const int checkWait = 1000;

	void processSendFailure(int r);
};