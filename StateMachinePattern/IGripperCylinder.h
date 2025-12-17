#pragma once
#include "ICylinder.h"
#include "CCylinderBase.h" // MANUAL_OPERATION 상수를 위해 포함

// 그리퍼 실린더의 동작을 정의하는 인터페이스
// ICylinder를 가상 상속하여 다이아몬드 문제 해결
class IGripperCylinder : public virtual ICylinder
{
public:
	virtual ~IGripperCylinder() {};
	virtual int clamp(bool bManual = MANUAL_OPERATION) = 0;
	virtual int unclamp(bool bManual = MANUAL_OPERATION) = 0;
	virtual bool isClamp() = 0;
	virtual bool isUnclamp() = 0;
};