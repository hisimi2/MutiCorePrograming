#pragma once

#include <vector>
using namespace std;

class CDBContext
{
public:
	CDBContext(CString strLibraryType);
	virtual int executeSQL(CString query) = 0;

	CString getLibraryType();
	
	int getRowCount();
	int getColCount();
	CString GetFieldValue(int nRow, int nCol);

protected:
	BOOL isValid(int nRow, int nCol);
	CString					m_strLibraryType;
	vector<vector<CString>>	m_records;
};



