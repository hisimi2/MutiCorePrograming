#include "stdafx.h"
#include "CVerticalCylinder.h"


CVerticalCylinder::CVerticalCylinder(std::string name, int nOnBit, int nOffBit)
    : CCylinderBase(name, nOnBit, nOffBit)
{
}

// IVerticalCylinder 인터페이스 구현
int CVerticalCylinder::up(bool bManual)
{
	return CCylinderBase::actA(bManual);
}
int CVerticalCylinder::down(bool bManual)
{
	return CCylinderBase::actB(bManual);
}
bool CVerticalCylinder::isUp()
{
	return	CCylinderBase::isActA();
}
bool CVerticalCylinder::isDown()
{
	return	CCylinderBase::isActB();
}