#pragma once
#include "CCylinderBase.h"
class CGripperCylinder :
    public CCylinderBase
{
public:
    using CCylinderBase::CCylinderBase; // 생성자 상속

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

