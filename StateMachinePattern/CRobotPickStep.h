#pragma once
#include "Framework/IStep.h"

// CRobotPickStep 선언 (추가 필요시)
class CRobotPickStep : public IStep
{
public:
    IStep* execute(CUnit* pUnit) override;
};




