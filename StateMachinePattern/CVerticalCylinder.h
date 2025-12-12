#pragma once
#include "CCylinderBase.h"

class CVerticalCylinder : public CCylinderBase
{
public:
    // 명시적으로 생성자 선언
    CVerticalCylinder(std::string name, int nOnBit, int nOffBit)
        : CCylinderBase(name, nOnBit, nOffBit)
    {
    }

    int up(bool bManual = MANUAL) {
        return actA(bManual);
    }
    int down(bool bManual = MANUAL) {
        return actB(bManual);
    }
    bool isUp() {
        return isActA();
    }
    bool isDown() {
        return isActB();
    }
};

