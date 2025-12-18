#include "stdafx.h"
#include "CUnit.h"

// 생성자 초기화 리스트에서 m_StartSwitch를 초기화
CUnit::CUnit(std::string name, IOPSwitch& startSwitch)
	: m_strName(name), m_StartSwitch(startSwitch), m_pCurrentStep(nullptr)
{
}

CUnit::~CUnit()
{
}

void CUnit::setStep(IStep* pStep)
{
	m_pCurrentStep = pStep;
}

bool CUnit::sequence()
{
	// 시작 스위치가 꺼져 있으면 대기
	// 포인터 연산자(->)를 멤버 접근 연산자(.)로 변경
	if (!m_StartSwitch.getSwitchStatus())
	{
		return true; // 대기 상태도 정상적인 동작이므로 true 반환
	}

	Sleep(500); // 동작 실행 지연 임시 지정
	if (m_pCurrentStep)
	{
		IStep* pNextStep = m_pCurrentStep->execute(this);
		if (pNextStep != m_pCurrentStep)
		{
			setStep(pNextStep);
		}
	}
	return true;
}