#pragma once
#include <string>
#include <vector>
#include "CSubject.h"


#define MANUAL			(TRUE)
#define INVALID			(-1)


class CCylinder : public CSubject
{
	vector<UINT> m_ASensor, m_BSensor;
	double		m_dTimer;
	UINT		m_uDelay=1000;
	string		m_strName;
	UINT		m_nOutA, m_nOutB;

public:

	CCylinder(string strName, UINT OutA = INVALID, UINT OutB = INVALID) 
		: m_strName(strName)
		, m_nOutA(OutA)
		, m_nOutB(OutB)
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
	int			clamp(BOOL bManual = MANUAL);
	int			unclamp(BOOL bManual = MANUAL);

	void		setName(string& strData);
	void		setDelay(double dTimer = 3.0, UINT uDelay = 1);
};

