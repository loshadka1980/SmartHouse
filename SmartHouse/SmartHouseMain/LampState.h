#pragma once
#include <array>

class LampState
{
public:

	static LampState& GetLamp();
	bool operator[](int i) const; 

	void setState(int i, bool b);
private:

	LampState();
	std::array<bool, 32> state;
};
