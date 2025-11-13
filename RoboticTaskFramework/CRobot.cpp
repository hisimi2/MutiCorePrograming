#pragma once
#include "stdafx.h"
#include <memory> // std::unique_ptr를 위해 추가
#include "../Framework/IStep.h"
#include "CRobot.h"

CRobot::CRobot(IOPSwitch& startSwitch)
	: CUnit(startSwitch), m_Z(), m_Grip()
{
	// std::make_unique를 사용하여 Step 인스턴스를 생성합니다.
	m_pReadyStep = std::make_unique<CRobotReadyStep>();
	m_pPickStep = std::make_unique<CRobotPickStep>();
	// ...
}
CRobot::~CRobot()
{
    // std::unique_ptr가 자동으로 메모리를 관리하므로 내용이 비어도 안전합니다.
}


