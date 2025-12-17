#include "stdafx.h"
#include "CVerticalCylinder.h"

CVerticalCylinder::CVerticalCylinder(std::string name, int nOnBit, int nOffBit)
	: CCylinderBase(name, nOnBit, nOffBit)
{
}

int CVerticalCylinder::up(bool bManual)
{
	return actA(bManual);
}

int CVerticalCylinder::down(bool bManual)
{
	return actB(bManual);
}

bool CVerticalCylinder::isUp()
{
	return isActA();
}

bool CVerticalCylinder::isDown()
{
	return isActB();
}

// C4250 경고 해결을 위해 ICylinder 함수 명시적 재정의
int CVerticalCylinder::actA(bool bManual)
{
	return CCylinderBase::actA(bManual);
}

int CVerticalCylinder::actB(bool bManual)
{
	return CCylinderBase::actB(bManual);
}

bool CVerticalCylinder::isActA()
{
	return CCylinderBase::isActA();
}

bool CVerticalCylinder::isActB()
{
	return CCylinderBase::isActB();
}