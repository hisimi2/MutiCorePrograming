#include "stdafx.h"
#include <memory> // std::unique_ptr를 위해 추가
#include "Framework/IStep.h"
#include "CRobot.h"
#include "CRobotReadyStep.h"
#include "CRobotPickStep.h" // CRobotPickStep 사용을 위해 추가


CRobot::CRobot(IOPSwitch& startSwitch)
	: CUnit(startSwitch)
	, m_Z(0, 0)        // 적절한 인자(예: 포트 번호 등)로 CVerticalCylinder 생성
	, m_Grip(0, 0)     // 적절한 인자(예: 포트 번호 등)로 CGripperCylinder 생성
{
	// std::make_unique를 사용하여 Step 인스턴스를 생성합니다.
	m_pReadyStep = std::make_unique<CRobotReadyStep>();
	m_pPickStep = std::make_unique<CRobotPickStep>();
	
	// 초기 스텝을 'Ready' 스텝으로 설정합니다.
	setStep(m_pReadyStep.get());
}
CRobot::~CRobot()
{
    // std::unique_ptr가 자동으로 메모리를 관리하므로 내용이 비어도 안전합니다.
}
