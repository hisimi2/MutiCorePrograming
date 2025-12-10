#include "stdafx.h"
#include "CCylinder.h"
#include "CTimer.h"

CCylinder::~CCylinder()
{
}

void CCylinder::setSensorPortA(UINT uCount, va_list& list)
{
	if (FALSE == m_ASensor.empty())
	{
		m_ASensor.clear();
	}
	for (UINT nIndex = 0; nIndex < uCount; nIndex++)
	{
		m_ASensor.push_back(va_arg(list, int));
	}
}

void CCylinder::setSensorPortB(UINT uCount, va_list& list)
{
	if (FALSE == m_BSensor.empty())
	{
		m_BSensor.clear();
	}
	for (UINT nIndex = 0; nIndex < uCount; nIndex++)
	{
		m_BSensor.push_back(va_arg(list, int));
	}
}

BOOL CCylinder::isUp()
{
	BOOL bPortA = true;
	BOOL bPortB = false;
	//IN SENSOR is Not exist
	if (m_ASensor.empty() && m_BSensor.empty())
	{
		if (INVALID != m_nOutA)
		{
			// bPortA = m_pDIO->out(m_nOutA);
		}

		if (INVALID != m_nOutB)
		{
			// bPortB = m_pDIO->out(m_nOutB);
		}
	}

	for (UINT nIndex = 0; nIndex < m_ASensor.size(); nIndex++)
	{
		if (1)// !m_pDIO->in(m_ASensor[nIndex]))
		{
			bPortA = false;
			break;
		}
	}
	for (UINT nIndex = 0; nIndex < m_BSensor.size(); nIndex++)
	{
		if (1)// m_pDIO->in(m_BSensor[nIndex]))
		{
			bPortB = true;
			break;
		}
	}

	if (bPortA && !bPortB)
	{
		return true;
	}
	else
	{
		return false;
	}
}

BOOL CCylinder::isDown()
{
	BOOL bPortA = false;
	BOOL bPortB = true;

	if (m_ASensor.empty() && m_BSensor.empty())
	{
		if (INVALID != m_nOutA)
		{
			// bPortA = m_pDIO->out(m_nOutA);
		}

		if (INVALID != m_nOutB)
		{
			// bPortB = m_pDIO->out(m_nOutB);
		}
	}
	else
	{
		for (UINT nIndex = 0; nIndex < m_ASensor.size(); nIndex++)
		{
			if (1)// m_pDIO->in(m_ASensor[nIndex]))
			{
				bPortA = true;
				break;
			}
		}
		for (UINT nIndex = 0; nIndex < m_BSensor.size(); nIndex++)
		{
			if (1)// !m_pDIO->in(m_BSensor[nIndex]))
			{
				bPortB = false;
				break;
			}
		}
	}

	if (!bPortA && bPortB)
	{
		return true;
	}

	return false;
}

void CCylinder::upAsync()
{
	if (INVALID != m_nOutA)
	{
		// m_pDIO->out(m_nOutA, true);
	}

	if (INVALID != m_nOutB)
	{
		// m_pDIO->out(m_nOutB, false);
	}
}

void CCylinder::downAsync()
{
	if (INVALID != m_nOutA)
	{
		// m_pDIO->out(m_nOutA, false);
	}
	if (INVALID != m_nOutB)
	{
		// m_pDIO->out(m_nOutB, true);
	}
}

void CCylinder::setName(string& strData)
{
	m_strName = strData;
}

int CCylinder::up(BOOL bManual)
{
	CTimer checkTimeOut(m_dTimer);
	upAsync();
	checkTimeOut.startTimer();
	while (1)
	{
		if (isUp())
		{
			Sleep(m_uDelay);
			notify("CYLINDER_UP_" + m_strName);
			return TRUE;
		}
		else
		{
			if (checkTimeOut.isOver())
			{
				return FALSE;
			}
		}
	}
}

int CCylinder::down(BOOL bManual)
{
	CTimer checkTimeOut(m_dTimer);
	downAsync();
	checkTimeOut.startTimer();
	while (1)
	{
		if (isDown())
		{
			Sleep(m_uDelay);
			notify("CYLINDER_DOWN_" + m_strName);
			return TRUE;
		}
		else
		{
			if (checkTimeOut.isOver())
			{
				return FALSE;
			}
		}
	}
}

void CCylinder::setDelay(double dTimer, UINT uDelay)
{
	m_dTimer = dTimer;
	m_uDelay = uDelay;
}