#pragma once
#include "CCylinderBase.h"
#include "IVerticalCylinder.h" // IVerticalCylinder 인터페이스 포함

class CVerticalCylinder : public CCylinderBase, public IVerticalCylinder
{
public: // 생성자를 public으로 변경
	// 생성자 및 소멸자
	CVerticalCylinder(std::string name, int nOnBit, int nOffBit);

	// IVerticalCylinder 인터페이스 구현
	int up(bool bManual = MANUAL_OPERATION) override;
	int down(bool bManual = MANUAL_OPERATION) override;
	bool isUp() override;
	bool isDown() override;
};

