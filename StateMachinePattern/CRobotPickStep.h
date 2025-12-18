#pragma once
#include "Framework/IStep.h"

// CRobotPickStep 선언 (추가 필요시)
class CRobotPickStep : public IStep
{
public:
    IStep*  execute(CUnit* pUnit) override;

    CRobotPickStep() : m_strName("CRobotPickStep"), m_eStep(EStep::MOVE_PICKING_POSITION) {}

private:
    enum class EStep
    {
        MOVE_PICKING_POSITION,
		WAIT_MOVE_PICKING_POSITION,
        DOWN_Z,
		WAIT_DOWN_Z,
		CLAMP_GRIPPER,
        WAIT_CLAMP_GRIPPER,
		UP_Z,
        WAIT_UP_Z,
		COMPLETE,
    };

    EStep m_eStep;

    std::string m_strName;
};





