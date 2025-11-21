#pragma once
#include "IPeriodicTask.h"
#include "IStep.h"
#include "IOPSwitch.h"

// 모든 로봇, 로더 등의 기반이 되는 추상 유닛 클래스
class CUnit : public IPeriodicTask
{
public:
	CUnit(IOPSwitch& startSwitch);
	virtual ~CUnit();

	// 유닛의 특정 컴포넌트에 접근하기 위한 가상 함수 (필요시 사용)
	// virtual CSomeComponent* getComponent() { return nullptr; }

protected:
	bool sequence() override;
	void setStep(IStep* pStep);

private:
	IStep*		m_pCurrentStep;
	IOPSwitch*	m_pStartSwitch;
};