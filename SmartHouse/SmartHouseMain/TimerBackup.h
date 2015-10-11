#pragma once






class BaseTimer
{
public:
	virtual bool processTimerSignal(CommandDesc& command) = 0;
};

class DelayedTurnOff : public BaseTimer
{
public:
	DelayedTurnOff(int lamp, std::chrono::seconds& i_delayInterval) : lampId(lamp), delayInterval(i_delayInterval), statusActive(false)
	{

	}

	bool processTimerSignal(CommandDesc& command)
	{
		if (!statusActive)
			return false;

		std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();

		if (currentTime > activationTime + delayInterval)
		{
			LampState::GetLamp()[lampId] = false;
			command.addSingleCommand(lampId, false);
			statusActive = false;
			std::cout << "W" << std::endl;
			return true;
		}
		else
		{
			std::cout << "-";
			return false;
		}
	}

	void activate()
	{
		statusActive = true;
		activationTime = std::chrono::system_clock::now();
		std::cout << "Activated" << std::endl;
	}

private:



	int lampId;

	std::chrono::seconds delayInterval;

	// status represents whether we are tracking time
	bool statusActive;
	std::chrono::system_clock::time_point activationTime;
};

class MonitorOtherLamps : public BaseTimer
{
public:
	MonitorOtherLamps(int lampA, int lampB, int i_lampControl) : lampAId(lampA), lampBId(lampB), lampControl(i_lampControl), doWeThinkItIsOn(true)
	{

	}

	bool processTimerSignal(CommandDesc& command)
	{
		bool b1 = LampState::GetLamp()[lampAId];
		bool b2 = LampState::GetLamp()[lampBId];
		bool bm = LampState::GetLamp()[lampControl];

		if ((b1 || b2) && (!bm || !doWeThinkItIsOn)) // turn monitored lamp on
		{
			command.addSingleCommand(lampControl, true);
			doWeThinkItIsOn = true;
			return true;
		}

		if ((!b1 && !b2) && (bm || doWeThinkItIsOn)) // turn monitored lamp off
		{
			command.addSingleCommand(lampControl, false);
			doWeThinkItIsOn = false;
			return true;
		}
		return false;
	}


private:

	int lampAId;
	int lampBId;
	int lampControl;

	bool doWeThinkItIsOn;
};

class OneSwitcherOneLampWithDelay : public OneSwitcherOneLamp
{
public:
	OneSwitcherOneLampWithDelay(int connectors, int lamp, DelayedTurnOff* i_pDTS) : OneSwitcherOneLamp(connectors, lamp), pDTS(i_pDTS)
	{

	}

	bool processIncomingSignal(int id, bool isTurnOn, CommandDesc& command)
	{
		if (id == switchId)
		{
			bool lampState = LampState::GetLamp()[lampId];
			lampState = !lampState;
			if (lampState)
			{
				LampState::GetLamp()[lampId] = lampState;
				command.addSingleCommand(lampId, lampState);
				return true;
			}
			else
			{
				pDTS->activate();
				return false;
			}

		}
		else
			return false;
	}

private:

	DelayedTurnOff* pDTS;
};