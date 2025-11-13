#pragma once
#include "CAbsThread.h"
#include "COPSwitch.h"
#include "CRobot.h"

class CThreadColtrol : public CAbsThread
{
	COPSwitch&	m_startSwitch;
	CRobot		m_robot;

public:
	CThreadColtrol(COPSwitch& startSwitch);
	~CThreadColtrol();
protected:
	int	sequence() override;
};

