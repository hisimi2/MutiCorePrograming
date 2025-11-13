#pragma once
#include "CAbsThread.h"
#include "CAxisController.h"
#include "CCylinder.h"

class CRobot : public CAbsThread
{
	CAxisController _axisXContoller;
	CAxisController _axisYContoller;
	CCylinder       _cylinderGripper;
	CCylinder       _cylinderHand;

public:
	CRobot(DWORD dwTimeOut = eTIMEOUT);
	~CRobot();
protected:
	int	sequence() override;
};

