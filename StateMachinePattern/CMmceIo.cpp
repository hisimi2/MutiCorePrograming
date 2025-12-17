#include "stdafx.h"
#include "CMmceIo.h"

CMmceIo::CMmceIo()
{
	// m_inputBuffer와 m_outputBuffer를 초기 크기로 할당합니다.
	// m_nTotalBytes는 헤더에 정의되어 있다고 가정합니다.
	m_inputBuffer.resize(m_nTotalBytes, 0);
	m_outputBuffer.resize(m_nTotalBytes, 0);
	startIoThread();
}

CMmceIo::~CMmceIo()
{
	stopIoThread();
}

void CMmceIo::out(int nChannel, bool bStatus)
{
	// 공유 버퍼에 접근하기 전에 뮤텍스를 잠급니다.
	std::lock_guard<std::mutex> lock(m_ioMutex);

	// 출력번호의 데이터를 Bit연산하여 전체 출력 byte에 반영하는 코드 구현
    if ((m_nTotalBytes * 8) <= nChannel) // 등호 포함으로 수정
    {
        TRACE("Buffer Number is too much high.");
        return;
    }
    unsigned int uIndex = (UINT)(nChannel / 8);
    if (FALSE == bStatus)
    {
        m_outputBuffer[uIndex] &= ~(1 << (nChannel % 8));
    }
    else
    {
        m_outputBuffer[uIndex] |= 1 << (nChannel % 8);
    }
}

bool CMmceIo::out(int nChannel)
{
	// 공유 버퍼에 접근하기 전에 뮤텍스를 잠급니다.
	std::lock_guard<std::mutex> lock(m_ioMutex);

	// 전체 출력 byte를 가지고 해당 nChannel의 상태를 Bit연산으로 추출하는 코드 구현
    if ((m_nTotalBytes * 8) <= nChannel)
    {
        return false;
    }
    unsigned int uIndex = (UINT)(nChannel / 8);
    bool bReturnValue = ((m_outputBuffer[uIndex] >> (nChannel % 8)) & 1) != 0;
    return bReturnValue;
}

bool CMmceIo::in(int nChannel)
{
	// 공유 버퍼에 접근하기 전에 뮤텍스를 잠급니다.
	std::lock_guard<std::mutex> lock(m_ioMutex);

	// 전체 입력 byte를 가지고 해당 nChannel의 상태를 Bit연산으로 추출하는 코드 구현
    if ((m_nTotalBytes * 8) <= nChannel)
    {
        return false;
    }
    unsigned int uIndex = (UINT)(nChannel / 8);
    bool bReturnValue = ((m_inputBuffer[uIndex] >> (nChannel % 8)) & 1) != 0;

    return bReturnValue;
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

std::vector<byte> CMmceIo::UpdateHardwareAndWait(const std::vector<byte>& outputBuffer)
{
    // 실제 하드웨어와 통신하는 코드 구현 필요
    // 예시: 입력 버퍼와 동일한 크기의 더미 데이터 반환
    std::vector<byte> dummyInputBuffer(outputBuffer.size(), 0);
    // TODO: 하드웨어 I/O 처리 구현
    Sleep(1); // 실제 API가 약간의 딜레이가 있다고 가정
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

