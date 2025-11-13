#pragma once

class CUnit; // 전방 선언

// 모든 시퀀스 단계의 기반이 되는 인터페이스
class IStep
{
public:
	virtual ~IStep() = default;
	// 단계를 실행하고 다음 단계를 반환합니다.
	virtual IStep* execute(CUnit* pUnit) = 0;
};