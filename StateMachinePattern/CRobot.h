#pragma once
#include "Framework/CUnit.h"
#include "Framework/IOPSwitch.h"
#include "CVerticalCylinder.h"   // 새로운 헤더 추가
#include "CGripperCylinder.h"    // 새로운 헤더 추가
#include "CRobotReadyStep.h"
#include "CRobotPickStep.h"
#include <memory>

class CRobot : public CUnit
{
public:
	CRobot(IOPSwitch& startSwitch);
	~CRobot();

	// 반환 타입도 새로운 클래스로 변경
	CVerticalCylinder& getZ() { return m_Z; }
	CGripperCylinder& getGrip() { return m_Grip; }

private:
	friend class CRobotReadyStep;
	friend class CRobotPickStep;
	// ... 다른 Step 클래스들을 friend로 선언

	// 멤버 변수 타입을 새로운 클래스로 변경
	
	CVerticalCylinder m_Z;
	CGripperCylinder m_Grip;

	// Step 인스턴스 타입을 std::unique_ptr로 변경
	std::unique_ptr<CRobotReadyStep> m_pReadyStep;
	std::unique_ptr<CRobotPickStep> m_pPickStep;
	// ... 다른 Step 포인터들도 동일하게 변경 ...
};

