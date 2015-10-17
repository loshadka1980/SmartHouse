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

	void addSingleCommand(const SingleCommandDesc& scmd)
	{
		listOfCommands.push_back(scmd);
	}

	bool isEmpty(void)
	{
		return listOfCommands.empty();
	}

	std::vector<SingleCommandDesc> listOfCommands;
};