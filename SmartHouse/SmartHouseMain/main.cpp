// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <map>
#include <string>

#include "..\..\driver\x86\Ahid.h"

// manual step: copy ahid.dll

/*
class AHid_api
{
	public: 
	__declspec(dllimport) static int AHid_Init(int, char*);
};
*/
/*
[DllImport("AHid.dll", CallingConvention = CallingConvention::Cdecl)] static int AHid_Register(int*, unsigned int, unsigned int, int, byte, byte, byte);
[DllImport("AHid.dll", CallingConvention = CallingConvention::Cdecl)] static int AHid_Deregister(int);
[DllImport("AHid.dll", CallingConvention = CallingConvention::Cdecl)] static int AHid_Write(int, byte*, unsigned int, unsigned int*);
[DllImport("AHid.dll", CallingConvention = CallingConvention::Cdecl)] static int AHid_Read(int, byte*, unsigned int, unsigned int*);
[DllImport("AHid.dll", CallingConvention = CallingConvention::Cdecl)] static int AHid_Attached(int, int, int);
[DllImport("AHid.dll", CallingConvention = CallingConvention::Cdecl)] static int AHid_Request(int);
[DllImport("AHid.dll", CallingConvention = CallingConvention::Cdecl)] static int AHid_Find(int);
[DllImport("AHid.dll", CallingConvention = CallingConvention::Cdecl)] static int AHid_Info(void);
*/

int _tmain(int argc, _TCHAR* argv[])
{
	std::map <int, std::string> senderNames{ { 2, "Experimental" } };
	std::map <int, bool> senderStatus;

	std::cout << "Hello, world!" << std::endl;
	byte b;
	int res1 = AHid_Init(0, &b);
	switch (res1)
	{
	case AHID_OK:
		std::cout << "Init Ok" << std::endl;
		break;
	case AHID_OTHER_ERROR:
		std::cout << "Init Other error" << std::endl; 
		break;
	case AHID_LICENSE_ERROR:
		std::cout << "Init License" << std::endl;
		break;
	default:
		std::cout << "Unknown " << std::endl;
		break;
	}

	int pipe;
	unsigned int vid = 5824;
	unsigned int pid = 1500;

	int res2 = AHid_Register(&pipe, vid, pid, -1, 0, 8, 2);

	switch (res2)
	{
	case AHID_OK:
		std::cout << "Register Ok" << std::endl;
		break;
	case AHID_NOT_INITIALIZED:
		std::cout << "Register not initialized" << std::endl;
		break;
	case AHID_WRONG_PARAMETER:
		std::cout << "Register wrong param" << std::endl;
		break;
	case AHID_LICENSE_ERROR:
		std::cout << "Register license" << std::endl;
		break;
	case AHID_NOT_LISTED:
		std::cout << "Register not listed" << std::endl;
		break;
	case AHID_OTHER_ERROR:
		std::cout << "Register Device not found" << std::endl;
		break;
	default:
		std::cout << "Register Unknown " << std::endl;
		break;
	}

	/*** AHid_Read **************************************************************************************

	Purpose:		Read data from USB device.

	Parameters:		pipe					- The data pipe.
	buffer					- Transfer buffer.
	to_read					- Number of bytes to read.
	*read					- Reference to the number of bytes read.

	Return values:	AHID_NOT_INITIALIZED	- AHID.DLL not initialised yet.
	AHID_WRONG_PARAMETER	- The pipe value is not valid or the number of bytes to read
	are not a multiple of the report size (set in AHid_Register()).
	AHID_NOT_LISTED			- Data pipe not registered.
	AHID_IO_ERROR			- Read failed.
	AHID_OK					- No error.

	****************************************************************************************************/
	
	int count = 0;

	int oldTogl = 0;

	while (count != 50)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(2200));
		count++;
		std::cout << "count " << count << std::endl;

		int res5 = AHid_Request(pipe);
		switch (res5)
		{
		case AHID_OK:
			std::cout << "Req Ok" << std::endl;
			break;
		case AHID_NOT_INITIALIZED:
			std::cout << "Req not initialized" << std::endl;
			break;
		case AHID_WRONG_PARAMETER:
			std::cout << "Req pipe / num of bytes wrong" << std::endl;
			break;
		case AHID_IO_ERROR:
			std::cout << "Req IO error" << std::endl;
			break;
		case AHID_NOT_LISTED:
			std::cout << "Req pipe not listed" << std::endl;
			break;
		case AHID_OTHER_ERROR:
			std::cout << "Req Other error" << std::endl;
			break;
		default:
			std::cout << "Req Unknown " << std::endl;
			break;
		}


		/*
		int res4 = AHid_Setup(pipe);
		switch (res4)
		{
		case AHID_OK:
			std::cout << "Setup Ok" << std::endl;
			break;
		case AHID_NOT_INITIALIZED:
			std::cout << "Setup not initialized" << std::endl;
			break;
		case AHID_WRONG_PARAMETER:
			std::cout << "Setup pipe / num of bytes wrong" << std::endl;
			break;
		case AHID_IO_ERROR:
			std::cout << "Setup IO error" << std::endl;
			break;
		case AHID_NOT_LISTED:
			std::cout << "Setup pipe not listed" << std::endl;
			break;
		case AHID_OTHER_ERROR:
			std::cout << "Setup Other error" << std::endl;
			break;
		default:
			std::cout << "Setup Unknown " << std::endl;
			break;
		}
		*/

		byte buf[8];
		unsigned int actRead = 0;
		int res3 = AHid_Read(pipe, buf, 8, &actRead);

		switch (res3)
		{
		case AHID_OK:
			std::cout << "Read Ok" << std::endl;
			break;
		case AHID_NOT_INITIALIZED:
			std::cout << "Read not initialized" << std::endl;
			break;
		case AHID_WRONG_PARAMETER:
			std::cout << "Read pipe / num of bytes wrong" << std::endl;
			break;
		case AHID_IO_ERROR:
			std::cout << "Read IO error" << std::endl;
			break;
		case AHID_NOT_LISTED:
			std::cout << "Read pipe not listed" << std::endl;
			break;
		case AHID_OTHER_ERROR:
			std::cout << "Read Other error" << std::endl;
			break;
		default:
			std::cout << "Read Unknown " << std::endl;
			break;
		}

		std::cout << "Act read " << actRead << std::endl;

		// -----------------------
		int togl = buf[0] & 63;
		if (togl != oldTogl)
		{
			if (togl != oldTogl + 1 && !(togl == 0 && oldTogl == 63))
			{
				std::cout << "New togl is not incremental from the previous one, initialization or missed some commands " << togl << std::endl;
			}
			std::cout << "New togl = " << togl << std::endl;
			oldTogl = togl;
		}
		else
		{
			std::cout << "Same togl" << std::endl;
			continue;
		}

		// ---------------------

		int sender = buf[1];

		auto it = senderNames.find(sender);

		if (it == senderNames.end())
		{
			std::cout << "SenderId = " << sender << ", SenderName unknown" << std::endl;
		}
		else
		{
			std::cout << "SenderId = " << sender << ", SenderName = " << it->second << std::endl;
		}

		// -------------

		int command = buf[2];
		bool newStatus;
		switch (command)
		{
		case 0: //off
			newStatus = false;
			std::cout << "Command = OFF" << std::endl;
			break;
		case 2: // on
			newStatus = true;
			std::cout << "Command = ON" << std::endl;
			break;
		default:
			std::cout << "Unknown command" << std::endl;
			continue;
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



		

		

		
	}

	_getch();
	return 0;
}

