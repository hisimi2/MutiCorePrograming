#include "stdafx.h"
#include "COPSwitch.h"

// static 멤버 변수 초기화
IDio* COPSwitch::m_pIo = nullptr;

// static 메서드 구현
void COPSwitch::setIo(IDio* pIo)
{
	m_pIo = pIo;
}

// 수정된 생성자
COPSwitch::COPSwitch()
	: m_BlinkTimer(500)
{
	// m_pIo가 설정되지 않았다면 로직이 제대로 동작하지 않을 수 있으므로
	// 방어 코드를 추가하는 것이 좋습니다.
	ASSERT(m_pIo != nullptr);
}

COPSwitch::~COPSwitch()
{
}

bool COPSwitch::getSwitchStatus() 
{
	return m_status.load();
}

void COPSwitch::setSwitchStatus(bool bStatus)
{
	bool previousStatus = m_status.load();
	if (previousStatus != bStatus)
	{
		m_status.store(bStatus);
		// 상태가 외부에서 변경되었을 때 LED 즉시 업데이트
		updateLed();
		// 상태 변경 알림
		std::string msg = "Switch status changed to " + std::string(bStatus ? "ON" : "OFF");
		notify(msg);
	}
}

bool COPSwitch::sequence()
{
	std::lock_guard<std::mutex> lock(m_logicMutex);

	bool sensorStatus = checkInSensor();
	updateStatusFromSensor(sensorStatus);
	updateLed();

	return true;
}

void COPSwitch::updateStatusFromSensor(bool sensorStatus)
{
	bool currentStatus = m_status.load();
	bool previousStatus = currentStatus;

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
	
	if (previousStatus != currentStatus)
	{
		m_status.store(currentStatus);
		// 상태 변경 알림
		std::string msg = "Switch status changed to " + std::string(currentStatus ? "ON" : "OFF");
		notify(msg);
	}
}

void COPSwitch::updateLed()
{
	bool currentStatus = m_status.load();
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
}

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
	// m_pIo를 사용하도록 수정
	if (m_pIo)
	{
		for (int channel : m_outputs)
		{
			m_pIo->out(channel, bStatus);
		}
	}
}

bool COPSwitch::checkInSensor()
{
	// m_pIo를 사용하도록 수정
	if (m_pIo == nullptr || m_inputs.empty())
	{
		return false;
	}

	for (int channel : m_inputs)
	{
		if (m_pIo->in(channel))
		{
			return true; // 하나라도 켜져 있으면 true
		}
	}
	return false;
}