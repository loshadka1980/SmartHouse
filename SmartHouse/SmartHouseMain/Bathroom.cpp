#include "stdafx.h"
#include "Bathroom.h"

const int ventIndex = 1;
const int rr1 = 2;
const int rr2 = 3;
const int rr3 = 4;
const int rr4 = 5;
const int rr5 = 6;


const int delayedMotion = 600000; // 600 sec
const int delayedVent = 20000; // 20 sec

// 1. �������� ���� ���� 
//		a. ���� �� ����� � ������ ������
//		b. ���� �� ����� � ������� �����
//		c. ���� �� ����� � ��������������� ��������
// 2. ��������� ���� ����
//		�. ���� ����� � ������ ������
// 3. ��� ��������� ����� ������� ���������� ����������
// 4.	a. ��� ���������� ����� � � ������ � � ������� ������� ���������� ���������� �����������
//		b. ��� �������� ��������� ���������� ���������� ����������
// 5. ��� �������� ��������� ���������� ����������


void Bathroom::turnVentOn(CommandDesc& command)
{
	command.addSingleCommand(ventId, true);
	dam.cancelDelayedEvent(ventIndex); // Implements req (4b.)
}

void Bathroom::turnBMLightandVentOn(CommandDesc& command)
{
	command.addSingleCommand(lampB1Id, true);
	command.addSingleCommand(lampB2Id, true);

	turnVentOn(command);

	createDelayedTurnOffBM(); // req 3
}

void Bathroom::turnBMLightandVentOff(CommandDesc& command)
{
	command.addSingleCommand(lampB1Id, false);
	command.addSingleCommand(lampB2Id, false);

	if (!isLightOnInWC())
		dam.triggerDelayedEvent(ventIndex, ventId, false, delayedVent); // req 4a
}

void Bathroom::turnWCLightandVentOn(CommandDesc& command)
{
	command.addSingleCommand(lampWCId, true);

	turnVentOn(command);

	createDelayedTurnOffWC(); // req 3
}

void Bathroom::turnWCLightandVentOff(CommandDesc& command)
{
	command.addSingleCommand(lampWCId, false);

	if (!isLightOnInBathroom())
		dam.triggerDelayedEvent(ventIndex, ventId, false, delayedVent); // req 4a
}

void Bathroom::createDelayedTurnOffBM()
{
	dam.triggerDelayedEvent(rr1, lampB1Id, false, delayedMotion); 
	dam.triggerDelayedEvent(rr2, lampB2Id, false, delayedMotion);
	dam.triggerDelayedEvent(rr3, ventId, false, delayedMotion);
}

void Bathroom::createDelayedTurnOffWC()
{
	dam.triggerDelayedEvent(rr4, lampWCId, false, delayedMotion); 
	dam.triggerDelayedEvent(rr3, ventId, false, delayedMotion);
}

bool Bathroom::processIncomingSignal(int id, bool isTurnOn, CommandDesc& command)
{
	if (!isLightOnInBathroom() && (id == switchBR || (id == doorBR && isTurnOn /* = door is opening */) || (id == motionDetectorBathroom && isTurnOn)))
		turnBMLightandVentOn(command); // req 1abc - bathroom

	if (isLightOnInBathroom() && id == switchBR)
		turnBMLightandVentOff(command); // req 2 - bathroom

	if (!isLightOnInWC() && (id == switchWC || (id == doorWC && isTurnOn /* = door is opening */) || (id == motionDetectorToilet && isTurnOn)))
		turnWCLightandVentOn(command); // req 1abc - wc

	if (isLightOnInWC() && id == switchWC)
		turnWCLightandVentOff(command); // req 2 - wc


	if (id == motionDetectorBathroom && isTurnOn)
		createDelayedTurnOffBM(); // req 5

	if (id == motionDetectorToilet && isTurnOn)
		createDelayedTurnOffWC(); // req 5

	return !command.isEmpty();
}