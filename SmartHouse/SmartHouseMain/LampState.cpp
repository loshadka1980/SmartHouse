#include "stdafx.h"
#include "LampState.h"
#include <exception>
#include <algorithm>

#include <iostream>

static LampState* pLS = nullptr;

LampState& LampState::GetLamp()
{
	if (pLS == nullptr)
		pLS = new LampState();
	return *pLS;
};


bool LampState::operator[](int i) const
{
	if (i < 0 || i>31)
		throw std::exception("Wrong Lamp");

	return state[i];
}

void LampState::setState(int i, bool b)
{
	if (i < 0 || i>31)
		throw std::exception("Wrong Lamp");

	state[i] = b;

	checkAllHooks(i);
}

LampState::LampState()
{
	std::fill(state.begin(), state.end(), false);

	hookCount = 0;
};


void LampState::checkAllHooks(int _i)
{
	for (auto it = hooks.begin(); it != hooks.end(); ++it)
	{
		if ((it->second).i == _i)
		{
			std::cout << "Setting reference flag for lamp id = " << _i << std::endl;
			((it->second).ref) = true;
		}
	}
}

int LampState::registerHook(int i)
{
	
	hookCount++;
	std::cout << "Registered hook for id = " << i << ", hookID = " << hookCount << std::endl;
	hooks[hookCount] = Hook(i, false);
	return hookCount;
}
void LampState::deregisterHook(int hookId)
{
	std::cout << "Deregistered hookID = " << hookId << std::endl;
	hooks.erase(hookId);
}