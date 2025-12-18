#include "stdafx.h"
#include "COPSwitch.h"

// static 멤버 변수 초기화
IDio* COPSwitch::m_pIo = nullptr;

COPSwitch::COPSwitch()
    : m_BlinkTimer(nullptr)
{
}

COPSwitch::~COPSwitch()
{
    if (m_BlinkTimer)
    {
        delete m_BlinkTimer;
        m_BlinkTimer = nullptr;
    }
}

void COPSwitch::setIo(IDio* pIo)
{
    m_pIo = pIo;
}

IOPSwitch& COPSwitch::setGroup(IOPSwitch* pObject)
{
    m_pGroup = pObject;
    return *this;
}
IOPSwitch& COPSwitch::setBlink(bool bStatus)
{
    m_isBlink = bStatus;
	return *this;
}

IOPSwitch& COPSwitch::setOption(EType type, bool isBlink, unsigned int blinkTime)
{
    m_type = type;
    m_isBlink = isBlink;
    if (m_BlinkTimer)
    {
        delete m_BlinkTimer;
    }
    m_BlinkTimer = new CTimer(static_cast<long long>(blinkTime * 1000)); // blinkTime이 초 단위라면 ms로 변환

	return *this;
}

void COPSwitch::setInput(std::initializer_list<int> inputs)
{
    m_inputs.assign(inputs);
}

void COPSwitch::setOutput(std::initializer_list<int> outputs)
{
    m_outputs.assign(outputs);
}

bool COPSwitch::getSwitchStatus() 
{
    return m_status.load();
}

void COPSwitch::setSwitchStatus(bool bStatus)
{
    bool expected = !bStatus;
    if (m_status.compare_exchange_strong(expected, bStatus))
    {
        notify((m_status ? "ON" : "OFF"));
    }
}

bool COPSwitch::sequence()
{
    std::lock_guard<std::mutex> lock(m_logicMutex);

    bool in_sensor = checkInSensor();

    switch (m_type)
    {
    case EType::KEEP:
        setSwitchStatus(in_sensor);
        break;
    case EType::PUSH:
        setSwitchStatus(in_sensor);
        break;
    case EType::TOGGLE:
        if (in_sensor && !m_toggleFlag)
        {
            m_toggleFlag = true;
            setSwitchStatus(!getSwitchStatus());
        }
        else if (!in_sensor)
        {
            m_toggleFlag = false;
        }
        break;
    }

    if (m_isBlink)
    {
        if (getSwitchStatus())
        {
            if (m_BlinkTimer->isOver())
            {
                m_blinkStatus = !m_blinkStatus;
                setLED(m_blinkStatus);
            }
        }
        else
        {
            setLED(false);
        }
    }
    else
    {
        setLED(getSwitchStatus());
    }

    if (m_pGroup)
    {
        if (getSwitchStatus())
        {
            m_pGroup->setSwitchStatus(false);
        }
    }

    return true;
}

void COPSwitch::setLED(bool bStatus)
{
    if (m_pIo)
    {
        for (int ch : m_outputs)
        {
            m_pIo->out(ch, bStatus);
        }
    }
}

bool COPSwitch::checkInSensor()
{
    if (m_pIo)
    {
        for (int ch : m_inputs)
        {
            if (m_pIo->in(ch)) return true;
        }
    }
    return false;
}