#include "stdafx.h"
#include "CCylinderBase.h"
#include "CTimer.h"


void CCylinderBase::setSensorPortA(UINT uCount, va_list& list)
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

void CCylinderBase::setSensorPortB(UINT uCount, va_list& list)
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

bool CCylinderBase::isActA()
{
	bool bPortA = true;
	bool bPortB = false;
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

bool CCylinderBase::isActB()
{
	bool bPortA = false;
	bool bPortB = true;

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

void CCylinderBase::actAAsync()
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

void CCylinderBase::actBAsync()
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

void CCylinderBase::setName(string& strData)
{
	m_strName = strData;
}

int CCylinderBase::actA(BOOL bManual)
{
	CTimer checkTimeOut(static_cast<long long>(m_dTimer));
	actAAsync();
	checkTimeOut.startTimer();
	while (1)
	{
		if (isActA())
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

int CCylinderBase::actB(BOOL bManual)
{
	CTimer checkTimeOut(static_cast<long long>(m_dTimer));
	actBAsync();
	checkTimeOut.startTimer();
	while (1)
	{
		if (isActB())
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


void CCylinderBase::setDelay(double dTimer, UINT uDelay)
{
	m_dTimer = dTimer;
	m_uDelay = uDelay;
}