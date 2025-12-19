#pragma once

#include "IOPSwitch.h"
#include "CSubject.h"
#include "IDio.h"
#include "CTimer.h"
#include <vector>
#include <atomic>
#include <mutex>
#include <initializer_list>

// 상속 구조 단순화: IPeriodicTask는 IOPSwitch에 포함됨
class COPSwitch : public IOPSwitch 
{
public:
    COPSwitch(std::string strName);
    virtual ~COPSwitch();

    // IOPSwitch (및 IPeriodicTask) 인터페이스 구현
    bool getSwitchStatus() override;
    void setSwitchStatus(bool bStatus) override;
    
    
    IOPSwitch& setGroup(IOPSwitch* pObject) override;
    IOPSwitch& setBlink(bool bStatus) override;
    IOPSwitch& setOption(EType type, bool isBlink, unsigned int pollIntervalMs) override;

    // IPeriodicTask 구현
    bool sequence() override;

    // COPSwitch 고유 메서드
    static void setIo(IDio* pIo);
    void setInput(std::initializer_list<int> inputs);
    void setOutput(std::initializer_list<int> outputs);

private:
    std::string m_strName;
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
    unsigned int m_pollIntervalMs = 10;

    CTimer* m_BlinkTimer = nullptr;
    std::mutex m_logicMutex;
};

