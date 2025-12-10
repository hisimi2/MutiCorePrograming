#include "stdafx.h"
#include "CRobot.h"

CRobot::CRobot(void)
	: _axisX(), _axisY(), _cylinderGripper("Gripper"), _cylinderHand("Hand")
{
	// 생성자에서 필요한 초기화 코드 작성
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
	_cylinderGripper.clampAsync();
}

void CRobot::release( void )
{
	_cylinderGripper.unclampAsync();
}

void CRobot::attach(IObserver* pObserver)
{
	_axisX.attach(pObserver);
	_axisY.attach(pObserver);
	_cylinderGripper.attach(pObserver);
}

bool CRobot::sequence()
{
	moveX(100);
	moveY(100);
	grip();


	moveX(200);
	moveY(200);
	release();

	return true;
}





