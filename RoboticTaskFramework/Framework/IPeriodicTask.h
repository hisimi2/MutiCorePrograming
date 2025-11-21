#pragma once

// CAbsThread.h 파일의 이름을 IPeriodicTask.h로 변경하고 아래와 같이 수정합니다.

class IPeriodicTask
{
public:
    virtual ~IPeriodicTask() = default;

    /// <summary>
    /// 이 메서드는 스레드 풀의 워커 스레드에 의해 주기적으로 호출됩니다.
    /// </summary>
    virtual bool sequence() = 0;
};