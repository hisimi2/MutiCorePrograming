#pragma once
class CLocalSystem
{
public:
    // L"%02d:%02d:%02d" 형식의 시간
	static CString getTime();
	static CString getTime(SYSTEMTIME& systemtime);

	//L"%04d-%02d-%02d" 형식의 날짜
	static CString getDate();
	static CString getDate(SYSTEMTIME& systemtime);

	static CString getIP();
};

#include <iostream>
using namespace std;

template <typename T>
CString changeString(const T tValue)
{
	CString strFormat;
			
	if (typeid(tValue) == typeid(int))
	{
		strFormat = L"%d";
	}
	else if (typeid(tValue) == typeid(double))
	{
		strFormat = L"%f";
	}

	CString strReturn;
	strReturn.Format(strFormat, tValue);

	return strReturn;
}