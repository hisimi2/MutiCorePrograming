#pragma once
#include "Framework/IOPSwitch.h"
#include "CTimer.h"
#include <vector>
#include <atomic>
#include <chrono>
#include "IDio.h"
#include "Framework/CAbsThread.h"
#include <mutex>

// COPSwitch: I/O 스위치 제어 클래스
class COPSwitch : public CAbsThread, public IOPSwitch
{
	IDio& m_DIO;
public:
	enum class EType { PUSH, TOGGLE, KEEP };

	// pollIntervalMs: 내부에서 실작업을 수행하는 최소 주기 (밀리초)
	COPSwitch(IDio& Dio);
	~COPSwitch();

	// 복사 금지 (std::thread 등을 포함하므로 안전하게 비활성화)
	COPSwitch(const COPSwitch&) = delete;
	COPSwitch& operator=(const COPSwitch&) = delete;

	// IOPSwitch 인터페이스 구현
	bool getStatus() override;
	void setStatus(BOOL bStatus) override;

	// 입/출력 설정
	void setInput(const std::vector<int>& inputs);
	void setOutput(const std::vector<int>& outputs);

	void setGroup(COPSwitch* pObject);
	void setBlink(bool bStatus);
	void setOption(EType type, bool isBlink, unsigned int pollIntervalMs); // setDio 선언 추가

private:
	void setLED(bool bStatus);
	bool checkInSensor();

	// CAbsThread의 가상 함수 오버라이드: 주기 작업을 여기서 수행
	int sequence() override;

	CTimer m_BlinkTimer;
	std::vector<int> m_inputs;
	std::vector<int> m_outputs;

	std::atomic<bool> m_status{ false }; // 스레드 안전하게 변경/열람
	// 토글/블링 상태
	bool m_toggleFlag = true;
	bool m_blinkStatus = false;
	EType m_type = EType::KEEP;
	bool m_isBlink = false;
	COPSwitch* m_pGroup = nullptr;

	// poll 간격 (밀리초)
	unsigned int m_pollIntervalMs = 100;
	// 마지막으로 실제 작업을 수행한 시점
	std::chrono::steady_clock::time_point m_lastPoll;

	// DIO 접근 동기화 (IDio 구현이 자체 동기화를 제공하지 않으면 필요)
	mutable std::mutex m_dioMutex;
};

