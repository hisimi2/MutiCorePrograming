#pragma once

#include <set>
#include "IObserver.h"

class CSubject
{
public:
	virtual ~CSubject() {}
	void attach(IObserver* pObserver)
	{
		m_Observers.insert(pObserver);
	}

	void detach(IObserver* pObserver)
	{
		m_Observers.erase(m_Observers.find(pObserver));
	}

	void notify(string notification) 
	{
		for (auto iter = m_Observers.begin(); iter != m_Observers.end(); ++iter)
		{
			IObserver* pObserver = *iter;
			if (pObserver != nullptr)
			{
				pObserver->update(notification);
			}
		}
	}

protected:
	set<IObserver*> m_Observers;
};

