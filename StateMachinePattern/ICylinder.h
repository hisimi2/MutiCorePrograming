#pragma once

class ICylinder
{
public:
    virtual ~ICylinder() = default;

    // 실린더의 두 가지 기본 동작을 위한 순수 가상 함수
    virtual int actA(bool bManual = false) = 0;
    virtual int actB(bool bManual = false) = 0;

    // 실린더의 상태를 확인하기 위한 순수 가상 함수
    virtual bool isActA() = 0;
    virtual bool isActB() = 0;
};
