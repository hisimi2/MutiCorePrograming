#pragma once
#include "IDio.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>


class CMmceIo : public IDio
{
	int m_nTotalBytes = 16; // 예시: 총 16바이트의 I/O 데이터

public:
    CMmceIo();
    ~CMmceIo() override;
    void out(int nChannel, bool bStatus) override;
    bool out(int nChannel) override;
    bool in(int nChannel) override;

private:
    void ioThreadFunc(); // 전용 I/O 스레드가 실행할 함수
    void startIoThread();
    void stopIoThread();

    std::vector<byte> UpdateHardwareAndWait(const std::vector<byte>& outputBuffer);

    std::thread m_ioThread;
    std::atomic<bool> m_stopIoThread{ false };

    // 공유 버퍼
    std::mutex m_ioMutex;
    std::vector<byte> m_inputBuffer;
    std::vector<byte> m_outputBuffer;
};

