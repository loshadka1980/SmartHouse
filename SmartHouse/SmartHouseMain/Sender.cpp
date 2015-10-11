#include "stdafx.h"
#include <exception>
#include <thread>
#include <chrono>
#include <algorithm>
#include <iostream>
#include "sender.h"
#include "LampState.h"
#include "..\..\driver\x86\Ahid.h"

void Sender::issueCommand(const std::string &channelName, const bool turnOn)
{
	auto it = names.find(channelName);
	if (it != names.end())
		issueCommandByID(it->second, turnOn);
	else
		throw std::exception("Wrong Name");
}

void Sender::issueCommandByID(const int channel, const bool turnOn)
{
	if (channel > 64 || channel < 0)
		throw("Send(): Invalid channel ID");

	byte buf2[8];
	unsigned int actWr = 0;

	buf2[0] = 0 * 1 + 2 * 8 + 2 * 32;

	if (turnOn)
		buf2[1] = 2; // turn on	sharply
	else
		buf2[1] = 0; // turn off sharply

	buf2[2] = 0; // format

	buf2[3] = 0;
	buf2[4] = channel; // 

	buf2[5] = 155;
	buf2[6] = 155;
	buf2[7] = 155;


	int r = AHid_Write(pipe, buf2, 8, &actWr);

	if (r != AHID_OK)
		processSendFailure(r);

	if (actWr != 8)
		throw("Send(): Invalid num of bytes sent");


	LampState& l = LampState::GetLamp();
	l.setState(channel, turnOn);
}

void Sender::check(void)
{
	std::for_each(names.begin(), names.end(),
		[this](const std::pair< std::string, int> pair)
	{
		issueCommandByID(pair.second, true);
		std::cout << "name = " << pair.first << "			id = " << pair.second << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(timeoutSender));
	});

	std::cout << "Waiting for " << checkWait / 1000.0 << " seconds..." << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(checkWait));
	std::cout << "...complete." << std::endl;

	std::for_each(names.begin(), names.end(),
		[this](const std::pair< std::string, int> pair)
	{
		issueCommandByID(pair.second, false);
		std::this_thread::sleep_for(std::chrono::milliseconds(timeoutSender));
	});
}

void Sender::processSendFailure(int r)
{
	switch (r)
	{
	case AHID_NOT_INITIALIZED:
		throw std::exception("Send: Not initialized");
		break;
	case AHID_WRONG_PARAMETER:
		throw std::exception("Send: pipe / num of bytes wrong");
		break;
	case AHID_IO_ERROR:
		throw std::exception("Send: IO Error");
		break;
	case AHID_NOT_LISTED:
		throw std::exception("Send: Pipe not listed");
		break;
	case AHID_OTHER_ERROR:
		throw std::exception("Send: Other error");
		break;
	default:
		throw std::exception("Send: Unknown");
		break;
	}
}