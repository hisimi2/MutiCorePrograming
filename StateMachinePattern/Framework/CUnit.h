#pragma once
#include <string>
#include "IStep.h"
#include "IOPSwitch.h"

class CUnit
{
public:
    // 생성자 선언 추가
    CUnit(std::string name);
    virtual ~CUnit();

    void setStep(IStep* pStep);
    bool sequence();

protected:
    std::string m_strName;
    IOPSwitch* m_pStartSwitch;
    IStep* m_pCurrentStep;
};