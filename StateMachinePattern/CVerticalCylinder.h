#pragma once
#include "CCylinderBase.h"
#include "IVerticalCylinder.h" // IVerticalCylinder 인터페이스 포함

class CVerticalCylinder : public CCylinderBase, public IVerticalCylinder
{
public: // 생성자를 public으로 변경
	// 생성자 및 소멸자
	CVerticalCylinder(std::string name, int nOnBit, int nOffBit);

	// IVerticalCylinder 인터페이스 구현
	int up(bool bManual) override;
	int down(bool bManual) override;
	bool isUp() override;
	bool isDown() override;

	// C4250 경고 해결을 위해 ICylinder 함수 명시적 재정의
	int actA(bool bManual) override;
	int actB(bool bManual) override;
	bool isActA() override;
	bool isActB() override;
};

