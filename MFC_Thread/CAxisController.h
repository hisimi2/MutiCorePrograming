#pragma once
#include "CSubject.h"

// 기존 코드에 맞춰 CAxisController에 attach 메서드 추가
class CAxisController : public CSubject
{
public:
    CAxisController() {}
    virtual ~CAxisController() {}

    // attach 메서드 추가
    void attach(IObserver* pObserver) {
        CSubject::attach(pObserver);
    }

    // 기존 move 등 필요한 메서드 정의
    void move(int position) {
        
		notify("Axis moved to position: " + std::to_string(position));
        Sleep(1000);
    }
};

