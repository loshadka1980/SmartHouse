#include "stdafx.h"
#include <exception>
#include "reciever.h"
#include "..\..\driver\x86\Ahid.h"

#include <iostream>
#include <windows.h> // for test reciever

Reciever::Reciever() : AhidWrapper(vidReciever, pidReciever), oldTogl(0)
{

}

bool Reciever:: obtainCommand(int& senderID, bool& isTurnOn, bool& isNonIncrementalFlag)
{
	isNonIncrementalFlag = false;
	senderID = -1;
	isTurnOn = false;

	int r = 0;
	r = AHid_Request(pipe);
	if (r == AHID_IO_ERROR)
	{
		std::cout << "Fail: AHID_IO error" << std::endl;
		return false;                 // frequent on/off signal may trigger this type of error
	}
	else if (r != AHID_OK)
	{
		processObtainFailure(r);
	}


	byte buf[8];
	unsigned int actRead = 0;
	r = AHid_Read(pipe, buf, 8, &actRead);

	if (r != AHID_OK)
		processObtainFailure(r);

	if (actRead != 8)
		throw std::exception("obtain(): Incorrect number of bytes read");

	// -----------------------
	int togl = buf[0] & 63;
	if (togl == oldTogl)
	{
		//std::cout << "same togl" << std::endl;
		return false;
	}


	if (togl != oldTogl + 1 && !(togl == 0 && oldTogl == 63))
	{
		isNonIncrementalFlag = true;
	}

	oldTogl = togl;

	senderID = buf[1];

	switch (buf[2])
	{
	case 0: //off
		isTurnOn = false;
		break;
	case 2: // on
		isTurnOn = true;
		break;
	case 5: // door closed
		isTurnOn = false;
		break;
	case 10: // door open
		isTurnOn = true;
		break;
	default:
		// throw std::exception("Obtain(): unknown command type");
		std::cout << "Fail: command type" << std::endl;
		return false;
	}

	return true;

}

void Reciever::processObtainFailure(int r)
{
	switch (r)
	{
	case AHID_NOT_INITIALIZED:
		throw std::exception("Obtain(): not initialized");
		break;
	case AHID_WRONG_PARAMETER:
		throw std::exception("Obtain(): pipe / num of bytes wrong");
		break;
	case AHID_IO_ERROR:
		throw std::exception("Obtain(): IO error");
		break;
	case AHID_NOT_LISTED:
		throw std::exception("Obtain(): pipe not listed");
		break;
	case AHID_OTHER_ERROR:
		throw std::exception("Obtain(): other error");
		break;
	default:
		throw std::exception("Obtain(): Unknown");
		break;
	}
}


bool iskeypressed(unsigned timeout_ms = 0)
{
	return WaitForSingleObject(
		GetStdHandle(STD_INPUT_HANDLE),
		timeout_ms
		) == WAIT_OBJECT_0;
}

bool TestReciever::obtainCommand(int& senderID, bool& isTurnOn, bool& isNonIncrementalFlag)
{
	
	if (!iskeypressed() )
		return false;
	else
	{
		int a = -1;
		std::cin >> senderID >> a;
		if (a == 0)
			isTurnOn = false;
		else if (a == 1)
			isTurnOn = true;
		else
			return false;
		std::cout << "Imitated received command from channel " << senderID << " , status = ";
		if (isTurnOn)
			std::cout << "On";
		else
			std::cout << "Off";
		std::cout << std::endl;
		return true;
	}
}