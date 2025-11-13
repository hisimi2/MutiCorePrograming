#pragma once
#include <string>
#include <vector>
using namespace std;

#define MANUAL			(TRUE)
#define INVALID			(-1)


class CCylinder
{
	vector<UINT> m_ASensor, m_BSensor;

public:

	CCylinder(UINT OutA = INVALID, UINT OutB = INVALID) :
		m_nOutA(OutA), m_nOutB(OutB)
	{
	};
	~CCylinder();
	void		setSensorPortA(UINT uCount, va_list& list);
	void		setSensorPortB(UINT uCount, va_list& list);
	BOOL		isUp();
	BOOL		isDown();
	void		upAsync();
	void		downAsync();
	int			up(BOOL bManual = MANUAL);
	int			down(BOOL bManual = MANUAL);

	BOOL		isClamp() { return isUp(); }
	BOOL		isUnclamp() { return isDown(); }
	void		clampAsync() { upAsync(); }
	void		unclampAsync() { downAsync(); }
	int			clamp(BOOL bManual = MANUAL) { return up(bManual); }
	int			unclamp(BOOL bManual = MANUAL) { return down(bManual); }

	void		setName(string& strData);

	void		setDelay(double dTimer = 3.0, UINT uDelay = 1);
	string		m_strName;
	UINT		m_nOutA, m_nOutB;
	
private:
	double		m_dTimer;
	UINT		m_uDelay;
};

