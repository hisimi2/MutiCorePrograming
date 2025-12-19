#pragma once

#include "CUnit.h"
#include "CSubject.h"
#include "IVerticalCylinder.h"
#include "IGripperCylinder.h"
#include "IAxisController.h"
#include <memory>

// 전방 선언으로 헤더 의존성 감소
class CRobotPickStep;
class CRobotReadyStep;

class CRobot : public CUnit, public CSubject
{
public:
    CRobot(IOPSwitch& startSwitch);
    virtual ~CRobot();

    // 컴포넌트 접근자 (Const Correctness 적용)
    IVerticalCylinder&  getZ() const;
    IGripperCylinder&   getGrip() const;
    IAxisController&    getXAxis() const;
    IAxisController&    getYAxis() const;

    // 상태 접근자 (캡슐화 적용)
    IStep* getPickStep() const;
    IStep* getReadyStep() const;

private:
    void initializeComponents();
    void initializeSteps();

    // 컴포넌트 (인터페이스 타입의 스마트 포인터)
    std::unique_ptr<IVerticalCylinder>  m_pZ;
    std::unique_ptr<IGripperCylinder>   m_pGrip;
    std::unique_ptr<IAxisController>    m_pXAxis;
    std::unique_ptr<IAxisController>    m_pYAxis;

    // 상태 객체
    std::unique_ptr<IStep> m_pPickStep;
    std::unique_ptr<IStep> m_pReadyStep;
};

