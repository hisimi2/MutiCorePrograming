#include "stdafx.h"
#include "CDBContext.h"

CDBContext::CDBContext(CString strLibraryType) 
	:m_strLibraryType(strLibraryType) 
{
}

CString CDBContext::getLibraryType()
{
	return m_strLibraryType;
}

int CDBContext::getRowCount()
{
	return m_records.size();
}

int CDBContext::getColCount()
{
	if (0 < getRowCount())
	{
		return m_records[0].size();
	}
	return 0;
}

CString CDBContext::GetFieldValue(int nRow, int nCol)
{
	if (isValid(nRow, nCol))
	{
		return m_records[nRow][nCol];
	}

	return CString();
}

BOOL CDBContext::isValid(int nRow, int nCol)
{
	if (0 > nRow || getRowCount() < nRow)
	{
		return FALSE;
	}

	if (0 > nCol || getColCount() < nCol)
	{
		return FALSE;
	}

	return TRUE;
}
