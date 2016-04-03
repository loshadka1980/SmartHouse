#include "stdafx.h"
#include "Logger.h"
#include <fstream>
#include <ctime>


Logger* s = nullptr;

Logger& Logger::Get(void)
{
	if (s == nullptr)
		s = new Logger();

	return *s;
}
void Logger::record(int channelId, std::string channelName, bool isOn)
{
	std::fstream f;
	f.open(filename, std::fstream::out | std::fstream::app);

	time_t seconds = time(NULL);
	tm timeinfo;
	localtime_s(&timeinfo,  &seconds);

	char buffer[32];
	errno_t errNum;
	errNum = asctime_s(buffer, 32, &timeinfo);
	if (errNum)
		buffer[0] = 0;
	else
	{
		int l = strlen(buffer);
		if (l > 0)
			buffer[l - 1] = 0;
	}

	f << buffer << ", " << channelId << ", " << channelName << ", " << std::boolalpha << isOn << std::endl;

	f.close();


}
Logger::Logger()
{
	filename = "Smarthouse.log";
}