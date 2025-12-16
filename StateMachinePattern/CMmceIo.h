#pragma once
#include "IDio.h"
#include "IPeriodicTask.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>


class CMmceIo : public IDio, public IPeriodicTask
{
    bool sequence() override;

public:
    CMmceIo();
    ~CMmceIo() override;
    void out(int nChannel, bool bStatus) override;
    bool out(int nChannel) override;
    bool in(int nChannel) override;

    void startIoThread();
    void stopIoThread();

private:
    void ioThreadFunc(); // 전용 I/O 스레드가 실행할 함수
    void prepareOutputBuffer(); // 추가: prepareOutputBuffer 선언
    void processInputBuffer(); // 추가: prepareOutputBuffer 선언
    std::vector<byte> UpdateHardwareAndWait(const std::vector<byte>& outputBuffer);

    std::thread m_ioThread;
    std::atomic<bool> m_stopIoThread{ false };

    // 공유 버퍼
    std::mutex m_ioMutex;
    std::vector<byte> m_inputBuffer;
    std::vector<byte> m_outputBuffer;
};

