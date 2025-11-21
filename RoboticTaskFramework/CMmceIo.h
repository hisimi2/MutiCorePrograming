#pragma once
#include "IDio.h"
#include "IPeriodicTask.h"

class CMmceIo : public IDio, public IPeriodicTask
{
	bool sequence() override;

public:
	CMmceIo();
	~CMmceIo() override;
	void out(int nChannel, bool bStatus) override;
	bool out(int nChannel) override;
	bool in(int nChannel) override;
};

