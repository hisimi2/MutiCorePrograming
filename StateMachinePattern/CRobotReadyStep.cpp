#include "stdafx.h"
#include "CRobotReadyStep.h"
#include "CRobot.h"

CRobotReadyStep::CRobotReadyStep()
    : m_eStep(EStep::START)
{
}

CRobotReadyStep::~CRobotReadyStep()
{
}

// IStep 인터페이스의 execute 메서드를 올바른 Non-Blocking 방식으로 구현합니다.
IStep* CRobotReadyStep::execute(CUnit* pUnit)
{
    CRobot* pRobot = static_cast<CRobot*>(pUnit);

    switch (m_eStep)
    {
    case EStep::START:
        // 1. 로봇 Z축을 올리고 그리퍼를 푸는 명령을 '한 번만' 내립니다.
        pRobot->getZ().up();
        pRobot->getGrip().unclamp();
        pRobot->notify("준비 동작 시작: Z축 상승, 그리퍼 열기");

        // 2. 내부 상태를 '완료 대기'로 변경하고 즉시 리턴합니다.
        m_eStep = EStep::WAIT_COMPLETE;
        
        // 아직 동작이 끝나지 않았으므로 '현재 스텝을 유지'하라는 의미로 this를 반환합니다.
        return this;

    case EStep::WAIT_COMPLETE:
        // 3. 동작 완료 조건을 '매 주기마다' 확인합니다. (대기하지 않음)
        if (pRobot->getZ().isUp() && pRobot->getGrip().isUnclamp())
        {
            // 4. 동작이 완료되었으면, 다음 스텝으로 전환하기 위해 다음 스텝 포인터를 반환합니다.
            pRobot->notify("준비 동작 완료");
            m_eStep = EStep::START; // 다음을 위해 내부 상태를 초기화합니다.
            // 멤버 변수 직접 접근 대신 Getter 사용
            return pRobot->getPickStep();
        }
        
        // 5. 아직 완료되지 않았으면 '현재 스텝을 유지'하기 위해 this를 반환합니다.
        return this;
    }

    // 모든 case가 처리되므로 이 라인에 도달해서는 안 됩니다.
    return this;
}
