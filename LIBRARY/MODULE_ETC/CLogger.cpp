#include "pch.h"
#include "CLogger.h"
#include "CLocalSystem.h"
using namespace std;

void CLogger::Write(CString strMessage)
{
	ofstream File;
	File.open(CLocalSystem::getDate() + ".log", ios::app);

	if (File.is_open())
	{
		CString strLogW;
		strLogW.Format(L"[%s] %s \n", CLocalSystem::getIP(), strMessage);

		CStringA strLogA(strLogW);
		File.write(strLogA, strLogA.GetLength());
		
		File.close();
	}
}
