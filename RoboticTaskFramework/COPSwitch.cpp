#include "stdafx.h"
#include "COPSwitch.h"
#include <chrono>

COPSwitch::~COPSwitch()
{
	// CAbsThread::setEnd()를 호출하여 스레드 종료를 요청.
	// base 소멸자에서 join 처리를 하므로 여기서는 setEnd만 호출하면 안전함.
	setEnd();
	// m_pDIO는 소유권을 가지지 않으므로 delete 하지 않습니다.
}

// 기존 생성자 시그니처를 헤더와 일치하도록 변경
COPSwitch::COPSwitch(IDio& Dio)
	: m_DIO(Dio)
	, m_BlinkTimer(500) // CTimer 기본 생성자 호출
	, m_lastPoll(std::chrono::steady_clock::now())
{
	// 초기 옵션 설정 가능
	m_type = EType::KEEP;
	// 내부 스레드는 CAbsThread를 통해 관리하므로 resume()으로 시작
	resume();
}

void COPSwitch::setOption(EType type, bool isBlink, unsigned int pollIntervalMs)
{
	m_type = type;
	m_isBlink = isBlink;
	m_pollIntervalMs = pollIntervalMs;
	// poll 간격은 sequence()의 내부 시간 검사에 의해 반영됩니다.
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
	std::lock_guard<std::mutex> lk(m_dioMutex);
	for (int nIndex = 0; nIndex < static_cast<int>(m_outputs.size()); nIndex++)
	{
		// IDio 구현이 자체 동기화를 제공하지 않으면 이 락이 필요합니다.
		m_DIO.out(m_outputs[nIndex], bStatus);
	}
}

bool COPSwitch::checkInSensor()
{
	std::lock_guard<std::mutex> lk(m_dioMutex);
	for (int nIndex = 0; nIndex < static_cast<int>(m_inputs.size()); nIndex++)
	{
		if (m_DIO.in(m_inputs[nIndex]))
		{
			return true;
		}
	}
	return false;
}

bool COPSwitch::getStatus()
{
	return m_status.load() ? TRUE : FALSE;
}

// CAbsThread::sequence() 오버라이드
// - 호출 빈도는 CAbsThread에 의해 짧게 반복될 수 있으므로
//   실제 작업은 poll 간격(pollIntervalMs) 기준으로 수행하도록 함.
// - 이 방식은 긴 블로킹을 피하여 suspend/resume/setEnd에 빠르게 반응하게 함.
int COPSwitch::sequence()
{
	using namespace std::chrono;

	auto now = steady_clock::now();
	auto elapsed = duration_cast<milliseconds>(now - m_lastPoll).count();
	// poll 간격이 지나지 않았다면 즉시 반환
	if (elapsed < static_cast<long long>(m_pollIntervalMs))
	{
		return 0;
	}
	// 실제 폴링 시점 기록
	m_lastPoll = now;

	// 기존 sequence() 구현 내용
	BOOL bInCheck = checkInSensor();
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
