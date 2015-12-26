#pragma once
#include <array>
#include <map>

class Hook
{
public:
	Hook(): i(-1), ref(false){};
	Hook(int _i, bool _ref) : i(_i), ref(_ref) {};
	Hook& operator=(Hook& src)
	{
		i = src.i;
		ref = src.ref;
		return *this;
	}
	int i;
	bool ref;
};

class LampState
{
public:

	static LampState& GetLamp();
	bool operator[](int i) const; 

	void setState(int i, bool b);
	int registerHook(int i);
	bool checkHook(int hookId)
	{
		return hooks[hookId].ref;
	}
	void deregisterHook(int hookId);

	
private:
	void checkAllHooks(int _i);
	std::map< int, Hook > hooks;
	int hookCount;

	LampState();
	std::array<bool, 32> state;
};
