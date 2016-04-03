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
#include "OneSwitcherTwoLamps.h"
#include "OneSwitcherOneLamp.h"
#include "SwitcherInBedroom.h"
#include "Bathroom.h"

#include <thread>
// #include <conio.h>
#include <windows.h>
#include <mutex>

#include "DelayedActionsManager.h"
#include "Logger.h"


std::mutex gs;
/*
void issueCmdWithDelay_detached(SingleCommandDesc& scmd, std::chrono::seconds& delay, Sender& s, int hid)
{
	std::this_thread::sleep_for(delay);
	LampState& ls = LampState::GetLamp();
	std::cout << "A:" << std::endl;
	bool anyChanges = ls.checkHook(hid);
	std::cout << "B:" << std::endl;
	if (!anyChanges)
	{
		std::cout << "C:" << std::endl;
		std::lock_guard<std::mutex> l(gs);
		std::cout << "D:" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(Sender::timeoutSender));
		std::cout << "E:" << std::endl;
		s.issueCommandByID(scmd.first, scmd.second);
		std::cout << "F:" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(Sender::timeoutSender));
		std::cout << "G:" << std::endl;
	}
	std::cout << "H:" << std::endl;
	ls.deregisterHook(hid);
	std::cout << "J:" << std::endl;
}

void issueCmdWithDelay(const SingleCommandDesc& scmd, std::chrono::seconds& delay, Sender& s)
{
	
	LampState& ls = LampState::GetLamp();
	int hid = ls.registerHook(scmd.first);

	std::thread t(issueCmdWithDelay_detached, scmd, delay, s, hid);
}

*/

void issueAllCmds(const CommandDesc allCmds, SenderAPI* s)
{
	std::lock_guard<std::mutex> l(gs);

	for (auto scmd = allCmds.listOfCommands.begin(); scmd != allCmds.listOfCommands.end(); ++scmd)
	{
		if (scmd != allCmds.listOfCommands.begin())
			std::this_thread::sleep_for(std::chrono::milliseconds(Sender::timeoutSender));
		
		s->issueCommandByID(scmd->first, scmd->second);	

	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
	std::cout << "-- Sender initialization" << std::endl;

	Sender* sPReal = nullptr;
	TestSender* sPTest = nullptr;

	SenderAPI* s = nullptr; 
	
	bool testImpl = false;

	std::string current_exec_name = argv[0]; // Name of the current exec program
	std::string first_arge;
	std::vector<std::string> all_args;

	if (argc > 1) 
	{
		first_arge = argv[1];
		all_args.assign(argv + 1, argv + argc);
	}

	if (first_arge == "-test")
		testImpl = true;

	
	if (!testImpl)
	{
		sPReal = new Sender();
		s = sPReal;
	}
	else
	{
		sPTest = new TestSender();
		s = sPTest;
	}



	std::cout << "-- Sender check" << std::endl;

	std::map < std::string, int> Lamps({ { "Exp", 2 }, { "BedroomYana", 3 }, { "BedroomRoma", 4 }, { "Vent", 5 }, 
	{ "Koridor", 7 }, { "BedroomCeiling", 8 }, { "AnteroomSmall", 9 }, { "AnteroomLarge", 10 }, 
	{ "BathroomWall", 11 }, { "BathroomCeiling", 12 }, { "WC", 13 } }); // в софтине к номерам прибавл +1 (нумерация здесь от 0)
	s->names = Lamps;
	s->check();
	

	//


	std::map <int, std::string> SwReverse{ { 2, "LeftExp" }, { 3, "RightExp" }, { 4, "BedroomYana" }, { 5, "BedroomRoma" }, { 6, "BedroomCmn" },
	{ 7, "BedroomDoor1" }, { 8, "BedroomDoor2" }, { 9, "BedroomDoor3" }, { 10, "Enterance1" }, { 11, "Enterance2" },
	{ 12, "Bathroom2" }, { 13, "Bathroom1WC" }, { 14, "BathroomDoor" }, { 15, "MotDetBathroom" }, { 16, "WcDoor" },
	{17, "LockerLeft"}, {18, "LockerRight"},
	{ 19, "MotDetInsideBathroom" }, {20, "MotDetInsideToilet"} }; // в софтине к номерам прибавл +1 (нумерация здесь от 0)
	std::map<std::string, int> Sw;
	for (auto it : SwReverse)
	{
		Sw[it.second] = it.first;
	}


	std::cout << "--Reciever Init" << std::endl;

	Reciever* rPReal;
	TestReciever* rPTest;

	RecieverAPI* r;
	if (!testImpl)
	{
		rPReal = new Reciever();
		r = rPReal;
	}
	else
	{
		rPTest = new TestReciever();
		r = rPTest;
	}

	std::cout << "--Main Loop" << std::endl;

	//------------------------
	// Delayed
	DelayedActionsManager dam;

	std::vector<BaseConnector*> connectors; 

	// Bedromm
	OneSwitcherOneLamp ss1(Sw["BedroomRoma"], Lamps["BedroomRoma"]); connectors.push_back(&ss1);
	OneSwitcherOneLamp ss2(Sw["BedroomYana"], Lamps["BedroomYana"]); connectors.push_back(&ss2);
	OneSwitcherOneLamp a1(Sw["BedroomDoor1"], Lamps["BedroomCeiling"]); connectors.push_back(&a1);
	OneSwitcherOneLamp a2(Sw["BedroomDoor2"], Lamps["BedroomYana"]); connectors.push_back(&a2);
	OneSwitcherOneLamp a3(Sw["BedroomDoor3"], Lamps["BedroomRoma"]); connectors.push_back(&a3);
	SwitcherInBedroom swCmn(Sw["BedroomCmn"], Lamps["BedroomRoma"], Lamps["BedroomYana"], Lamps["BedroomCeiling"]); connectors.push_back(&swCmn);


	// Anteroom
	OneSwitcherOneLamp b1(Sw["Enterance1"], Lamps["AnteroomSmall"]); connectors.push_back(&b1);
	OneSwitcherOneLamp b2(Sw["Enterance2"], Lamps["AnteroomLarge"]); connectors.push_back(&b2);

	// Bathroom
	// OneSwitcherTwoLamps c1(Sw["Bathroom2"], Lamps["BathroomWall"], Lamps["BathroomCeiling"]); connectors.push_back(&c1);
	// OneSwitcherOneLamp c2(Sw["Bathroom1WC"], Lamps["WC"]); connectors.push_back(&c2);
	Bathroom brm(Sw["Bathroom2"], Sw["Bathroom1WC"], Sw["BathroomDoor"], Sw["WcDoor"], Sw["MotDetInsideBathroom"], Sw["MotDetInsideToilet"], Lamps["BathroomWall"], Lamps["BathroomCeiling"], Lamps["WC"], Lamps["Vent"], dam); connectors.push_back(&brm);

	// Locker
	OneSwitcherTwoLamps l1(Sw["LockerRight"], Lamps["AnteroomSmall"], Lamps["AnteroomLarge"]); connectors.push_back(&l1);




	while (true)
	{
		CommandDesc allCmds;

		/**************************************/
		if (dam.monitor(allCmds))
		{
			issueAllCmds(allCmds, s);
		}
		/**************************************/


		int sender = -1;
		bool newStatus = false;
		bool isNonIncremental = false;

		if (!r->obtainCommand(sender, newStatus, isNonIncremental))
		{
			//std::cout << "Failed to obtain command. Skip further actions. " << std::endl;
			// hee
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			continue;
		}
		else
		if (isNonIncremental)
		{
			std::cout << "New togl is not incremental from the previous one, initialization or missed some commands. Skip further actions. " << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			continue;
		}

		
		
		// Beep(500, 50);


		auto it = SwReverse.find(sender);

		if (it == SwReverse.end())
		{
			std::cout << "SenderId = " << sender << ", SenderName unknown" << std::endl;
			Logger::Get().record(sender, "Unknown", newStatus);
		}
		else
		{
			std::cout << "SenderId = " << sender << ", SenderName = " << it->second << std::endl;
			Logger::Get().record(sender, it->second, newStatus);
		}

		

		auto sit = connectors.begin();
		while (sit != connectors.end())
		{
			CommandDesc cmd;
			if ((*sit)->processIncomingSignal(sender, newStatus, cmd))
			{
				for (auto scmd = cmd.listOfCommands.begin(); scmd != cmd.listOfCommands.end(); ++scmd)
				{
					/*
					if (scmd != cmd.listOfCommands.begin())
						std::this_thread::sleep_for(std::chrono::milliseconds(Sender::timeoutSender));

					s.issueCommandByID(scmd->first, scmd->second);
					*/
					allCmds.addSingleCommand(*scmd);
				}
			}

			++sit;
		}
		//issueAllCmds(allCmds, s);

		if (!allCmds.isEmpty())
		{
			
			issueAllCmds(allCmds, s);
			
			/*
			std::thread t(issueAllCmds, allCmds, s);
			 t.detach();
			 
			 t.join();
			 */
			
		}
		

	}

	_getch();
	return 0;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::exit(2);
	}
}

