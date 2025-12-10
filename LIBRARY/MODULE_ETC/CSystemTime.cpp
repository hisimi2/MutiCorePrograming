#include "pch.h"
#include "CSystemTime.h"


CString CSystemTime::getTime()
{
	SYSTEMTIME time;
	::GetLocalTime(&time);

	return getTime(time);
}

CString CSystemTime::getDate()
{
	SYSTEMTIME time;
	::GetLocalTime(&time);

	return getDate(time);
}

CString CSystemTime::getTime(SYSTEMTIME& time)
{
	CString strTime;
	strTime.Format(L"%02d:%02d:%02d", time.wHour, time.wMinute, time.wSecond);

	return strTime;
}

CString CSystemTime::getDate(SYSTEMTIME& time)
{
	CString strDate;
	strDate.Format(L"%04d-%02d-%02d", time.wYear, time.wMonth, time.wDay);

	return strDate;
}
