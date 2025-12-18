#pragma once

#include "CUnit.h"
#include "CVerticalCylinder.h"
#include "CGripperCylinder.h"
#include "CAxisController.h"
#include "IVerticalCylinder.h" // 인터페이스 포함
#include "IGripperCylinder.h" // 인터페이스 포함
#include "CSubject.h"

#include "CRobotPickStep.h"
#include "CRobotReadyStep.h"

class CRobot : public CUnit, public CSubject
{
public:
    CRobot(IOPSwitch& startSwitch)
        : CUnit("Robot", startSwitch)
    {
        initializeComponents();
        m_pReadyStep = std::make_unique<CRobotReadyStep>();
        m_pPickStep = std::make_unique<CRobotPickStep>();

        setStep(m_pReadyStep.get());
    }

    virtual ~CRobot() {};

    // Getter가 구체 클래스 대신 인터페이스를 반환하도록 수정
    IVerticalCylinder&  getZ()      { return *m_Z; }
    IGripperCylinder&   getGrip()   { return *m_Grip; }
	IAxisController&    getXAxis()  { return *m_XAxis; }
	IAxisController&    getYAxis()  { return *m_YAxis; }

    std::unique_ptr<IStep> m_pPickStep;
    std::unique_ptr<IStep> m_pReadyStep;

private:
    void initializeComponents()
    {
        // 실제 객체는 구체 클래스로 생성
        auto zCylinder      = std::make_unique<CVerticalCylinder>("Hand", 0, 0);
        auto gripCylinder   = std::make_unique<CGripperCylinder>("Grip", 0, 0);

        // 인터페이스 포인터로 관리
        m_Z     = std::move(zCylinder);
        m_Grip  = std::move(gripCylinder);
        m_XAxis = std::make_unique<CAxisController>();
        m_YAxis = std::make_unique<CAxisController>();
    }

    // 인터페이스 타입의 스마트 포인터로 변경
    std::unique_ptr<IVerticalCylinder>  m_Z;
    std::unique_ptr<IGripperCylinder>   m_Grip;
	std::unique_ptr<IAxisController>    m_XAxis;
	std::unique_ptr<IAxisController>    m_YAxis;
};

