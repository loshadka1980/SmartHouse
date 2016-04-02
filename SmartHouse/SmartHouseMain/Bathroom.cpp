#include "stdafx.h"
#include "Bathroom.h"

const int ventIndex = 1;
const int rr1 = 2;
const int rr2 = 3;
const int rr3 = 4;

void Bathroom::turnLightandVentOn(CommandDesc& command)
{
	// turn light in Bathroom on
	command.addSingleCommand(lampB1Id, true);
	command.addSingleCommand(lampB2Id, true);

	// turn vent on
	command.addSingleCommand(ventId, true);
	dam.cancelDelayedEvent(ventIndex);
}

bool Bathroom::processIncomingSignal(int id, bool isTurnOn, CommandDesc& command)
{
	if (id == motionDetectorBathroom && isTurnOn)
	{
		dam.cancelDelayedEvent(rr1); // clean monitor
		dam.cancelDelayedEvent(rr2);
		dam.cancelDelayedEvent(rr3);
		dam.triggerDelayedEvent(rr1, lampB1Id, false, 1200000); // start count again
		dam.triggerDelayedEvent(rr2, lampB2Id, false, 1200000);
		dam.triggerDelayedEvent(rr3, ventId, false, 1200000);

		if (!isLightOnInBathroom())
			turnLightandVentOn(command);
	}
	
	if (id == switchBR || (id == doorBR && isTurnOn /* = door is opening */ && !isLightOnInBathroom()))
	{
		if (isLightOnInBathroom())
		{
			// turn it off
			command.addSingleCommand(lampB1Id, false);
			command.addSingleCommand(lampB2Id, false);

			// check if light in WC is off - turn off vent as well. 
			if (!isLightOnInWC())
			{
				// command.addSingleCommand(ventId, false);
				dam.triggerDelayedEvent(ventIndex, ventId, false, 60000);
			}
		}
		else
		{
			turnLightandVentOn(command);
		}
		return true;
	}

	if (id == switchWC || (id == doorWC && isTurnOn /* = door is opening */ && !isLightOnInWC()))
	{
		if (isLightOnInWC())
		{
			// turn it off
			command.addSingleCommand(lampWCId, false);

			// check if light in Bathroom is off - turn off vent as well. 
			if (!isLightOnInBathroom())
			{
				// command.addSingleCommand(ventId, false);
				dam.triggerDelayedEvent(ventIndex, ventId, false, 60000);
			}
		}
		else
		{
			// turn light in WC on
			command.addSingleCommand(lampWCId, true);

			// turn vent on
			command.addSingleCommand(ventId, true);
			dam.cancelDelayedEvent(ventIndex);
		}
		return true;
	}

	return false;

}