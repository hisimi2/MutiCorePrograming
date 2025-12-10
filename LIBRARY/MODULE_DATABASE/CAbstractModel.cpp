#include "stdafx.h"
#include "CAbstractModel.h"

CAbstractModel::CAbstractModel(upCONTEXT& upDB)
{
	m_upContext = move(upDB);
}

CString CAbstractModel::getLibraryType()
{
	return m_upContext->getLibraryType();
}

int CAbstractModel::getRowCount()
{
	return m_upContext->getRowCount();
}

int CAbstractModel::getColCount()
{
	return m_upContext->getColCount();
}

void CAbstractModel::executeSQL(CString query)
{
	m_upContext->executeSQL(query);
}

