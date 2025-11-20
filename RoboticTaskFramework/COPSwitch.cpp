#include "stdafx.h"
#include "COPSwitch.h"
#include <thread>
#include <chrono>

COPSwitch::~COPSwitch()
{
	// 내부 스레드 안전 종료
	m_threadExit.store(true);
	if (m_thread.joinable())
	{
		m_thread.join();
	}
	// m_pDIO는 소유권을 가지지 않으므로 delete 하지 않습니다.
}

// 기존 생성자 시그니처를 헤더와 일치하도록 변경
COPSwitch::COPSwitch(IDio& Dio)
	: m_DIO(Dio)
	, m_BlinkTimer(0.5) // CTimer 기본 생성자 호출
{
	// 필요한 초기화 코드 추가
}

void COPSwitch::threadLoop()
{
	using namespace std::chrono;
	while (!m_threadExit.load())
	{
		sequence();
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<std::chrono::milliseconds::rep>(m_pollIntervalMs)));
	}
}

void COPSwitch::setOption(EType type, bool isBlink, unsigned int pollIntervalMs)
{
	m_type = type;
	m_isBlink = isBlink;
	m_pollIntervalMs = pollIntervalMs;
}

void COPSwitch::setInput(const std::vector<int>& inputs)
{
	m_inputs = inputs;
}

void COPSwitch::setOutput(const std::vector<int>& outputs)
{
	m_outputs = outputs;
}

void COPSwitch::setGroup(COPSwitch* pObject)
{
	m_pGroup = pObject;
}

void COPSwitch::setStatus(BOOL bStatus)
{
	m_status.store(bStatus != FALSE);
}

void COPSwitch::setBlink(bool bStatus)
{
	m_isBlink = bStatus;
}

void COPSwitch::setLED(bool bStatus)
{
	// m_pDIO가 유효하면 실제 출력 호출
	for (int nIndex = 0; nIndex < static_cast<int>(m_outputs.size()); nIndex++)
	{
		m_DIO.out(m_outputs[nIndex], bStatus);
	}
}

bool COPSwitch::checkInSensor()
{
	for (int nIndex = 0; nIndex < static_cast<int>(m_inputs.size()); nIndex++)
	{
		if (m_DIO.in(m_inputs[nIndex]))
		{
			return true;
		}
	}
	return false;
}

BOOL COPSwitch::getStatus()
{
	return m_status.load() ? TRUE : FALSE;
}

int COPSwitch::sequence()
{
	BOOL	bInCheck = checkInSensor();
	switch (m_type)
	{
	case EType::KEEP:
		if (bInCheck)
		{
			setStatus(TRUE);
			if (m_pGroup) m_pGroup->setStatus(FALSE);
		}
		break;
	case EType::PUSH:
		setStatus(bInCheck);
		break;
	case EType::TOGGLE:
		if (bInCheck && m_toggleFlag)	
		{
			m_toggleFlag = false;
			setStatus(!getStatus());	
		}

		if (!bInCheck)
		{
			m_toggleFlag = true;
		}
		break;
	default:
		break;
	}
	setLED(getStatus());
	if (this->m_isBlink && getStatus())
	{
		if (m_BlinkTimer.isOver())
		{
			m_blinkStatus = !m_blinkStatus;
			setLED(m_blinkStatus);
			m_BlinkTimer.startTimer();
		}
	}
	return 0;
}
