#pragma once
#include "CAxisController.h"
#include "CCylinder.h"
#include "IObserver.h"
#include "CAbsThread.h"

class CRobot : public CAbsThread
{
	CAxisController _axisX;
	CAxisController _axisY;
	CCylinder       _cylinderGripper;
	CCylinder		_cylinderHand;

	bool	sequence() override;
public:
	CRobot(void);
	~CRobot(void);

	void moveX(int position);
    void moveY(int position);
    void grip(void);
    void release(void);
	void up(void);
	void down(void);

    
    void attach(IObserver* pObserver);

};

