#pragma once
#include "../Framework/IStep.h"

// CRobotReadyStep 선언
class CRobotReadyStep : public IStep
{
public:
    IStep* execute(CUnit* pUnit) override;
};

// CRobotPickStep 선언 (추가 필요시)
class CRobotPickStep : public IStep
{
public:
    IStep* execute(CUnit* pUnit) override;
};



