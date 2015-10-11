#pragma once
#include "Commands.h"

class BaseConnector
{
public:
	virtual bool processIncomingSignal(int id, bool isTurnOn, CommandDesc& command) = 0;
};