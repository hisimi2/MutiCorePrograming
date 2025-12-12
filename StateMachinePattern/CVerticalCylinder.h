#pragma once
#include "CCylinderBase.h"
class CVerticalCylinder :
    public CCylinderBase
{
public:
    using CCylinderBase::CCylinderBase; // 생성자 상속

    int up(bool bManual = MANUAL) {
		return actA(bManual);
    }
    int down(bool bManual = MANUAL)
    {
		return actB(bManual);
    }
    bool isUp() {
		return isActA();
    }
    bool isDown() {
		return isActB();
    }

};

