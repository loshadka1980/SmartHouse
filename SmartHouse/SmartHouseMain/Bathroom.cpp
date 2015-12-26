#include "stdafx.h"
#include "Bathroom.h"

bool Bathroom::processIncomingSignal(int id, bool isTurnOn, CommandDesc& command)
{
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
				dam.triggerDelayedEvent(ventId, false, 60000);
			}
		}
		else
		{
			// turn light in Bathroom on
			command.addSingleCommand(lampB1Id, true);
			command.addSingleCommand(lampB2Id, true);

			// turn vent on
			command.addSingleCommand(ventId, true);
			dam.cancelDelayedEvent();
			
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
				dam.triggerDelayedEvent(ventId, false, 60000);
			}
		}
		else
		{
			// turn light in WC on
			command.addSingleCommand(lampWCId, true);

			// turn vent on
			command.addSingleCommand(ventId, true);
			dam.cancelDelayedEvent();
		}
		return true;
	}

	return false;

}