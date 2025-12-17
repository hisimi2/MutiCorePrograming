#include "stdafx.h"
#include "CRobotPickStep.h"
#include "CRobot.h"

// CRobotPickStep 클래스의 execute 함수 정의
IStep* CRobotPickStep::execute(CUnit* pUnit)
{
    CRobot* pRobot = static_cast<CRobot*>(pUnit);

    // Z축을 내린다.
    pRobot->getZ().down();

    // Z축이 내려갈 때까지 기다린다.
    if (pRobot->getZ().isDown())
    {
        // 다음 단계(예: 그리퍼 닫기)로 전환하거나,
        // 시퀀스를 종료하기 위해 nullptr을 반환할 수 있다.
        // return pRobot->m_pGripCloseStep.get();
        return this;
    }

    return this;
}