#pragma once
#include "IAxisController.h"

class CAxisController : public IAxisController
{
public:
	CAxisController();
	~CAxisController();
	void MoveTo(double targetPosition);
	double GetCurrentPosition();
};

