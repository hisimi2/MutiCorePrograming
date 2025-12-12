#pragma once
#include <memory>
#include "Framework/CUnit.h"
#include "CVerticalCylinder.h"
#include "CGripperCylinder.h"
#include "CSubject.h" // CSubject 클래스 포함

class CRobot : public CUnit, public CSubject
{
public:
    CRobot(IOPSwitch& startSwitch);
    virtual ~CRobot();

    // 로봇의 구성 요소에 접근하기 위한 getter 메서드들
    CVerticalCylinder& getZ() { return m_Z; }
    CGripperCylinder& getGrip() { return m_Grip; }

    // 스텝 객체들
    std::unique_ptr<IStep> m_pPickStep;
    std::unique_ptr<IStep> m_pReadyStep;

private:
    // 로봇의 구성 요소들
    CVerticalCylinder   m_Z;
    CGripperCylinder    m_Grip;

    
};

