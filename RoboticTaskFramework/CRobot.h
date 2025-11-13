#pragma once
#include "Framework/CUnit.h"
#include "Framework/IOPSwitch.h"
#include "CCylinder.h"
#include "CRobotReadyStep.h"
#include <memory>


class CRobot : public CUnit
{
public:
	CRobot(IOPSwitch& startSwitch);
	~CRobot();

	// CRobot의 컴포넌트에 접근하기 위한 public 메서드
	CCylinder& getZ() { return m_Z; }
	CCylinder& getGrip() { return m_Grip; }

private:
	friend class CRobotReadyStep;
	friend class CRobotPickStep;
	// ... 다른 Step 클래스들을 friend로 선언

	CCylinder m_Z;
	CCylinder m_Grip;

	// Step 인스턴스 타입을 std::unique_ptr로 변경
	std::unique_ptr<CRobotReadyStep> m_pReadyStep;
	std::unique_ptr<CRobotPickStep> m_pPickStep;
	// ... 다른 Step 포인터들도 동일하게 변경 ...
};

