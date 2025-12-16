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
class COPSwitch : public IPeriodicTask, public IOPSwitch
{
	IDio& m_Io;
	// m_bStatus는 m_status(atomic)으로 대체하여 스레드 안전성 확보
public:
	enum class EType { PUSH, TOGGLE, KEEP };

	COPSwitch(IDio& Dio);
	~COPSwitch();

	// 복사 생성자 및 대입 연산자 비활성화
	COPSwitch(const COPSwitch&) = delete;
	COPSwitch& operator=(const COPSwitch&) = delete;

	// IOPSwitch 인터페이스 구현
	bool getSwitchStatus() override;
	// E1455 오류 수정: IOPSwitch::setSwitchStatus는 'bool'을 파라미터로 받으므로 BOOL이 아닌 bool로 수정합니다.
	void setSwitchStatus(bool bStatus) override; 

	// IPeriodicTask 인터페이스 구현
	// E0317 오류 수정: IPeriodicTask::sequence는 'bool'을 반환하므로 void가 아닌 bool로 수정합니다.
	bool sequence() override;

	// 입/출력 설정
	COPSwitch& setInput(const std::vector<int>& inputs);
	COPSwitch& setOutput(const std::vector<int>& outputs);

	COPSwitch& setGroup(COPSwitch* pObject);
	COPSwitch& setBlink(bool bStatus);
	COPSwitch& setOption(EType type, bool isBlink = false, unsigned int pollIntervalMs = 0);

private:
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

	// DIO 접근 동기화
	mutable std::mutex m_dioMutex;
};

