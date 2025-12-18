#include "stdafx.h"
#include "CRobot.h"

// 구체 클래스 헤더는 구현 파일에 포함
#include "CVerticalCylinder.h"
#include "CGripperCylinder.h"
#include "CAxisController.h"

// Step 클래스 헤더 포함
#include "CRobotPickStep.h"
#include "CRobotReadyStep.h"

CRobot::CRobot(IOPSwitch& startSwitch)
    : CUnit("Robot", startSwitch)
{
    initializeComponents();
    initializeSteps();
}

CRobot::~CRobot() = default;

void CRobot::initializeComponents()
{
    // 실제 객체 생성 및 인터페이스 포인터로 관리
    m_pZ     = std::make_unique<CVerticalCylinder>("Hand", 0, 0);
    m_pGrip  = std::make_unique<CGripperCylinder>("Grip", 0, 0);
    m_pXAxis = std::make_unique<CAxisController>();
    m_pYAxis = std::make_unique<CAxisController>();
}

void CRobot::initializeSteps()
{
    m_pReadyStep = std::make_unique<CRobotReadyStep>();
    m_pPickStep  = std::make_unique<CRobotPickStep>();

    // 초기 상태 설정
    setStep(m_pReadyStep.get());
}

IVerticalCylinder& CRobot::getZ() const
{
    return *m_pZ;
}

IGripperCylinder& CRobot::getGrip() const
{
    return *m_pGrip;
}

IAxisController& CRobot::getXAxis() const
{
    return *m_pXAxis;
}

IAxisController& CRobot::getYAxis() const
{
    return *m_pYAxis;
}

IStep* CRobot::getPickStep() const
{
    return m_pPickStep.get();
}

IStep* CRobot::getReadyStep() const
{
    return m_pReadyStep.get();
}

