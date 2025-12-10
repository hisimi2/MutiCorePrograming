#pragma once

#include "CDBContext.h"

class CODBCAPIContext :public CDBContext
{
	CString		m_strDSN;
	CString		m_strID;
	CString		m_strPassword;

public:
	CODBCAPIContext(CString strDSN, CString strID = L"", CString strPassword = L"");
	int executeSQL(CString query) override;
};