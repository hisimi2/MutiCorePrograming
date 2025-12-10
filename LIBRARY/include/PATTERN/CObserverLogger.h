#pragma once

#include "CLogger.h"
#include "IObserver.h"
class CObserverLogger :  public IObserver
{
	CLogger m_logger;
public:
	void update(string notification) override
	{
		m_logger.Write(notification);
	}
};