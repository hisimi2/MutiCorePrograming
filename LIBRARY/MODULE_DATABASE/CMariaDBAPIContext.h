#pragma once
#include "CDBContext.h"
class CMariaDBAPIContext :public CDBContext
{
	CStringA	m_strHost;
	CStringA	m_strUserID;
	CStringA	m_strPassword;
	CStringA	m_strDBName;
	int			m_nPort;

public:
	CMariaDBAPIContext(CString strHost, CString strUserID,CString strPassword, CString strDBName, int nPort);
	int executeSQL(CString queryW) override;
};