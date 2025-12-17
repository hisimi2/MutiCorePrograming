#pragma once
#include "stdafx.h"
#include "Framework/IOPSwitch.h"
#include "Framework/IPeriodicTask.h"
#include "IDio.h"
#include "CTimer.h"
#include <vector>
#include <atomic>
#include <mutex>
#include "CSubject.h" // CSubject 추가

class COPSwitch : public IOPSwitch, public IPeriodicTask, public CSubject
{
public:
	using CSubject::notify; // notify(string)을 사용할 수 있도록 추가
	enum class EType { KEEP, PUSH, TOGGLE };

	COPSwitch(IDio& io);
	virtual ~COPSwitch();

	// IOPSwitch 인터페이스 구현
	bool getSwitchStatus() override;
	void setSwitchStatus(bool bStatus) override;

	// IPeriodicTask 인터페이스 구현
	bool sequence() override;

	COPSwitch& setInput(const std::vector<int>& inputs);
	COPSwitch& setOutput(const std::vector<int>& outputs);
	COPSwitch& setGroup(COPSwitch* pObject);
	COPSwitch& setBlink(bool bStatus);
	COPSwitch& setOption(EType type, bool isBlink = false, unsigned int pollIntervalMs = 10);

private:
	void updateStatusFromSensor(bool sensorStatus);
	void updateLed();
	void setLED(bool bStatus);
	bool checkInSensor();

	IDio& m_Io;
	std::vector<int> m_inputs;
	std::vector<int> m_outputs;

	std::atomic<bool> m_status{ false };
	bool m_toggleFlag = false;
	bool m_blinkStatus = false;
	EType m_type = EType::PUSH;
	bool m_isBlink = false;
	COPSwitch* m_pGroup = nullptr;
	unsigned int m_pollIntervalMs = 10;

	CTimer m_BlinkTimer;
	std::mutex m_logicMutex;
};

