#pragma once
#include "CSubject.h"
#include "ICylinder.h"
#include <string>
#include <vector>
#include <initializer_list> // std::initializer_list 사용을 위해 추가

// using 선언으로 코드 간결화
using std::string;
using std::vector;
using std::initializer_list;

// 매크로 대신 constexpr 상수 사용
constexpr unsigned int INVALID_PORT = 0xFFFFFFFF;


// ICylinder를 가상 상속하여 다이아몬드 문제 해결
class CCylinderBase : public virtual ICylinder, public CSubject
{
protected:
	vector<UINT> m_ASensor, m_BSensor;
	double		m_dTimer;
	UINT		m_uDelay = 1000;
	string		m_strName;
	UINT		m_nOutA, m_nOutB;

public:

	CCylinderBase(string strName, UINT OutA = INVALID_PORT, UINT OutB = INVALID_PORT)
		: m_strName(strName)
		, m_nOutA(OutA)
		, m_nOutB(OutB)
	{
	};

	virtual ~CCylinderBase() {};

	// std::initializer_list를 사용하여 타입-안전하게 센서 포트 설정
	void		setSensorPortA(initializer_list<UINT> list);
	void		setSensorPortB(initializer_list<UINT> list);

	// override 시 기본값은 선언부에만 둡니다. 구현부에서는 기본값을 제거해야 합니다.
	virtual int			actA(bool bManual = MANUAL_OPERATION) override;
	virtual int			actB(bool bManual = MANUAL_OPERATION) override;
	virtual bool		isActA() override;			
	virtual bool		isActB() override;

	void		setName(string& strData);
	void		setDelay(double dTimer = 3.0, UINT uDelay = 1);
};

