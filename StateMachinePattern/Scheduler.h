#pragma once

#include "ctpl_stl.h" // CTPL 헤더
#include "IPeriodicTask.h"
#include <vector>
#include <chrono>
#include <atomic>
#include <thread>
#include <memory>
#include <future> // std::future 사용을 위해 추가
#include <map>    // std::map 사용을 위해 추가

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
    // 각 작업의 future와 실행 상태를 관리합니다.
    std::map<IPeriodicTask*, std::future<bool>> m_taskFutures;
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
    while (!m_stop)
    {
        auto start_time = std::chrono::steady_clock::now();

        for (auto& task : m_tasks)
        {
            auto it = m_taskFutures.find(task.get());
            bool isRunning = false;

            if (it != m_taskFutures.end())
            {
                // 작업이 이미 실행 중인지 확인합니다.
                // wait_for(0)는 즉시 리턴하며, 작업 완료 여부를 확인할 수 있습니다.
                if (it->second.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
                {
                    isRunning = true;
                    TRACE("작업이 아직 실행 중입니다. 이번 주기는 건너뜁니다.\n");
                }
                else
                {
                    // 완료되었다면 맵에서 제거합니다.
                    it->second.get(); // 예외가 있다면 가져옵니다.
                    m_taskFutures.erase(it);
                }
            }

            // 작업이 실행 중이 아닐 때만 스레드 풀에 추가합니다.
            if (!isRunning)
            {
                auto future = m_pool.push([task = task](int /*id*/) {
                    return task->sequence();
                    });
                m_taskFutures[task.get()] = std::move(future);
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