#pragma once
#include "Framework/IOPSwitch.h"
#include "CTimer.h"
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include "IDio.h"

class COPSwitch : public IOPSwitch
{
	IDio& m_DIO;
public:
	enum class EType { PUSH, TOGGLE, KEEP };

	// pollIntervalMs: 내부 스레드가 sequence()를 호출하는 주기 (밀리초)
	COPSwitch(IDio& Dio);
	~COPSwitch();

	// IOPSwitch 인터페이스 구현
	BOOL getStatus() override;
	void setStatus(BOOL bStatus) override;

	// 입/출력 설정
	void setInput(const std::vector<int>& inputs);
	void setOutput(const std::vector<int>& outputs);

	void setGroup(COPSwitch* pObject);
	void setBlink(bool bStatus);
	void setOption(EType type, bool isBlink, unsigned int pollIntervalMs); // setDio 선언 추가

	// 외부에서 직접 호출하던 sequence()는 내부에서 주기적으로 실행됩니다.
	// (필요하면 public으로 유지할 수 있으나 여기서는 private으로 둡니다.)

private:
	void setLED(bool bStatus);
	bool checkInSensor();
	int sequence();

	// 내부 스레드 루프
	void threadLoop();

	CTimer m_BlinkTimer;
	std::vector<int> m_inputs;
	std::vector<int> m_outputs;

	std::atomic<bool> m_status{ false }; // 스레드 안전하게 변경/열람
	bool m_toggleFlag = true;
	bool m_blinkStatus = false;
	EType m_type;
	bool m_isBlink = false;
	COPSwitch* m_pGroup = nullptr;

	// 스레드 관련
	std::thread m_thread;
	std::atomic<bool> m_threadExit{ false };
	unsigned int m_pollIntervalMs = 100;
};

