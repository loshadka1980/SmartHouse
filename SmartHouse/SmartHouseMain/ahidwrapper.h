#pragma once

class AhidWrapper
{
public:
	AhidWrapper(unsigned int vid, unsigned int pid);

protected:
	int pipe;


	void regCleanup(void);
	void processInitFailure(int r);
	void processRegisterFailure(int r);

};