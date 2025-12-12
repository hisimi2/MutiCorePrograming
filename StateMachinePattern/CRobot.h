#pragma once
#include <memory>
#include "CUnit.h"
#include "CVerticalCylinder.h"
#include "CGripperCylinder.h"
#include "CSubject.h" // CSubject 클래스 포함

#include "CRobotPickStep.h"
#include "CRobotReadyStep.h"

class CRobot : public CUnit, public CSubject
{
public:

    CRobot(IOPSwitch& startSwitch)
        : CUnit("Robot") // nullptr 대신 빈 문자열을 전달합니다.
        , m_Z("Hand", 0, 0)        // 적절한 인자(예: 포트 번호 등)로 CVerticalCylinder 생성
        , m_Grip("Grip", 0, 0)     // 적절한 인자(예: 포트 번호 등)로 CGripperCylinder 생성
    {
        // std::make_unique를 사용하여 Step 인스턴스를 생성합니다.
        m_pReadyStep = std::make_unique<CRobotReadyStep>();
        m_pPickStep = std::make_unique<CRobotPickStep>();

        // 초기 스텝을 'Ready' 스텝으로 설정합니다.
        setStep(m_pReadyStep.get());
    }

    virtual ~CRobot() {};

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

