#pragma once
#include "afxdialogex.h"
#include "CSubject.h"

// CDialogSubject 대화 상자
class CDialogSubject : public CDialogEx, public CSubject
{
	DECLARE_DYNAMIC(CDialogSubject)
public:
	CDialogSubject(CWnd* pParent = nullptr) {}
	virtual ~CDialogSubject() {}
};
