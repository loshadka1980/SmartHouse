#pragma once
#include "ahidwrapper.h"

class Reciever : public AhidWrapper
{
public:
	Reciever();

	bool obtainCommand(int& senderID, bool& isTurnOn, bool& isNonIncrementalFlag);
	
private:
	static const unsigned int vidReciever = 5824;
	static const unsigned int pidReciever = 1500;
	int oldTogl;

	void processObtainFailure(int r);

};

