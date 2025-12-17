#pragma once

#include "IStep.h"
#include "IOPSwitch.h"
#include "IPeriodicTask.h"

class CUnit : public IPeriodicTask
{
public:
	// StartSwitch를 레퍼런스로 받도록 변경
    CUnit(std::string name, IOPSwitch& startSwitch);
    virtual ~CUnit();

    void setStep(IStep* pStep);
    bool sequence() override; // override 키워드 추가

protected:
    std::string m_strName;
    IStep* m_pCurrentStep;

    IOPSwitch& m_StartSwitch; // 포인터에서 레퍼런스로 변경
};