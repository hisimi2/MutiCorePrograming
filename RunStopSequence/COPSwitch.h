#pragma once
#include "../Framework/CAbsThread.h"
#include "../Framework/IOPSwitch.h"
#include "CTimer.h"
#include <vector>

class COPSwitch : public CAbsThread, public IOPSwitch
{
public:
	enum _eTYPE { eKEEP = 0, ePUSH, eTOGGLE };
	COPSwitch(_eTYPE nType = ePUSH, BOOL bBLink = false) :
		m_nType(nType), m_bBlink(bBLink), m_BlinkTimer(0.5)
	{
	};
	~COPSwitch();

	BOOL getStatus();
	void setStatus(BOOL bStatus);

	void setInput(int nCount, int ...);
	void setOutput(int nCount, int ...);
	void setGroup(COPSwitch* pObject);
	void setBlink(BOOL bStatus);
	void setLED(BOOL bStatus);
	BOOL checkInSensor();

	CTimer		m_BlinkTimer;
private:
	int* m_pnIns = nullptr;
	int* m_pnOuts = nullptr;
	int			m_nInputCount = 0;
	int			m_nOutputCount = 0;
	BOOL		m_bStatus = false;
	BOOL		m_bToggleFlag = false;
	BOOL		m_bBlinkStatus = false;
	_eTYPE		m_nType;
	BOOL		m_bBlink = false;
	COPSwitch* m_pGroup;

protected:
	int sequence();
};

