#pragma once

#include "ctpl_stl.h" // CTPL 헤더
#include "IPeriodicTask.h"
#include <vector>
#include <chrono>
#include <atomic>
#include <thread>

class Scheduler
{
public:
    // 스케줄러는 CTPL 스레드 풀에 대한 참조를 가집니다.
    Scheduler(ctpl::thread_pool& pool, std::chrono::milliseconds interval);
    ~Scheduler();

    void addTask(IPeriodicTask* task);
    void start();
    void stop();

private:
    void run();

    ctpl::thread_pool& m_pool;
    std::vector<IPeriodicTask*> m_tasks;
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

inline void Scheduler::addTask(IPeriodicTask* task)
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
    while (!m_stop)
    {
        auto start_time = std::chrono::steady_clock::now();

        // 모든 등록된 작업에 대해 sequence() 실행을 CTPL 스레드 풀에 요청
        for (auto& task : m_tasks)
        {
            // CTPL의 push 메서드를 사용하여 작업을 큐에 넣습니다.
            m_pool.push([task](int /*id*/) {
                task->sequence();
                });
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