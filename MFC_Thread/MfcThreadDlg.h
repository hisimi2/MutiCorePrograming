#pragma once
#include "CRobot.h"
#include "COPSwitch.h"
#include "CRunStopControl.h"
#include "afxwin.h"
#include "IObserver.h" // IObserver 인터페이스 헤더 추가


class MfcThreadDlg : public CDialogEx, public IObserver // IObserver 상속 추가
{
public:
	MfcThreadDlg(CWnd* pParent = NULL);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_THREAD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedStop();

private:
	COPSwitch m_StartSwitch;
	CRobot m_robot;
	CRunStopControl m_MainThread;

public:
	CListBox m_ActionList;

	// IObserver 인터페이스 구현
	virtual void update(std::string notification) override
	{
		// TODO: 옵저버 업데이트 시 필요한 동작 구현
	}
};
