#include "stdafx.h"
#include "COPSwitch.h"

COPSwitch::COPSwitch(IDio& io)
	: m_Io(io)
	, m_bStatus(FALSE)
	, m_BlinkTimer(500)
{
	// 생성자에서는 스레드를 시작하지 않습니다.
	// resume()을 통해 명시적으로 시작해야 합니다.
}

COPSwitch::~COPSwitch()
{
	// 소멸 시 스레드가 실행 중이면 안전하게 종료 신호를 보냅니다.
	// CAbsThread의 소멸자가 join()으로 실제 종료를 기다립니다.
	setEnd();
}

bool COPSwitch::getStatus()
{
	return m_bStatus;
}

void COPSwitch::setStatus(BOOL bStatus)
{
	m_bStatus = bStatus;
}

bool COPSwitch::sequence()
{
	// 이 함수는 백그라운드 스레드에서 주기적으로 호출됩니다.
	if (m_bStatus)
	{
		// ON 상태일 때의 로직 (예: 특정 I/O 포트 켜기)
		// m_Io.out(CHANNEL_START_SWITCH, true);
	}
	else
	{
		// OFF 상태일 때의 로직 (예: 특정 I/O 포트 끄기)
		// m_Io.out(CHANNEL_START_SWITCH, false);
	}
	Sleep(10); // CPU 사용량 조절을 위한 짧은 대기
	return false; // 스레드를 계속 실행하기 위해 false 반환
}
