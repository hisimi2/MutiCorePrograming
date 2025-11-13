#include "stdafx.h"
#include "COPSwitch.h"

COPSwitch::COPSwitch(EType type, bool isBlink)
	: m_type(type), m_isBlink(isBlink), m_BlinkTimer(0.5)
{
}

COPSwitch::~COPSwitch()
{
	this->setEnd();
	Sleep(100);
}

void COPSwitch::setInput(std::initializer_list<int> inputs)
{
	m_inputs = inputs;
}

void COPSwitch::setOutput(std::initializer_list<int> outputs)
{
	m_outputs = outputs;
}

void COPSwitch::setGroup(COPSwitch* pObject)
{
	m_pGroup = pObject;
}

void COPSwitch::setStatus(bool bStatus)
{
	m_status = bStatus;
}

void COPSwitch::setBlink(bool bStatus)
{
	m_isBlink = bStatus;
}

void COPSwitch::setLED(bool bStatus)
{
	for (int output : m_outputs)
	{
		// m_pDIO->out(output, bStatus);
	}
}

bool COPSwitch::checkInSensor()
{
	for (int input : m_inputs)
	{
		if (1)//m_pDIO->in(input))
		{
			return true;
		}
	}
	return false;
}

bool COPSwitch::getStatus() const
{
	return m_status;
}

int COPSwitch::sequence()
{
	bool bInCheck = checkInSensor();
	switch (m_type)
	{
	case EType::KEEP:
		if (bInCheck)
		{
			setStatus(true);
			if (m_pGroup)
			{
				m_pGroup->setStatus(false);
			}
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
	if (m_isBlink && getStatus())
	{
		// TIME DELAY
		if (m_BlinkTimer.isOver())
		{
			m_blinkStatus = !m_blinkStatus;
			setLED(m_blinkStatus);
			m_BlinkTimer.startTimer();
		}
	}
	return 0;
}
