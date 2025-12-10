#pragma once

#include "CDBContext.h"

#include <memory>
using namespace std;

typedef unique_ptr<CDBContext> upCONTEXT;

class CAbstractModel
{
public:
	CAbstractModel(upCONTEXT& upDB);
	virtual ~CAbstractModel() {}

	CString getLibraryType();
	int getRowCount();
	int getColCount();

protected:
	void executeSQL(CString query);
	upCONTEXT	m_upContext;
};



