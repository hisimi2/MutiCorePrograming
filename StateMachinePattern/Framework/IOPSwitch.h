#pragma once
#include <Windows.h>
#include "IObserver.h"

class IOPSwitch
{
public:

	enum class EType { KEEP, PUSH, TOGGLE };
	virtual ~IOPSwitch() {}
	virtual bool getSwitchStatus() =0;
	virtual void setSwitchStatus(bool bStatus)=0;
	virtual IOPSwitch& setGroup(IOPSwitch* pObject)=0;
	virtual IOPSwitch& setBlink(bool bStatus) = 0;
	virtual IOPSwitch& setOption(EType type, bool isBlink=false, unsigned int pollIntervalMs=500) = 0;
	// Observer 등록 메서드 추가
	virtual void attach(IObserver* observer) = 0;
};