#pragma once
class IAxisController
{
public:
	virtual int MoveTo(double targetPosition)	= 0;
	virtual double GetCurrentPosition()			= 0;
};

