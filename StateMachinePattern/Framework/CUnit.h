#pragma once

#include "IStep.h"
#include "IOPSwitch.h"
#include "IPeriodicTask.h"
#include "CSubject.h" // CSubject 헤더 포함

// CUnit이 CSubject를 상속받아 모든 Unit이 알림 기능을 가짐
class CUnit : public IPeriodicTask, public CSubject
{
public:
	// StartSwitch를 레퍼런스로 받도록 변경
    CUnit(std::string name, IOPSwitch& startSwitch);
    virtual ~CUnit();
    void setStep(IStep* pStep);

protected:
    bool sequence() override; // override 키워드 추가
    std::string m_strName;
    IStep*      m_pCurrentStep;
    IOPSwitch&  m_StartSwitch; // 포인터에서 레퍼런스로 변경
};