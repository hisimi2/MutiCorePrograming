#pragma once
#include "CAbsThread.h"
class CMmceIo:public CAbsThread
{
	bool sequence() override;

public:
	CMmceIo();
	~CMmceIo() override;
	void out(int nChannel, bool bStatus);
	bool out(int nChannel);
	bool in(int nChannel);
};

