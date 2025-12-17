#pragma once

class IPeriodicTask
{
public:
    virtual ~IPeriodicTask() = default;

    /// <summary>
    /// 이 메서드는 스레드 풀의 워커 스레드에 의해 주기적으로 호출됩니다.
    /// non-blocking 작업만 수행해야 합니다.
    /// </summary>
    virtual bool sequence() = 0;
};