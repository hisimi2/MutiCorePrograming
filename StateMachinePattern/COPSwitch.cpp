#include "stdafx.h"
#include "COPSwitch.h"

COPSwitch::COPSwitch(IDio& io)
	: m_Io(io)
	, m_BlinkTimer(500)
{
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
	m_status.store(bStatus);
	// 상태가 외부에서 변경되었을 때 LED 즉시 업데이트
	updateLed();
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