#include "stdafx.h"
#include "CRobotReadyStep.h"
#include "CRobot.h"

// ... 다른 Step 클래스 선언 ...

// CRobotReadyStep 클래스의 execute 함수 정의
IStep* CRobotReadyStep::execute(CUnit* pUnit)
{
    CRobot* pRobot = static_cast<CRobot*>(pUnit);
    // TODO: 실제 로직 구현 필요
    // 예: 다음 스텝으로 전환
    // return pRobot->m_pPickStep.get(); // .get()으로 원시 포인터 접근
    return nullptr;
}

// CRobotPickStep 클래스의 execute 함수 정의
IStep* CRobotPickStep::execute(CUnit* pUnit)
{
    // TODO: 실제 로직 구현 필요
    return nullptr;
}
