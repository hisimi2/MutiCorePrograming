#include "stdafx.h"
#include "COPSwitch.h"

COPSwitch::COPSwitch(IDio& io)
	: m_Io(io)
	, m_BlinkTimer(500)
{
	// 스레드 풀 기반이므로 생성자에서 아무것도 하지 않음
}

COPSwitch::~COPSwitch()
{
	// 스레드 생명주기를 관리하지 않으므로 소멸자에서 아무것도 하지 않음
}

bool COPSwitch::getSwitchStatus()
{
	return m_status.load();
}

void COPSwitch::setSwitchStatus(bool bStatus)
{
	m_status.store(bStatus != FALSE);
}

// 헤더와 일치하도록 반환 타입을 void로 수정
bool COPSwitch::sequence()
{
	// 현재 상태(m_status)와 설정(m_type, m_isBlink)에 따라 LED 및 출력 제어
	std::lock_guard<std::mutex> lock(m_dioMutex); // DIO 접근 동기화

	bool sensorStatus = checkInSensor();
	bool currentStatus = m_status.load();

	switch (m_type)
	{
	case EType::PUSH:
		currentStatus = sensorStatus;
		break;
	case EType::TOGGLE:
		if (sensorStatus && m_toggleFlag)
		{
			currentStatus = !currentStatus;
		}
		m_toggleFlag = !sensorStatus;
		break;
	case EType::KEEP:
		// 외부에서 setSwitchStatus로 설정된 값을 유지하므로 변경 없음
		break;
	}
	
	m_status.store(currentStatus);

	if (m_isBlink && currentStatus)
	{
		if (m_BlinkTimer.isOver())
		{
			m_blinkStatus = !m_blinkStatus;
			setLED(m_blinkStatus);
			m_BlinkTimer.startTimer();
		}
	}
	else
	{
		setLED(currentStatus);
	}

	return true;
}

// --- 나머지 멤버 함수 구현 ---

COPSwitch& COPSwitch::setInput(const std::vector<int>& inputs)
{
	m_inputs = inputs;
	return *this;
}
COPSwitch& COPSwitch::setOutput(const std::vector<int>& outputs)
{
	m_outputs = outputs;
	return *this;
}

COPSwitch& COPSwitch::setGroup(COPSwitch* pObject)
{
	m_pGroup = pObject;
	return *this;
}
COPSwitch& COPSwitch::setBlink(bool bStatus)
{
	m_isBlink = bStatus;
	return *this;
}
COPSwitch& COPSwitch::setOption(EType type, bool isBlink, unsigned int pollIntervalMs)
{
	m_type = type;
	m_isBlink = isBlink;
	m_pollIntervalMs = pollIntervalMs;
	return *this;
}

void COPSwitch::setLED(bool bStatus)
{
	for (int channel : m_outputs)
	{
		m_Io.out(channel, bStatus);
	}
}

bool COPSwitch::checkInSensor()
{
	if (m_inputs.empty())
	{
		return false;
	}

	for (int channel : m_inputs)
	{
		if (m_Io.in(channel))
		{
			return true; // 하나라도 켜져 있으면 true
		}
	}
	return false;
}