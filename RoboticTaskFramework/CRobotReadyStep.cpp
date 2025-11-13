#include "stdafx.h"
#include "CRobotReadyStep.h"
#include "CRobot.h"

// ... 다른 Step 클래스 선언 ...

// CRobotReadyStep 클래스의 execute 함수 정의
IStep* CRobotReadyStep::execute(CUnit* pUnit)
{
    CRobot* pRobot = static_cast<CRobot*>(pUnit);
    
    // 로봇 Z축을 올리고 그리퍼를 푼다.
    pRobot->getZ().up();
    pRobot->getGrip().unclamp();

    // Z축이 올라가고 그리퍼가 풀릴 때까지 기다린다.
    if (pRobot->getZ().isUp() && pRobot->getGrip().isUnclamp())
    {
        // 동작이 완료되면 다음 단계인 PickStep으로 전환한다.
        return pRobot->m_pPickStep.get(); 
    }

    // 아직 동작이 완료되지 않았으면 현재 단계를 유지한다.
    return this;
}


