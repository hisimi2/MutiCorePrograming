#include "pch.h"
#include "CLocalSystem.h"

#include "winsock2.h"
#pragma comment(lib, "WS2_32.lib")

CString CLocalSystem::getIP()
{
	WSADATA wsadata;
	CString strIP(L"");

	if (!::WSAStartup(MAKEWORD(0x02, 0x02), &wsadata))
	{
		HOSTENT *p_host_info;
		IN_ADDR in;
		char host_name[128] = { 0, };

		gethostname(host_name, 128);
		p_host_info = gethostbyname(host_name);

		if (p_host_info != NULL)
		{
			for (int i = 0; p_host_info->h_addr_list[i]; i++)
			{
				memcpy(&in, p_host_info->h_addr_list[i], 4);
				strIP = inet_ntoa(in);
			}
		}

		WSACleanup();
	}
	return strIP;
}


CString CLocalSystem::getTime()
{
	SYSTEMTIME time;
	::GetLocalTime(&time);

	return getTime(time);
}

CString CLocalSystem::getDate()
{
	SYSTEMTIME time;
	::GetLocalTime(&time);

	return getDate(time);
}

CString CLocalSystem::getTime(SYSTEMTIME& time)
{
	CString strTime;
	strTime.Format(L"%02d:%02d:%02d", time.wHour, time.wMinute, time.wSecond);

	return strTime;
}

CString CLocalSystem::getDate(SYSTEMTIME& time)
{
	CString strDate;
	strDate.Format(L"%04d-%02d-%02d", time.wYear, time.wMonth, time.wDay);
	return strDate;
}















