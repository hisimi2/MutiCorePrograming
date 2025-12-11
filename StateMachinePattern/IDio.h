#pragma once
class IDio
{
	public:
		virtual ~IDio() = default;
		virtual void out(int nChannel, bool bStatus) = 0;
		virtual bool out(int nChannel) = 0;
		virtual bool in(int nChannel) = 0;

};

