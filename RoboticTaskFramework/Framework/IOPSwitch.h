#pragma once
#include <Windows.h>

class IOPSwitch
{
public:
	virtual BOOL getStatus()=0;
	virtual void setStatus(BOOL bStatus)=0;
};