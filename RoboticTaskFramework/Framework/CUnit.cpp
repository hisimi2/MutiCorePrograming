#include "stdafx.h"
#include "CUnit.h"

CUnit::CUnit(IOPSwitch& startSwitch)
	: m_pStartSwitch(&startSwitch), m_pCurrentStep(nullptr)
{
}

CUnit::~CUnit()
{
	// Step 객체들은 각 유닛의 소멸자에서 해제합니다.
}

void CUnit::setStep(IStep* pStep)
{
	m_pCurrentStep = pStep;
}

bool CUnit::sequence()
{
	// 시작 스위치가 꺼져 있으면 대기
	if (!m_pStartSwitch->getStatus())
	{
		Sleep(10);
		return 0;
	}

	if (m_pCurrentStep)
	{
		IStep* pNextStep = m_pCurrentStep->execute(this);
		if (pNextStep != m_pCurrentStep)
		{
			setStep(pNextStep);
		}
	}
	return 0;
}