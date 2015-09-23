#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <ctime>
#include <map>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <cstdlib>

#include "ahidwrapper.h"
#include "Sender.h"
#include "Reciever.h"
#include "LampState.h"
#include "Commands.h"
#include "BaseConnector.h"
// #include "TwoSwitchersOneLamp.h"






class SwitchOffBoth : public BaseConnector
{
public:
	SwitchOffBoth(int sw, int lampA, int lampB) : switchId(sw), lampAId(lampA), lampBId(lampB)
	{

	}

	bool processIncomingSignal(int id, bool isTurnOn, CommandDesc& command)
	{
		if (id == switchId)
		{
			LampState& l = LampState::GetLamp();
			bool lampAState = l[lampAId];
			bool lampBState = l[lampBId];

			if (lampAState || lampBState)
			{
				// switch off both
				lampAState = false;
				lampBState = false;
			}
			else
			{
				lampAState = true;
				lampBState = true;
				// switch on both
			}
			l[lampAId] = lampAState;
			l[lampBId] = lampBState;

			//command.first = lampId;
			//command.second = lampState;
			command.addSingleCommand(lampAId, lampAState);
			command.addSingleCommand(lampBId, lampBState);
			command.addSingleCommand(lampAId, lampAState);
			command.addSingleCommand(lampBId, lampBState);
			return true;
		}
		else
			return false;
	}

private:

	int switchId;
	int lampAId;
	int lampBId;
};

class SwitchOffTree : public BaseConnector
{
public:
	SwitchOffTree(int sw, int lampA, int lampB, int lampC) : switchId(sw), lampAId(lampA), lampBId(lampB), lampCId(lampC)
	{

	}

	bool processIncomingSignal(int id, bool isTurnOn, CommandDesc& command)
	{
		if (id == switchId)
		{
			LampState& l = LampState::GetLamp();
			bool lampAState = l[lampAId];
			bool lampBState = l[lampBId];
			bool lampCState = l[lampCId];

			if (lampAState || lampBState || lampCState)
			{
				// switch off both
				lampAState = false;
				lampBState = false;
				lampCState = false;
			}
			else
			{
				lampAState = true;
				lampBState = true;
				lampCState = true;
				// switch on both
			}
			l[lampAId] = lampAState;
			l[lampBId] = lampBState;
			l[lampCId] = lampCState;

			//command.first = lampId;
			//command.second = lampState;
			command.addSingleCommand(lampAId, lampAState);
			command.addSingleCommand(lampBId, lampBState);
			command.addSingleCommand(lampCId, lampCState);
			return true;
		}
		else
			return false;
	}

private:

	int switchId;
	int lampAId;
	int lampBId;
	int lampCId;
};

class SingleSwitcher : public BaseConnector
{
public:
	SingleSwitcher(int sw, int lamp) : /*lampState(false),*/ switchId(sw), lampId(lamp)
	{

	}

	bool processIncomingSignal(int id, bool isTurnOn, CommandDesc& command)
	{
		if (id == switchId)
		{
			bool lampState = LampState::GetLamp()[lampId];
			lampState= !lampState;
			LampState::GetLamp()[lampId] = lampState;

			command.addSingleCommand(lampId, lampState);
			//command.first = lampId;
			//command.second = lampState;
			return true;
		}
		else
			return false;
	}

protected:
	
	// bool state;

	int switchId;

	int lampId;
};


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


class SingleSwitcherWithDelay : public SingleSwitcher
{
public:
	SingleSwitcherWithDelay(int sw, int lamp, DelayedTurnOff* i_pDTS) : SingleSwitcher(sw, lamp), pDTS(i_pDTS)
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

			//command.first = lampId;
			//command.second = lampState;
			
		}
		else
			return false;
	}

private:

	DelayedTurnOff* pDTS;
};

const int timeoutSender = 250; // milliseconds

const int serviceEventsBase = 701;

int _tmain(int argc, _TCHAR* argv[])
{

	std::cout << "-- Sender initialization" << std::endl;

	Sender* stemp = nullptr;
	try
	{
		stemp = new Sender();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::exit(2);
	}

	Sender& s = *stemp;

	std::cout << "-- Sender check" << std::endl;

	// s.names = std::map < std::string, int>({ { "Exp", 2 }, { "BedRoomYana", 3 }, { "BedRoomRoma", 4 } });
	std::map < std::string, int> Lamps({ { "Exp", 2 }, { "BedroomYana", 3 }, { "BedroomRoma", 4 }, { "Vent", 5 }, { "Koridor", 7 }, { "BedroomCeiling", 8 }, { "AnteroomSmall", 9 }, { "AnteroomLarge", 10 }, { "BathroomWall", 11 }, { "BathroomCeiling", 12 }, {"WC", 13} });
	s.names = Lamps;

	std::for_each(s.names.begin(), s.names.end(),
		[&s](const std::pair< std::string, int> pair)
	{
		s.issueCommandByID(pair.second, true);
		std::cout << "name = " << pair.first << "			id = " << pair.second << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(timeoutSender));
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	std::for_each(s.names.begin(), s.names.end(),
		[&s](const std::pair< std::string, int> pair)
	{
		s.issueCommandByID(pair.second, false);
		std::this_thread::sleep_for(std::chrono::milliseconds(timeoutSender));
	});

//
	

	std::map <int, std::string> SwReverse{ { 2, "LeftExp" }, { 3, "RightExp" }, { 4, "BedroomYana" }, { 5, "BedroomRoma" }, { 6, "BedroomCmn" }, { 7, "BedroomDoor1" }, { 8, "BedroomDoor2" }, { 9, "BedroomDoor3" }, { 10, "Enterance1" }, { 11, "Enterance2" }, { 12, "Bathroom2" }, { 13, "Bathroom1WC" } }; // в софтине к номерам прибавл +1 (нумерация здесь от 0)
	std::map<std::string, int> Sw;
	for (auto it : SwReverse)
	{
		Sw[it.second] = it.first;
	}
	
	std::map <int, bool> senderStatus;	
	

	// std::map < std::string, int> recieverNames{ { "Exp", 2 },  {"BedRoomYana", 3 }, { "BedRoomRoma", 4 } }; // в софтине к номерам прибавл +1 (нумерация здесь от 0)
	
	
	int count = 0;

	//_getch();
	std::cout << "--Main Loop" << std::endl;

	Reciever r;

	//TwoSwitchers ts(2, 3, 2);
	// SingleSwitcher ss1(4, 3);
	SingleSwitcher ss1(Sw["BedroomRoma"], Lamps["BedroomRoma"]);
	
	// SingleSwitcher ss2(5, 4);
	SingleSwitcher ss2(Sw["BedroomYana"], Lamps["BedroomYana"]);
	
	//SingleSwitcher es1(2, 3);
	//SingleSwitcher es2(3, 4);
	
	// SwitchOffBoth swBoth(6, 3, 4);
	SwitchOffBoth swBoth(Sw["BedroomCmn"], Lamps["BedroomRoma"], Lamps["BedroomYana"]);

	SingleSwitcher a1(Sw["BedroomDoor1"], Lamps["BedroomCeiling"]);
	SingleSwitcher a2(Sw["BedroomDoor2"], Lamps["BedroomYana"]);
	SingleSwitcher a3(Sw["BedroomDoor3"], Lamps["BedroomRoma"]);

	SingleSwitcher b1(Sw["Enterance1"], Lamps["AnteroomSmall"]);
	SingleSwitcher b2(Sw["Enterance2"], Lamps["AnteroomLarge"]);

	SwitchOffBoth c1(Sw["Bathroom2"], Lamps["BathroomWall"], Lamps["BathroomCeiling"]);
	SingleSwitcher c2(Sw["Bathroom1WC"], Lamps["WC"]);
	
	// DelayedTurnOff delayTest(4, std::chrono::seconds(3));
	//DelayedTurnOff delayVentOff(4, std::chrono::seconds(20));
	MonitorOtherLamps mol(Lamps["BathroomWall"], Lamps["WC"], Lamps["Vent"]);
	// SingleSwitcherWithDelay sswd(5, 4, &delayTest);

	std::vector<BaseConnector*> sw; // { /*&ts, &ss1, &ss2, &es1, &es2, &swBoth };
    sw.push_back(&ss1);
	sw.push_back(&ss2);
	sw.push_back(&swBoth);
	// sw.push_back(&sswd);
	sw.push_back(&a1);
	sw.push_back(&a2);
	sw.push_back(&a3);

	sw.push_back(&b1);
	sw.push_back(&b2);

	sw.push_back(&c1);
	sw.push_back(&c2);

	std::vector<BaseTimer*> swTimer;
	// swTimer.push_back(&delayTest);
	// swTimer.push_back(&delayVentOff);
	// swTimer.push_back(&mol);

	while (true)
	{

		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		count++;
		// std::cout << "count " << count << std::endl;
		for (auto timerIt : swTimer)
		{
			CommandDesc cmd;
			if (timerIt->processTimerSignal(cmd))
			{
				for (auto scmd : cmd.listOfCommands)
				{
					// std::cout << "Issue command  id = " << scmd.first << " indication = " << scmd.second << std::endl;
					s.issueCommandByID(scmd.first, scmd.second);
					std::this_thread::sleep_for(std::chrono::milliseconds(timeoutSender));
				}
			}
		}


		int sender = -1;
		bool newStatus = false;
		bool isNonIncremental = false;
		
		if (!r.obtainCommand(sender, newStatus, isNonIncremental))
		{
			//std::cout << "Failed to obtain command. Skip further actions. " << std::endl;
			continue;
		}

		if (isNonIncremental)
		{
			std::cout << "New togl is not incremental from the previous one, initialization or missed some commands. Skip further actions. " << std::endl;
			continue;
		}


		auto it = SwReverse.find(sender);

		if (it == SwReverse.end())
		{
			std::cout << "SenderId = " << sender << ", SenderName unknown" << std::endl;
		}
		else
		{
			std::cout << "SenderId = " << sender << ", SenderName = " << it->second << std::endl;
		} 


		//----------------------------

		auto it2 = senderStatus.find(sender);
		if (it2 == senderStatus.end())
		{
			std::cout << "First command from senderId = " << sender << std::endl;
			senderStatus[sender] = newStatus;
		}
		else if (it2->second == newStatus)
		{
			std::cout << "Received same status, may be missed signal from senderId = " << sender << std::endl;
		}
		else
		{
			it2->second = newStatus;
			// std::cout << "Regular status change from senderId = " << sender << std::endl;
		}

		//s.issueCommand(2, newStatus);

		/*
		CommandDesc cmd;
		if (ts.processIncomingSignal(sender, newStatus, cmd))
		{
			s.issueCommand(cmd.first, cmd.second);
		} 
		*/

		auto sit = sw.begin();

		while (sit != sw.end())
		{
			CommandDesc cmd;
			if ((*sit)->processIncomingSignal(sender, newStatus, cmd))
			{
				for (auto scmd : cmd.listOfCommands)
				{
					// std::cout << "Issue command  id = " << scmd.first << " indication = " << scmd.second << std::endl;
					s.issueCommandByID(scmd.first, scmd.second);
					std::this_thread::sleep_for(std::chrono::milliseconds(timeoutSender));
				}
			//	std::this_thread::sleep_for(std::chrono::milliseconds(30));
			//	s.issueCommand(cmd.first, cmd.second);
			}

			++sit;
		}


		

		

		
	}

	_getch();
	return 0;
}

