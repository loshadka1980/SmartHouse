#pragma once
#include <array>

class LampState
{
public:

	static LampState& GetLamp();
	bool& operator[](int i);

private:

	LampState();
	std::array<bool, 32> state;
};
