#pragma once
#include "CCylinderBase.h"
#include "IGripperCylinder.h" // IGripperCylinder 인터페이스 포함

class CGripperCylinder : public CCylinderBase, public IGripperCylinder
{
public: 
	// 부모 클래스 생성자를 명시적으로 호출
	CGripperCylinder(std::string name, int nOnBit, int nOffBit);
		

	// IGripperCylinder 인터페이스 구현
	int		clamp(bool bManual = MANUAL_OPERATION)override;
	int		unclamp(bool bManual = MANUAL_OPERATION) override;
	bool	isClamp() override;
	bool	isUnclamp() override;
};

