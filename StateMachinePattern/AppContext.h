#pragma once

#include <memory>
#include "CMmceIo.h"
#include "COPSwitch.h"
#include "CRobot.h"
#include "Scheduler.h"
#include "ctpl_stl.h"

// AppContext.h : 모든 객체의 생성과 조립을 담당
class AppContext
{
public:
    AppContext()
    {
        Initialize();
    }

    ~AppContext()
    {
        if (m_pScheduler)
        {
            m_pScheduler->stop();
        }
    }

    // 초기화 로직을 한 곳에 집중
    void Initialize()
    {
        // 1. IO 초기화
        m_pMmceIo = std::make_unique<CMmceIo>();
        COPSwitch::setIo(m_pMmceIo.get());

        // 2. Switch 생성
        m_pStartSwitch = std::make_shared<COPSwitch>("StartSwitch");
        m_pStartSwitch->setOption(IOPSwitch::EType::TOGGLE);

        // 3. Robot 생성
        m_pRobot = std::make_shared<CRobot>(*m_pStartSwitch);

        // 4. 스케줄러 및 스레드 풀 초기화
        unsigned int core_count = std::thread::hardware_concurrency();
        m_pThreadPool = std::make_unique<ctpl::thread_pool>(core_count > 0 ? core_count : 4);
        m_pScheduler = std::make_unique<Scheduler>(*m_pThreadPool, std::chrono::milliseconds(10));

        // 5. 스케줄러에 작업 등록
        m_pScheduler->addTask(m_pStartSwitch);
        m_pScheduler->addTask(m_pRobot);
    }

    void StartScheduler()
    {
        if (m_pScheduler)
        {
            m_pScheduler->start();
        }
    }

    // Getter 메서드 (필요한 객체만 노출)
    std::shared_ptr<IOPSwitch> GetStartSwitch() const { return m_pStartSwitch; }
    std::shared_ptr<CRobot> GetRobot()          const { return m_pRobot; }

private:
    std::unique_ptr<IDio>               m_pMmceIo;
    std::shared_ptr<COPSwitch>          m_pStartSwitch;
    std::shared_ptr<CRobot>             m_pRobot;
    std::unique_ptr<ctpl::thread_pool>  m_pThreadPool;
    std::unique_ptr<Scheduler>          m_pScheduler;
};
