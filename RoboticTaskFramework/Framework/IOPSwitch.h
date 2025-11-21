#pragma once
#include <Windows.h>

class IOPSwitch
{
public:
	virtual bool getSwitchStatus()=0;
	virtual void setSwitchStatus(bool bStatus)=0;
};