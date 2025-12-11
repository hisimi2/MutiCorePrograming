#include "stdafx.h"
#include "CRobot.h"

CRobot::CRobot(void)
	: _axisX(), _axisY(), _cylinderGripper("Gripper"), _cylinderHand("Hand")
{
	suspend();
}

CRobot::~CRobot(void)
{
	// 소멸자에서 필요한 정리 코드 작성
}

void CRobot::moveX( int position )
{
	_axisX.move(position);
}

void CRobot::moveY( int position )
{
	_axisY.move(position);
}

void CRobot::grip( void )
{
	_cylinderGripper.clamp();
}

void CRobot::release( void )
{
	_cylinderGripper.unclamp();
}

void CRobot::up(void)
{
	_cylinderHand.up();
}

void CRobot::down(void)
{
	_cylinderHand.down();
}

void CRobot::attach(IObserver* pObserver)
{
	_axisX.attach(pObserver);
	_axisY.attach(pObserver);
	_cylinderGripper.attach(pObserver); 
	_cylinderHand.attach(pObserver);
}

bool CRobot::sequence()
{
	// picking
	up();
	moveX(100);
	moveY(100);
	release();
	down();
	grip();
	up();
	
	// placing
	up();
	moveX(200);
	moveY(200);
	down();
	release();
	up();

	return true;
}





