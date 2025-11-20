#pragma once
#include "IDio.h"
class CMmceIo : public IDio
{
	public:
	CMmceIo();
	~CMmceIo() override;
	void out(int nChannel, bool bStatus) override;
	bool out(int nChannel) override;
	bool in(int nChannel) override;

};

