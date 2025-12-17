#pragma once
#include "CCylinderBase.h"
#include "IGripperCylinder.h" // IGripperCylinder 인터페이스 포함

class CGripperCylinder : public CCylinderBase, public IGripperCylinder
{
public: // 생성자를 public으로 변경
	CGripperCylinder(std::string name, int nOnBit, int nOffBit);

	// IGripperCylinder 인터페이스 구현
	int		clamp(bool bManual = MANUAL)override;
	int		unclamp(bool bManual = MANUAL) override;
	bool	isClamp() override;
	bool	isUnclamp() override;
};

