#pragma once
#include "CAbsThread.h"
#include "CTimer.h"
#include <vector>
#include <initializer_list>

class COPSwitch : public CAbsThread
{
public:
	enum class EType { KEEP, PUSH, TOGGLE };

	COPSwitch(EType type = EType::PUSH, bool isBlink = false);
	~COPSwitch();

	void setInput(std::initializer_list<int> inputs);
	void setOutput(std::initializer_list<int> outputs);
	void setGroup(COPSwitch* pObject);
	void setStatus(bool bStatus);
	void setBlink(bool bStatus);
	void setLED(bool bStatus);
	bool checkInSensor();
	bool getStatus() const;

protected:
	int sequence() override;

private:
	CTimer m_BlinkTimer;
	std::vector<int> m_inputs;
	std::vector<int> m_outputs;

	bool m_status = false;
	bool m_toggleFlag = false;
	bool m_blinkStatus = false;
	EType m_type;
	bool m_isBlink = false;
	COPSwitch* m_pGroup = nullptr;
};

