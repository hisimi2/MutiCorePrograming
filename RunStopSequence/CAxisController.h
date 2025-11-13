#pragma once
class CAxisController
{
public:
	CAxisController();
	~CAxisController();
	void MoveTo(double targetPosition);
	double GetCurrentPosition();
};

