#pragma once
#include "ICylinder.h"

// 수직 실린더에 대한 인터페이스
// ICylinder로부터 가상 상속을 받도록 수정
class IVerticalCylinder : public virtual ICylinder
{
public:
	virtual ~IVerticalCylinder() {}
	virtual int up(bool bManual = MANUAL_OPERATION) = 0;
	virtual int down(bool bManual = MANUAL_OPERATION) = 0;
	virtual bool isUp() = 0;
	virtual bool isDown() = 0;
};