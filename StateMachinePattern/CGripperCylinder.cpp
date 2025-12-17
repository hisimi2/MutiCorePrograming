#include "stdafx.h"
#include "CGripperCylinder.h"

CGripperCylinder::CGripperCylinder(std::string name, int nOnBit, int nOffBit)
	: CCylinderBase(name, nOnBit, nOffBit)
{
}

int CGripperCylinder::clamp(bool bManual)
{
	return actA(bManual);
}

int CGripperCylinder::unclamp(bool bManual)
{
	return actB(bManual);
}

bool CGripperCylinder::isClamp()
{
	return isActA();
}

bool CGripperCylinder::isUnclamp()
{
	return isActB();
}

// C4250 경고 해결을 위해 ICylinder 함수 명시적 재정의
int CGripperCylinder::actA(bool bManual)
{
	return CCylinderBase::actA(bManual);
}

int CGripperCylinder::actB(bool bManual)
{
	return CCylinderBase::actB(bManual);
}

bool CGripperCylinder::isActA()
{
	return CCylinderBase::isActA();
}

bool CGripperCylinder::isActB()
{
	return CCylinderBase::isActB();
}