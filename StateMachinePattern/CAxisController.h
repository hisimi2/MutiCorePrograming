#pragma once
#include "IAxisController.h"

class CAxisController : public IAxisController
{
	double m_encoderPosition;
public:
	CAxisController();
	~CAxisController();
	int MoveTo(double targetPosition) override;
	double GetCurrentPosition();
};

