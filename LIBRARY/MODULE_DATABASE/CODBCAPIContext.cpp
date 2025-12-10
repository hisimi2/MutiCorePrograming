#include "stdafx.h"
#include "CODBCAPIContext.h"

#include <sqlext.h>
#pragma comment(lib,"odbc32.lib")

CODBCAPIContext::CODBCAPIContext(CString strDSN, CString strID, CString strPassword)
				:CDBContext(L"ODBC API Library"), 
				 m_strDSN(strDSN), m_strID(strID), 
				 m_strPassword(strPassword) 
{
}

int CODBCAPIContext::executeSQL(CString query)
{
	SQLHANDLE	hEnvironment;	// ODBC를 사용하기 위한 환경 정보
	SQLHDBC		hODBC;		// ODBC 함수를 사용하기 위한 정보
	HSTMT		hStatement;


	SQLRETURN sqlReturn;

	sqlReturn = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnvironment);

	if (SQL_SUCCESS != sqlReturn)
	{
		TRACE("Alloc handle error: SQL_HANDLE_ENV \n");
		return FALSE;
	}

	sqlReturn = SQLSetEnvAttr(hEnvironment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
	if (SQL_SUCCESS != sqlReturn)
	{
		TRACE("Set Attribute error: SQL_HANDLE_ENV \n");
		return FALSE;
	}

	sqlReturn = SQLAllocHandle(SQL_HANDLE_DBC, hEnvironment, &hODBC);
	if (SQL_SUCCESS != sqlReturn)
	{
		TRACE("Alloc handle error : SQL_HANDLE_DBC \n");
		return FALSE;
	}

	SQLSetConnectAttr(hODBC, 5, (SQLPOINTER)SQL_LOGIN_TIMEOUT, 0);

	sqlReturn = SQLConnect(hODBC, m_strDSN.GetBuffer(), SQL_NTS,
		m_strID.GetBuffer(), SQL_NTS,
		m_strPassword.GetBuffer(), SQL_NTS);
	if (SQL_SUCCESS != sqlReturn)
	{
		TRACE("DB connection error. \n");
		return FALSE;
	}

	SQLAllocHandle(SQL_HANDLE_STMT, hODBC, &hStatement);

	if (SQL_SUCCESS == SQLExecDirect(hStatement, query.GetBuffer(), SQL_NTS))
	{
		// 소문자로 변경하여 select문 판별
		CString strCompare(query);
		strCompare.MakeLower();

		if (0 <= strCompare.Find(L"select"))
		{
			m_records.clear();

			while (SQL_SUCCESS == SQLFetch(hStatement))
			{
				vector<CString> strRecord;

				int nCol = 1;
				while (1)
				{
					SQLCHAR szBuf[255] = { 0 };
					if (SQL_SUCCESS != SQLGetData(hStatement, nCol, SQL_CHAR, szBuf, sizeof(szBuf), NULL))
					{
						break;
					}
					strRecord.push_back(CString(szBuf));
					++nCol;
				}
				m_records.push_back(strRecord);
			}
		}
	}

	SQLFreeHandle(SQL_HANDLE_STMT,	hStatement);
	SQLFreeHandle(SQL_HANDLE_DBC,	hODBC);
	SQLFreeHandle(SQL_HANDLE_ENV,	hEnvironment);
	return TRUE;
}
