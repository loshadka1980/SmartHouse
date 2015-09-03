#include "stdafx.h"
#include "LampState.h"
#include <exception>
#include <algorithm>

static LampState* pLS = nullptr;

LampState& LampState::GetLamp()
{
	if (pLS == nullptr)
		pLS = new LampState();
	return *pLS;
};


bool& LampState::operator[](int i)
{
	if (i < 0 || i>31)
		throw std::exception("Wrong Lamp");

	return state[i];
}

LampState::LampState()
{
	std::fill(state.begin(), state.end(), false);
};