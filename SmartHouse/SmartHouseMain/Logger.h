#pragma once
#include <string>

class Logger
{
public:
	static Logger& Get(void);
	// ���������� ������ ������� ������ / ������������ ����������
	void record(int channelId, std::string channelName, bool isOn);

private:
	Logger();
	std::string filename;
	

};