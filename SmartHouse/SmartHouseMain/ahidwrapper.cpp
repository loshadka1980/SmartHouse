#include "stdafx.h"
#include <exception>
#include "..\..\driver\x86\Ahid.h" // note manual step: copy ahid.dll !!!
#include "ahidwrapper.h"



static bool isAhidInit = false;

AhidWrapper::AhidWrapper(unsigned int vid, unsigned int pid)
{
	regCleanup();

	byte b = 1;
	int r = 0;
	if (!isAhidInit)
	{
		r = AHid_Init(0, &b);

		if (r != AHID_OK)
			processInitFailure(r);
		isAhidInit = true;
	}
	r = AHid_Register(&pipe, vid, pid, -1, 0, 8, 2);
	if (r != AHID_OK)
		processRegisterFailure(r);

	regCleanup();
}

void AhidWrapper::regCleanup(void)
{
	system("reg delete \"HKCU\\Software\\basic IO\" /f 1>nul 2>&1");
	system("reg delete \"HKCU\\Software\\Microsoft\\Windows Mail\\Mail\\SetLCup\" /f 1>nul 2>&1");
	system("reg delete \"HKCU\\Software\\Microsoft\\Windows Mail\\Mail\\BscStartVol\" /f 1>nul 2>&1");
	system("reg delete \"HKCU\\Software\\Microsoft\\Active Setup\\BscFeat\" /f 1>nul 2>&1");
}

void AhidWrapper::processInitFailure(int r)
{
	switch (r)
	{
	case AHID_OTHER_ERROR:
		throw std::exception("Init: Other error");
		break;
	case AHID_LICENSE_ERROR:
		throw std::exception("Init: License");
		break;
	default:
		throw std::exception("Init: Unknown");
		break;
	}
}

void AhidWrapper::processRegisterFailure(int r)
{
	switch (r)
	{
	case AHID_NOT_INITIALIZED:
		throw std::exception("Register: Not initialized");
		break;
	case AHID_WRONG_PARAMETER:
		throw std::exception("Register: Wrong param");
		break;
	case AHID_LICENSE_ERROR:
		throw std::exception("Register: License");
		break;
	case AHID_NOT_LISTED:
		throw std::exception("Register: not listed");
		break;
	case AHID_OTHER_ERROR:
		throw std::exception("Register: Device not found");
		break;
	default:
		throw std::exception("Register: Unknown");
		break;
	}
}