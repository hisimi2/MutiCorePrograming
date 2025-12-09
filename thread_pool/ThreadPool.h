#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

class ThreadPool {
public:
    // 생성자: 지정된 수만큼 워커 스레드를 생성하고 대기시킵니다.
    ThreadPool(size_t threads);
    // 소멸자: 모든 스레드를 join합니다.
    ~ThreadPool();

    // 작업을 큐에 추가합니다.
    // std::future를 반환하여 작업의 결과값을 나중에 받을 수 있습니다.
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>;

private:
    // 워커 스레드들을 저장할 벡터
    std::vector<std::thread> workers;
    // 작업들을 저장할 큐
    std::queue<std::function<void()>> tasks;

    // 동기화를 위한 뮤텍스와 조건 변수
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop; // 스레드 풀을 중지하기 위한 플래그
};

// 생성자 구현
inline ThreadPool::ThreadPool(size_t threads) : stop(false) {
    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    // 뮤텍스로 큐를 잠그고, 작업이 들어오거나 스레드 풀이 멈출 때까지 대기
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });

                    // 스레드 풀이 멈췄고 큐가 비어있으면 스레드 종료
                    if (this->stop && this->tasks.empty()) {
                        return;
                    }

                    // 큐에서 작업 하나를 가져옴
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                // 가져온 작업을 실행
                task();
            }
        });
    }
}

// 소멸자 구현
inline ThreadPool::~ThreadPool() {
    {
        // 뮤텍스를 잠그고 스레드 풀 중지 플래그를 true로 설정
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    // 대기 중인 모든 스레드를 깨움
    condition.notify_all();
    // 모든 워커 스레드가 종료될 때까지 대기
    for (std::thread& worker : workers) {
        worker.join();
    }
}

// 작업을 큐에 추가하는 enqueue 함수 구현
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type> {
    
    using return_type = typename std::result_of<F(Args...)>::type;

    // std::packaged_task를 사용하여 함수와 인자를 묶음
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    // std::future를 통해 나중에 결과값을 받을 수 있도록 함
    std::future<return_type> res = task->get_future();
    {
        // 뮤텍스를 잠그고 작업을 큐에 추가
        std::unique_lock<std::mutex> lock(queue_mutex);

        if (stop) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }

        tasks.emplace([task]() { (*task)(); });
    }
    // 대기 중인 스레드 하나를 깨워 작업을 처리하도록 함
    condition.notify_one();
    return res;
}