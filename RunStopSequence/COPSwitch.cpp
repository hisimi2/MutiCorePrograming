#include "stdafx.h"
#include "COPSwitch.h"

COPSwitch::~COPSwitch()
{
	this->setEnd();
	Sleep(100);
	if (m_pnIns)
	{
		delete m_pnIns;
	}
	if (m_pnOuts)
	{
		delete m_pnOuts;
	}
}

void COPSwitch::setInput(int nCount, int ...)
{
	if (m_pnIns)
	{
		delete m_pnIns;
	}
	m_nInputCount = nCount;
	m_pnIns = new int[nCount];
	va_list list;
	va_start(list, nCount);
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		m_pnIns[nIndex] = va_arg(list, int);
	}
	va_end(list);
}

void COPSwitch::setOutput(int nCount, int ...)
{
	if (m_pnOuts)
	{
		delete m_pnOuts;
	}
	m_nOutputCount = nCount;
	m_pnOuts = new int[nCount];
	va_list list;
	va_start(list, nCount);
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		m_pnOuts[nIndex] = va_arg(list, int);
	}
	va_end(list);
}

void COPSwitch::setGroup(COPSwitch* pObject)
{
	m_pGroup = pObject;
}

void COPSwitch::setStatus(BOOL bStatus)
{
	m_bStatus = bStatus;
}

void COPSwitch::setBlink(BOOL bStatus)
{
	m_bBlink = bStatus;
}

void COPSwitch::setLED(BOOL bStatus)
{
	for (int nIndex = 0; nIndex < m_nOutputCount; nIndex++)
	{
		//m_pDIO->out(m_pnOuts[nIndex], bStatus);
	}
}

BOOL COPSwitch::checkInSensor()
{
	for (int nIndex = 0; nIndex < m_nInputCount; nIndex++)
	{
		if (1)//m_pDIO->in(m_pnIns[nIndex]))
		{
			return true;
		}
	}
	return false;
}

BOOL COPSwitch::getStatus()
{
	return m_bStatus;
}

int COPSwitch::sequence()
{
	BOOL	bInCheck = checkInSensor();
	switch (m_nType)
	{
	case eKEEP:
		if (bInCheck)
		{
			setStatus(TRUE);
			m_pGroup->setStatus(FALSE);
		}
		break;
	case ePUSH:
		setStatus(bInCheck);
		break;
	case eTOGGLE:
		if (bInCheck && m_bToggleFlag)
		{
			m_bToggleFlag = false;
			setStatus(!getStatus());
		}

		if (!bInCheck)
		{
			m_bToggleFlag = true;
		}
		break;
	default:
		break;
	}
	setLED(getStatus());
	if (m_bBlink && getStatus())
	{
		// TIME DELAY
		if (m_BlinkTimer.isOver())
		{
			m_bBlinkStatus = !m_bBlinkStatus;
			setLED(m_bBlinkStatus);
			m_BlinkTimer.startTimer();
		}
	}
	return 0;
}
