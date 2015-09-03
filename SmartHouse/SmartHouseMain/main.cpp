#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <map>
#include <string>

#include "ahidwrapper.h"
#include "sender.h"
#include "reciever.h"

typedef std::pair<int, bool> CommandDesc;

class TwoSwitchers
{
public:
	TwoSwitchers(int swA, int swB, int lamp) : lampState(false), stateA(false), stateB(false), switchAId(swA), switchBId(swB), lampId(lamp)
	{

	}
	
	bool processIncomingSignal(int id, bool isTurnOn, CommandDesc& command)
	{
		if (id == switchAId || id == switchBId)
		{
			lampState = !lampState;
			command.first = lampId;
			command.second = lampState;
			return true;
		}
		else 
			return false;
	}

private:
	bool lampState;
	bool stateA;
	bool stateB;

	int switchAId;
	int switchBId;
	int lampId;
};


int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "--Sender check" << std::endl;

	Sender s;
	s.issueCommand(2, true);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	s.issueCommand(2, false);
	
	//_getch();

	std::cout << "--Reciever" << std::endl;
	
	

	std::map <int, std::string> senderNames{ { 2, "Left" }, { 3, "Right" } };
	std::map <int, bool> senderStatus;	
	
	
	int count = 0;

	Reciever r;

	TwoSwitchers ts(2, 3, 2);

	while (count != 500)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		count++;
		// std::cout << "count " << count << std::endl;


		int sender = -1;
		bool newStatus = false;
		bool isNonIncremental = false;
		
		if (!r.obtainCommand(sender, newStatus, isNonIncremental))
			continue;

		if (isNonIncremental)
		{
			std::cout << "New togl is not incremental from the previous one, initialization or missed some commands. Skip further actions. " << std::endl;
			continue;
		}


		auto it = senderNames.find(sender);

		if (it == senderNames.end())
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
			std::cout << "Regular status change from senderId = " << sender << std::endl;
		}

		//s.issueCommand(2, newStatus);

		CommandDesc cmd;
		if (ts.processIncomingSignal(sender, newStatus, cmd))
		{
			s.issueCommand(cmd.first, cmd.second);
		}



		

		

		
	}

	_getch();
	return 0;
}

