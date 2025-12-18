#include "stdafx.h"
#include "CAxisController.h"

CAxisController::CAxisController()
{

}
CAxisController::~CAxisController()
{

}

int CAxisController::MoveTo(double targetPosition)
{
	m_encoderPosition = targetPosition;
	return 0;
}

double CAxisController::GetCurrentPosition()
{
	return m_encoderPosition;
}
