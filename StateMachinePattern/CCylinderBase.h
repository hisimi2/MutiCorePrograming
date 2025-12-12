#pragma once
#include <string>
#include <vector>
#include <cstdarg>
#include "CSubject.h"

#define INVALID 0xFFFFFFFF
#define MANUAL true

// 실린더의 공통 기능을 담는 기반 클래스
class CCylinderBase : public CSubject
{
	vector<UINT> m_ASensor, m_BSensor;
	double		m_dTimer;
	UINT		m_uDelay = 1000;
	string		m_strName;
	UINT		m_nOutA, m_nOutB;

public:

	CCylinderBase(string strName, UINT OutA = INVALID, UINT OutB = INVALID)
		: m_strName(strName)
		, m_nOutA(OutA)
		, m_nOutB(OutB)
	{
	};
	virtual ~CCylinderBase() {};
	void		setSensorPortA(UINT uCount, va_list& list);
	void		setSensorPortB(UINT uCount, va_list& list);
	bool		isActA();
	bool		isActB();
	void		actAAsync();
	void		actBAsync();
	int			actA(BOOL bManual = MANUAL);
	int			actB(BOOL bManual = MANUAL);


	void		setName(string& strData);
	void		setDelay(double dTimer = 3.0, UINT uDelay = 1);
};

