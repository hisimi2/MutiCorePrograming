#include "stdafx.h"
#include "CODBCClassContext.h"

#include <afxdb.h>

CODBCClassContext::CODBCClassContext(CString strConnecting)
		:CDBContext(L"ODBC Class Library"), m_strConnecting(strConnecting) 
{
}

int CODBCClassContext::executeSQL(CString query) 
{
	try
	{
		CDatabase db;
		db.OpenEx(m_strConnecting);

		// 소문자로 변경한 것으로 select문 판별
		CString strCompare(query);
		strCompare.MakeLower();

		if (0 > strCompare.Find(L"select"))
		{
			db.ExecuteSQL(query);
		}
		else
		{
			m_records.clear();

			CRecordset rs(&db);
			rs.Open(AFX_DB_USE_DEFAULT_TYPE, query);

			short nCols = rs.GetODBCFieldCount();

			while (!rs.IsEOF())
			{
				vector<CString> strRecord;
				for (int nCol = 0; nCol < nCols; nCol++)
				{
					CString  strField;
					rs.GetFieldValue(nCol, strField);
					strRecord.push_back(strField);
				}
				m_records.push_back(strRecord);
				rs.MoveNext();
			}
			rs.Close();
		}

		db.Close();
	}
	catch (CDBException* xODBCexception)
	{
		xODBCexception->ReportError();
		return FALSE;
	}

	return TRUE;
}
