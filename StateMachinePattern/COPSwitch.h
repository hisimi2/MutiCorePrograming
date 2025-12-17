#pragma once
#include "IOPSwitch.h"
#include "CTimer.h"
#include <vector>
#include <atomic>
#include <chrono>
#include "IDio.h"
#include "Framework/IPeriodicTask.h" // 경로 수정
#include <mutex>

// COPSwitch: I/O 스위치 제어 클래스
class COPSwitch : public IOPSwitch, public IPeriodicTask
{
	IDio& m_Io;
public:
	enum class EType { PUSH, TOGGLE, KEEP };

	COPSwitch(IDio& Dio);
	~COPSwitch();

	// 복사 생성자 및 대입 연산자 비활성화
	COPSwitch(const COPSwitch&) = delete;
	COPSwitch& operator=(const COPSwitch&) = delete;

	// IOPSwitch 인터페이스 구현
	bool getSwitchStatus() override; // const 추가
	void setSwitchStatus(bool bStatus) override; 

	// IPeriodicTask 인터페이스 구현
	bool sequence() override;

	// 입/출력 설정
	COPSwitch& setInput(const std::vector<int>& inputs);
	COPSwitch& setOutput(const std::vector<int>& outputs);

	COPSwitch& setGroup(COPSwitch* pObject);
	COPSwitch& setBlink(bool bStatus);
	COPSwitch& setOption(EType type, bool isBlink = false, unsigned int pollIntervalMs = 0);

private:
	// sequence 로직을 더 작은 단위로 분리
	void updateStatusFromSensor(bool sensorStatus);
	void updateLed();
	void setLED(bool bStatus);
	bool checkInSensor();

	CTimer m_BlinkTimer;
	std::vector<int> m_inputs;
	std::vector<int> m_outputs;

	std::atomic<bool> m_status{ false }; // 스레드 안전하게 상태 관리
	// 토글/블링 상태
	bool m_toggleFlag = true;
	bool m_blinkStatus = false;
	EType m_type = EType::KEEP;
	bool m_isBlink = false;
	COPSwitch* m_pGroup = nullptr;

	// poll 간격 (밀리초) - 이제 스케줄러 주기에 의해 제어됨
	unsigned int m_pollIntervalMs = 100;

	// 로직 접근 동기화 (이름 변경)
	mutable std::mutex m_logicMutex;
};

