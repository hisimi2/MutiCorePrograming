#pragma once
#include "ICylinder.h"

// 수직 실린더의 동작을 정의하는 인터페이스
// ICylinder를 가상으로 상속하도록 수정
class IVerticalCylinder : public virtual ICylinder
{
public:
	virtual ~IVerticalCylinder() {};
	virtual int up(bool bManual = false) = 0;
	virtual int down(bool bManual = false) = 0;
	virtual bool isUp() = 0;
	virtual bool isDown() = 0;
};