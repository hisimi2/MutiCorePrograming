#pragma once

#include "CDBContext.h"

class CODBCClassContext :public CDBContext
{
	CString m_strConnecting; // L"DSN=SQLite DB CRUD"
public:
	CODBCClassContext(CString strConnecting);

	int executeSQL(CString query) override;
};