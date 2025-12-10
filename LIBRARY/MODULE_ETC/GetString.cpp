#include "pch.h"
#include "GetString.h"

CString String(int nValue)
{
	CString strValue;
	strValue.Format(L"%d", nValue);
	return strValue;
}

CString String(unsigned int uValue)
{
	CString strValue;
	strValue.Format(L"%d", uValue);
	return strValue;
}

CString String(double dValue)
{
	CString strValue;
	strValue.Format(L"%10.2f", dValue);
	return strValue;
}
