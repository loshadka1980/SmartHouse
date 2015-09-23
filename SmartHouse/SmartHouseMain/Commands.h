#pragma once
#include <vector>

using SingleCommandDesc = std::pair<int, bool>;

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