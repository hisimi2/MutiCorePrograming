#pragma once
class IAxisController
{
	virtual void MoveTo(double targetPosition) = 0;
	virtual double GetCurrentPosition() = 0;
};

