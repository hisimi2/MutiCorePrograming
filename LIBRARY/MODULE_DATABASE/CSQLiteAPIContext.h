#pragma once

#include "CDBContext.h"

class CSQLiteAPIContext :public CDBContext
{
	CString m_strFile;

public:
	CSQLiteAPIContext(CString strFile);
	int executeSQL(CString queryW) override;
};