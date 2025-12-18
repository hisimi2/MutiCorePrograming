#pragma once

#include "IOPSwitch.h"
#include "IPeriodicTask.h"
#include "CSubject.h"
#include "IDio.h"
#include "CTimer.h"
#include <vector>
#include <atomic>
#include <mutex>
#include <initializer_list>

class COPSwitch : public IOPSwitch, public IPeriodicTask, public CSubject
{
public:
    COPSwitch();
    virtual ~COPSwitch();

    // IOPSwitch 인터페이스 구현
    bool getSwitchStatus() override;
    void setSwitchStatus(bool bStatus) override;
    void attach(IObserver* observer) override { CSubject::attach(observer); }


    IOPSwitch& setGroup(IOPSwitch* pObject);
    IOPSwitch& setBlink(bool bStatus);
    IOPSwitch& setOption(EType type, bool isBlink, unsigned int pollIntervalMs);
 
 



    // COPSwitch 고유 메서드
    static void setIo(IDio* pIo);
    void setInput(std::initializer_list<int> inputs);
    void setOutput(std::initializer_list<int> outputs);

    
    

    // IPeriodicTask 인터페이스 구현
    bool sequence() override;

private:
    void setLED(bool bStatus);
    bool checkInSensor();
    static IDio* m_pIo;

    std::vector<int> m_inputs;
    std::vector<int> m_outputs;

    std::atomic<bool> m_status{ false };
    bool m_toggleFlag = false;
    bool m_blinkStatus = false;
    EType m_type = EType::PUSH;
    bool m_isBlink = false;
    IOPSwitch* m_pGroup = nullptr;
    unsigned int m_pollIntervalMs = 10; // 폴링 주기 (ms)

    CTimer* m_BlinkTimer = nullptr;
    std::mutex m_logicMutex; // 로직 접근 동기화를 위한 뮤텍스
};

