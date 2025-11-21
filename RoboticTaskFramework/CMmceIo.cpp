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
	// 출력번호의 데이터를 Bit연산하여 전체 출력 byte에 반영하는 코드 구현
}
bool CMmceIo::out(int nChannel)
{
	// 전체 출력 byte를 가지고 해당 nChannel의 상태를 Bit연산으로 추출하는 코드 구현
	return false; // 자리 표시자 반환값
}
bool CMmceIo::in(int nChannel)
{
	// 전체 입력 byte를 가지고 해당 nChannel의 상태를 Bit연산으로 추출하는 코드 구현
	return false; // 자리 표시자 반환값
}

bool CMmceIo::sequence()
{
	// 전체 출력 bytes를 MMCE 하드웨어에서 설정하는 코드 구현
	// 전체 입력 bytes에 MMCE 하드웨어에서 읽어오는 코드 구현
	return 0; 
};

