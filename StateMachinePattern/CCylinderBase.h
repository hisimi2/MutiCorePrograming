#pragma once
#include "CSubject.h"
#include "ICylinder.h"
#include <string>
#include <vector>

#define INVALID 0xFFFFFFFF
#define MANUAL true

// ICylinder를 가상으로 상속하도록 수정
class CCylinderBase : public virtual ICylinder, public CSubject
{
protected:
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
	virtual int			actA(bool bManual = false) override;
	virtual int			actB(bool bManual = false) override;
	virtual bool		isActA() override;
	virtual bool		isActB() override;

	void		setName(string& strData);
	void		setDelay(double dTimer = 3.0, UINT uDelay = 1);
};

