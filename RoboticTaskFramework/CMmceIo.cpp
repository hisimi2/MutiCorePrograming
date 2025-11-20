#include "stdafx.h"
#include "CMmceIo.h"

CMmceIo::CMmceIo()
{
}
CMmceIo::~CMmceIo()
{
}
void CMmceIo::out(int nChannel, bool bStatus)
{
	// MMCE 하드웨어에 출력 신호를 설정하는 코드 구현
	// 예: MMCE_SetOutput(nChannel, bStatus);
}
bool CMmceIo::out(int nChannel)
{
	// MMCE 하드웨어에서 출력 신호 상태를 읽는 코드 구현
	// 예: return MMCE_GetOutput(nChannel);
	return false; // 자리 표시자 반환값
}
bool CMmceIo::in(int nChannel)
{
	// MMCE 하드웨어에서 입력 신호 상태를 읽는 코드 구현
	// 예: return MMCE_GetInput(nChannel);
	return false; // 자리 표시자 반환값
}

