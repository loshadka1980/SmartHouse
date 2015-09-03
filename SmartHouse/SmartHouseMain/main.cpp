#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <map>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

#include "ahidwrapper.h"
#include "sender.h"
#include "reciever.h"
#include "LampState.h"

typedef std::pair<int, bool> SingleCommandDesc;

struct CommandDesc
{
	CommandDesc(){};
	CommandDesc(const SingleCommandDesc& singleCommand){ listOfCommands.push_back(singleCommand); }
	void addSingleCommand(int id, bool status)
	{
		listOfCommands.push_back(std::pair<int, bool>(id, status));
	}

	std::vector<SingleCommandDesc> listOfCommands;
};


class BaseSwitcher
{
public:
	virtual bool processIncomingSignal(int id, bool isTurnOn, CommandDesc& command) = 0;
};

class TwoSwitchers: public BaseSwitcher
{
public:
	TwoSwitchers(int swA, int swB, int lamp) :  switchAId(swA), switchBId(swB), lampId(lamp)
	{

	}
	
	bool processIncomingSignal(int id, bool isTurnOn, CommandDesc& command)
	{
		if (id == switchAId || id == switchBId)
		{
			bool lampState = LampState::GetLamp()[lampId];
			lampState = !lampState;
			LampState::GetLamp()[lampId] = lampState;


			command.addSingleCommand(lampId, lampState);

			// command.first = lampId;
			// command.second = lampState;
			return true;
		}
		else 
			return false;
	}

private:

	int switchAId;
	int switchBId;
	int lampId;
};

class SwitchOffBoth : public BaseSwitcher
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

class SingleSwitcher : public BaseSwitcher
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

private:
	
	bool state;

	int switchId;

	int lampId;
};

const int timeoutSender = 200; // milliseconds

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "--Sender check" << std::endl;

	Sender s;
	// s.names = std::map < std::string, int>({ { "Exp", 2 }, { "BedRoomYana", 3 }, { "BedRoomRoma", 4 } });
	s.names = std::map < std::string, int>({ { "Exp", 2 }, { "BedRoomYana", 3 }, { "BedRoomRoma", 4 }, { "Vent", 5 }, { "Koridor", 7 } });

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
	


	
	

	std::map <int, std::string> recivierSourceNames{ { 2, "LeftExp" }, { 3, "RightExp" }, { 4, "BedRoomYana" }, { 5, "BedRoomRoma" }, { 6, "BedRoomCmn" } }; // в софтине к номерам прибавл +1 (нумерация здесь от 0)
	
	std::map <int, bool> senderStatus;	
	

	// std::map < std::string, int> recieverNames{ { "Exp", 2 },  {"BedRoomYana", 3 }, { "BedRoomRoma", 4 } }; // в софтине к номерам прибавл +1 (нумерация здесь от 0)
	
	
	int count = 0;

	//_getch();
	std::cout << "--Main Loop" << std::endl;

	Reciever r;

	//TwoSwitchers ts(2, 3, 2);
	SingleSwitcher ss1(4, 3);
	SingleSwitcher ss2(5, 4);
	//SingleSwitcher es1(2, 3);
	//SingleSwitcher es2(3, 4);
	SwitchOffBoth swBoth(6, 3, 4);

	std::vector<BaseSwitcher*> sw; // { /*&ts, &ss1, &ss2, &es1, &es2, &swBoth };
	sw.push_back(&ss1);
	sw.push_back(&ss2);
	sw.push_back(&swBoth);

	while (true)
	{

		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		count++;
		// std::cout << "count " << count << std::endl;


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


		auto it = recivierSourceNames.find(sender);

		if (it == recivierSourceNames.end())
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

