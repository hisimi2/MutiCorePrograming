#include "stdafx.h"
#include "CRobotPickStep.h"
#include "CRobot.h"

// CRobotPickStep 클래스의 execute 함수 정의
IStep* CRobotPickStep::execute(CUnit* pUnit)
{
    CRobot* pRobot = static_cast<CRobot*>(pUnit);

    switch (m_eStep)
    {
    case EStep::MOVE_PICKING_POSITION:
		pRobot->getXAxis().MoveTo(100.0); // 예시 위치
		pRobot->getYAxis().MoveTo(200.0); // 예시 위치
		pRobot->notify("로봇의 X, Y 축을 피킹 위치로 이동 명령");
        m_eStep = EStep::WAIT_MOVE_PICKING_POSITION;
        return this;
    case EStep::WAIT_MOVE_PICKING_POSITION:
		if (pRobot->getXAxis().GetCurrentPosition() == 100.0 &&
			pRobot->getYAxis().GetCurrentPosition() == 200.0)
		{
			m_eStep = EStep::DOWN_Z;
			pRobot->notify("이동 완료 확인");
		}
        return this;
    case EStep::DOWN_Z:
		pRobot->getZ().down();
		m_eStep = EStep::WAIT_DOWN_Z;
		pRobot->notify("Z축을 내리는 명령");
        return this;

    case EStep::WAIT_DOWN_Z:
		if (pRobot->getZ().isDown())
		{
			m_eStep = EStep::CLAMP_GRIPPER;
			pRobot->notify("Z축이 내려갔는지 확인");
		}

        return this;

    case EStep::CLAMP_GRIPPER:
		pRobot->getGrip().clamp();
		pRobot->notify("그리퍼를 잡기 명령");
		m_eStep = EStep::WAIT_CLAMP_GRIPPER;

        return this;

    case EStep::WAIT_CLAMP_GRIPPER:
		if (pRobot->getGrip().isClamp())
		{
			pRobot->notify("그리퍼를 잡기 확인");
			m_eStep = EStep::UP_Z;
		}

        return this;

    case EStep::UP_Z:
		pRobot->getZ().up();
		pRobot->notify("Z축을 올리는 명령");
		m_eStep = EStep::WAIT_UP_Z;

        return this;

    case EStep::WAIT_UP_Z:
		if (pRobot->getZ().isUp())
		{
			pRobot->notify("Z축이 올라갔는지 확인");
			m_eStep = EStep::COMPLETE;
		}
        return this;

    case EStep::COMPLETE:
		pRobot->notify("피킹 동작 완료, 다음 스텝으로 전환");
		m_eStep = EStep::MOVE_PICKING_POSITION; // 다음을 위해 내부 상태 초기화
		return pRobot->m_pReadyStep.get();
    }

    return this;
}