#pragma once

#include <set>
#include "IObserver.h"
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <ctime>

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
		// 현재 시간을 가져옵니다.
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);

		std::tm tm_buf;
		// 스레드 안전을 위해 localtime_s (Windows) 또는 localtime_r (POSIX)을 사용합니다.
#ifdef _WIN32
		localtime_s(&tm_buf, &in_time_t);
#else
		localtime_r(&in_time_t, &tm_buf);
#endif

		std::stringstream ss;
		ss << std::put_time(&tm_buf, "%H:%M:%S");
		std::string timestamp = ss.str();

		// 타임스탬프와 알림 메시지를 결합합니다.
		std::string message_with_timestamp = "[" + timestamp + "] " + notification;

		for (auto iter = m_Observers.begin(); iter != m_Observers.end(); ++iter)
		{
			IObserver* pObserver = *iter;
			if (pObserver != nullptr)
			{
				pObserver->update(message_with_timestamp);
			}
		}
	}

protected:
	set<IObserver*> m_Observers;
};

