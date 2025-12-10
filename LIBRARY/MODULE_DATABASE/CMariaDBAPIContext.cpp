#include "stdafx.h"
#include "CMariaDBAPIContext.h"
#include "mysql.h"

#pragma comment(lib,"libmariadb.lib")

CMariaDBAPIContext::CMariaDBAPIContext(	CString strHost, CString strUserID,
										CString strPassword, CString strDBName, int nPort)
										: CDBContext(L"MariaDB API Library")
{
	m_strHost = CStringA(strHost);
	m_strUserID = CStringA(strUserID);
	m_strPassword = CStringA(strPassword);
	m_strDBName = CStringA(strDBName);
	m_nPort = nPort;
}

int CMariaDBAPIContext::executeSQL(CString queryW)
{
	MYSQL mysql;
	mysql_init(&mysql);

	if (mysql_real_connect(&mysql,
		m_strHost.GetBuffer(),
		m_strUserID.GetBuffer(),
		m_strPassword.GetBuffer(),
		m_strDBName.GetBuffer(),
		m_nPort, NULL, 0) == NULL)
	{
		return FALSE;
	}

	CStringA queryA = CStringA(queryW);
	mysql_query(&mysql, queryA);
	queryA.MakeLower();
	if (0 <= queryA.Find("select"))
	{
		MYSQL_RES* resultSQL = mysql_store_result(&mysql);
		if (NULL == resultSQL)
		{
			return FALSE;
		}

		m_records.clear();

		MYSQL_ROW row = mysql_fetch_row(resultSQL);
		while (row)
		{
			int nColums = mysql_num_fields(resultSQL);

			vector<CString> strRecord;
			for (auto nCol = 0; nCol < nColums; nCol++)
			{
				strRecord.push_back(CString(row[nCol]));
			}
			m_records.push_back(strRecord);

			row = mysql_fetch_row(resultSQL);
		}

		mysql_free_result(resultSQL);
	}

	mysql_close(&mysql);

	return TRUE;
}


