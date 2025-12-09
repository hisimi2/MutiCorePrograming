#pragma once
#include "CAbsThread.h"
#include "CAxisController.h"
#include "CCylinder.h"

class CRobot : public CAbsThread
{
	CAxisController _axisX;
	CAxisController _axisY;
	CCylinder       _cylinderGripper;
	CCylinder       _cylinderHand;

protected:
	bool sequence()
	{
		// pick up hand
		_cylinderHand.up();
		_axisX.MoveTo(100.0);
		_axisY.MoveTo(50.0);
		_cylinderGripper.unclamp();
		_cylinderHand.down();
		_cylinderGripper.clamp();
		_cylinderHand.up();

		// move to place position
		_axisX.MoveTo(200.0);
		_axisY.MoveTo(150.0);
		_cylinderHand.down();
		_cylinderGripper.unclamp();
		_cylinderHand.up();

		return 0;
	}
};

