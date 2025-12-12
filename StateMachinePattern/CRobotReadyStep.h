#pragma once
#include "IStep.h"

// CRobotReadyStep ¼±¾ð
class CRobotReadyStep : public IStep
{
public:
    IStep*  execute(CUnit* pUnit) override;
};




