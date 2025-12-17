#pragma once

#include "ctpl_stl.h" // CTPL 헤더
#include "IPeriodicTask.h"
#include <vector>
#include <chrono>
#include <atomic>
#include <thread>
#include <memory>
#include <future> // std::future 사용을 위해 추가

class Scheduler
{
public:
    // 스케줄러는 CTPL 스레드 풀에 대한 참조를 가집니다.
    Scheduler(ctpl::thread_pool& pool, std::chrono::milliseconds interval);
    ~Scheduler();

    void addTask(std::shared_ptr<IPeriodicTask> task);
    void start();
    void stop();

private:
    void run();

    ctpl::thread_pool& m_pool;
    std::vector<std::shared_ptr<IPeriodicTask>> m_tasks;
    std::thread m_schedulerThread;
    std::atomic<bool> m_stop{ false };
    std::chrono::milliseconds m_interval;
};

inline Scheduler::Scheduler(ctpl::thread_pool& pool, std::chrono::milliseconds interval)
    : m_pool(pool), m_interval(interval)
{
}

inline Scheduler::~Scheduler()
{
    stop();
}

inline void Scheduler::addTask(std::shared_ptr<IPeriodicTask> task)
{
    m_tasks.push_back(task);
}

inline void Scheduler::start()
{
    m_schedulerThread = std::thread(&Scheduler::run, this);
}

inline void Scheduler::stop()
{
    m_stop = true;
    if (m_schedulerThread.joinable())
    {
        m_schedulerThread.join();
    }
}

inline void Scheduler::run()
{
    // 타임아웃 시간을 스케줄러 주기보다 약간 길게 설정합니다.
    const auto timeout = m_interval + std::chrono::milliseconds(5);

    while (!m_stop)
    {
        auto start_time = std::chrono::steady_clock::now();

        std::vector<std::future<bool>> futures;
        for (auto& task : m_tasks)
        {
            futures.push_back(m_pool.push([task](int /*id*/) {
                return task->sequence();
                }));
        }

        // 모든 작업이 완료될 때까지 '타임아웃을 가지고' 기다립니다.
        for (size_t i = 0; i < futures.size(); ++i)
        {
            auto& f = futures[i];
            // wait_for는 지정된 시간 동안만 기다립니다.
            std::future_status status = f.wait_for(timeout);

            if (status == std::future_status::timeout)
            {
                // 타임아웃이 발생한 경우, 심각한 문제이므로 로그를 남깁니다.
                // 이 로그를 통해 어떤 작업이 문제를 일으키는지 찾을 수 있습니다.
                // 예: TRACE("작업 %zu번이 타임아웃되었습니다.\n", i);
                
                // 여기에 정책을 추가할 수 있습니다. (예: 비상 정지)
            }
            else if (status == std::future_status::ready)
            {
                // 정상적으로 완료된 경우, 결과를 얻을 수 있습니다.
                // bool result = f.get(); 
            }
        }

        auto end_time = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        // 다음 주기까지 대기
        if (elapsed < m_interval)
        {
            std::this_thread::sleep_for(m_interval - elapsed);
        }
    }
}