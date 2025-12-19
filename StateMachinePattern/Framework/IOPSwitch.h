#pragma once
#include "IPeriodicTask.h"
#include "IObserver.h"
#include "CSubject.h"

// IOPSwitch가 IPeriodicTask를 상속받아 주기적 실행 기능을 포함시킵니다.
class IOPSwitch : public IPeriodicTask, public CSubject
{
public:
    enum class EType { KEEP, PUSH, TOGGLE };

    virtual ~IOPSwitch() {}

    // 스위치 고유 기능
    virtual bool getSwitchStatus() = 0;
    virtual void setSwitchStatus(bool bStatus) = 0;
    
    // 설정 관련 메서드 (반환 타입을 IOPSwitch&로 유지하여 체이닝 지원)
    virtual IOPSwitch& setGroup(IOPSwitch* pObject) = 0;
    virtual IOPSwitch& setBlink(bool bStatus) = 0;
    virtual IOPSwitch& setOption(EType type, bool isBlink = false, unsigned int pollIntervalMs = 500) = 0;
};