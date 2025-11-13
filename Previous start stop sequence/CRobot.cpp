#include "stdafx.h"
#include "CRobot.h"

CRobot::CRobot(DWORD dwTimeOut)
	: CAbsThread(dwTimeOut)
{
}
CRobot::~CRobot()
{
	this->setEnd();
	Sleep(100);
}
int CRobot::sequence()
{
	// pick up hand
	_cylinderHand.up();
	_axisXContoller.MoveTo(100.0);
	_axisYContoller.MoveTo(50.0);
	_cylinderGripper.unclamp();
	_cylinderHand.down();
	_cylinderGripper.clamp();
	_cylinderHand.up();

	// move to place position
	_axisXContoller.MoveTo(200.0);
	_axisYContoller.MoveTo(150.0);
	_cylinderHand.down();
	_cylinderGripper.unclamp();
	_cylinderHand.up();

	return 0;
}


