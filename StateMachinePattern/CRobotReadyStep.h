#pragma once
#include "Framework/IStep.h"

class CRobotReadyStep : public IStep
{
public:
    CRobotReadyStep() : m_strName("CRobotReadyStep"), m_eStep(EStep::START) {}
    

    // CUnit이 호출하는 execute 메서드를 오버라이딩합니다.
    IStep* execute(CUnit* pUnit) override;

private:
    enum class EStep
    {
        START,
        WAIT_COMPLETE,
    };

    EStep m_eStep;
    std::string m_strName;
};




