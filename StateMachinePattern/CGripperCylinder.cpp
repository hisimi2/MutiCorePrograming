#include "stdafx.h"
#include "CGripperCylinder.h"


CGripperCylinder::CGripperCylinder(std::string name, int nOnBit, int nOffBit)
    : CCylinderBase(name, nOnBit, nOffBit)
{
}

// IGripperCylinder 인터페이스 구현
int CGripperCylinder::clamp(bool bManual)
{

    return CCylinderBase::actA(bManual);
}

int CGripperCylinder::unclamp(bool bManual)
{
    return CCylinderBase::actB(bManual);
}

bool CGripperCylinder::isClamp()
{
	return CCylinderBase::isActA();
}

bool CGripperCylinder::isUnclamp() 
{
    return CCylinderBase::isActB();
}