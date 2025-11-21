#pragma once
#include <Windows.h>

class IOPSwitch
{
public:
	virtual bool getStatus()=0;
	virtual void setStatus(BOOL bStatus)=0;
};