#pragma once
#include "CCylinderBase.h"
#include "IGripperCylinder.h" // IGripperCylinder 인터페이스 포함

class CGripperCylinder : public CCylinderBase, public IGripperCylinder
{
public: 
	// 부모 클래스 생성자를 명시적으로 호출
	CGripperCylinder(std::string name, int nOnBit, int nOffBit);
		

	// IGripperCylinder 인터페이스 구현
	// 기본 매개변수는 베이스 클래스에 이미 정의되어 있으므로 여기서는 생략합니다.
	int		clamp(bool bManual) override;
	int		unclamp(bool bManual) override;
	bool	isClamp() override;
	bool	isUnclamp() override;

	// C4250 경고 해결을 위해 ICylinder 함수 명시적 재정의
	int actA(bool bManual) override;
	int actB(bool bManual) override;
	bool isActA() override;
	bool isActB() override;
};

