#include "stdafx.h"
#include "CMmceIo.h"

CMmceIo::CMmceIo()
{
	startIoThread();
}

CMmceIo::~CMmceIo()
{
	stopIoThread();
}

void CMmceIo::out(int nChannel, bool bStatus)
{
	// 출력번호의 데이터를 Bit연산하여 전체 출력 byte에 반영하는 코드 구현
}

bool CMmceIo::out(int nChannel)
{
	// 전체 출력 byte를 가지고 해당 nChannel의 상태를 Bit연산으로 추출하는 코드 구현
	return false; // 자리 표시자 반환값
}

bool CMmceIo::in(int nChannel)
{
	// 전체 입력 byte를 가지고 해당 nChannel의 상태를 Bit연산으로 추출하는 코드 구현
	return false; // 자리 표시자 반환값
}

// 전용 I/O 스레드가 실행할 함수
// 느린 Blocking API 호출을 수행
void CMmceIo::ioThreadFunc()
{
    while (!m_stopIoThread)
    {
        std::vector<byte> localOutputBuffer;
        {
            // 공유 버퍼에서 출력 데이터를 복사
            std::lock_guard<std::mutex> lock(m_ioMutex);
            localOutputBuffer = m_outputBuffer;
        }

        // 여기서 느린 Blocking API 호출
        std::vector<byte> localInputBuffer = UpdateHardwareAndWait(localOutputBuffer);

        {
            // 공유 버퍼에 최신 입력 데이터를 씀
            std::lock_guard<std::mutex> lock(m_ioMutex);
            m_inputBuffer = localInputBuffer;
        }

        Sleep(10); // I/O 스레드 자체의 주기
    }
}

// 스케줄러가 10ms마다 호출하는 매우 빠른 함수
bool CMmceIo::sequence()
{
	// 출력 버퍼 준비 및 입력 버퍼 처리
    prepareOutputBuffer(); 
    processInputBuffer();

    return true;
}

void CMmceIo::prepareOutputBuffer()
{
    // 출력 버퍼를 준비하는 코드 구현
    // 예시: m_outputBuffer를 원하는 값으로 설정
    // std::lock_guard<std::mutex> lock(m_ioMutex);
    // m_outputBuffer = ...;
}

void CMmceIo::processInputBuffer()
{
    // 입력 버퍼를 처리하는 코드 구현
    // 예시: m_inputBuffer의 데이터를 해석하여 필요한 동작 수행
}

std::vector<byte> CMmceIo::UpdateHardwareAndWait(const std::vector<byte>& outputBuffer)
{
    // 실제 하드웨어와 통신하는 코드 구현 필요
    // 예시: 입력 버퍼와 동일한 크기의 더미 데이터 반환
    std::vector<byte> dummyInputBuffer(outputBuffer.size(), 0);
    // TODO: 하드웨어 I/O 처리 구현
    return dummyInputBuffer;
}

void CMmceIo::startIoThread()
{
    // m_stopIoThread 플래그를 false로 초기화
    m_stopIoThread = false;
    // &CMmceIo::ioThreadFunc 멤버함수 포인터와
    // 이 함수를 실행할 객체의 인스턴스(this)를 전달하여 스레드 생성 및 시작
    m_ioThread = std::thread(&CMmceIo::ioThreadFunc, this);
}

void CMmceIo::stopIoThread()
{
    m_stopIoThread = true; // 스레드 루프 종료 신호
    if (m_ioThread.joinable())
    {
        m_ioThread.join(); // 스레드가 끝날 때까지 대기
    }
}

