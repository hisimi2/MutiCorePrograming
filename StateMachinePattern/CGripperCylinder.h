#pragma once
#include "CCylinderBase.h"
class CGripperCylinder :
    public CCylinderBase
{
public:
    CGripperCylinder(std::string name, int nOnBit, int nOffBit)
        : CCylinderBase(name, nOnBit, nOffBit)
    {
    }

    int clamp(bool bManual = MANUAL) {
        return actA(bManual);
    }
    int unclamp(bool bManual = MANUAL) {
		return actB(bManual);
    }
    bool isClamp()
    {
		return isActA();
    }
    bool isUnclamp()
    {
		return isActB();
    }
};

