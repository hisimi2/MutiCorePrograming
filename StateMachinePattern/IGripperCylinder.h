#pragma once
#include "ICylinder.h"

// 그리퍼 실린더의 동작을 정의하는 인터페이스
// ICylinder를 가상으로 상속하도록 수정
class IGripperCylinder : public virtual ICylinder
{
public:
	virtual ~IGripperCylinder() {};
	virtual int clamp(bool bManual = false) = 0;
	virtual int unclamp(bool bManual = false) = 0;
	virtual bool isClamp() = 0;
	virtual bool isUnclamp() = 0;
};