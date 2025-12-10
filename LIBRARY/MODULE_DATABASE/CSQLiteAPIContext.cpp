#include "stdafx.h"
#include "CSQLiteAPIContext.h"

#include "sqlite3.h"
#pragma comment(lib,"sqlite3.lib")

CSQLiteAPIContext::CSQLiteAPIContext(CString strFile)
		:CDBContext(L"SQLite3 API Library"), m_strFile(strFile) 
{
}

int CSQLiteAPIContext::executeSQL(CString queryW) 
{
	CStringA queryA(queryW);

	sqlite3*		pDB;
	sqlite3_stmt*	pStmt;

	if (SQLITE_OK != sqlite3_open(CStringA(m_strFile).GetBuffer(), &pDB))
	{
		sqlite3_close(pDB);
		return FALSE;
	}

	if (SQLITE_OK == sqlite3_prepare_v2(pDB, queryA.GetBuffer(), -1, &pStmt, NULL))
	{
		queryA.MakeLower();

		m_records.clear();
		while (SQLITE_DONE != sqlite3_step(pStmt))
		{
			int nColums = sqlite3_column_count(pStmt);
			vector<CString> strRecord;
			for (auto nCol = 0;nCol < nColums;nCol++)
			{
				CString strCol;
				strCol = sqlite3_column_text(pStmt, nCol);
				strRecord.push_back(strCol);
			}
			m_records.push_back(strRecord);
		}
		sqlite3_finalize(pStmt);
	}
	sqlite3_close(pDB);

	return TRUE;
}
