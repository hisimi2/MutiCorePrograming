#include "stdafx.h"
#include "CCylinderBase.h"
#include "CTimer.h"

constexpr int INVALID = -1;

// std::initializer_list를 사용하여 타입-안전하게 센서 포트 설정
void CCylinderBase::setSensorPortA(initializer_list<UINT> list)
{
	m_ASensor.clear();
	for (const auto& port : list)
	{
		m_ASensor.push_back(port);
	}
}
void CCylinderBase::setSensorPortB(initializer_list<UINT> list)
{
	m_BSensor.clear();
	for (const auto& port : list)
	{
		m_BSensor.push_back(port);
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

void CCylinderBase::setName(string& strData)
{
	m_strName = strData;
}

int	CCylinderBase::actA(bool bManual)
{
	CTimer checkTimeOut(m_dTimer);

	checkTimeOut.startTimer();
	while (1)
	{
		if (isActA())
		{
			Sleep(m_uDelay);
			
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

int CCylinderBase::actB(bool bManual)
{
	CTimer checkTimeOut(static_cast<long long>(m_dTimer));
	
	checkTimeOut.startTimer();
	while (1)
	{
		if (isActB())
		{
			Sleep(m_uDelay);
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

void CCylinderBase::setDelay(long long dTimer, UINT uDelay)
{
	m_dTimer = dTimer;
	m_uDelay = uDelay;
}