#pragma once
#include "IDio.h"
#include "CAbsThread.h"

class CMmceIo : public IDio, public CAbsThread
{
	bool sequence() override;

public:
	CMmceIo();
	~CMmceIo() override;
	void out(int nChannel, bool bStatus) override;
	bool out(int nChannel) override;
	bool in(int nChannel) override;


};

