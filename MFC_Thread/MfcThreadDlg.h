#pragma once
#include "CRobot.h"
#include "COPSwitch.h"
#include "CRunStopControl.h"
#include "afxwin.h"
#include "IObserver.h" // IObserver 인터페이스 헤더 추가

// 사용자 지정 메시지를 정의합니다.
#define WM_UPDATE_ACTION_LIST (WM_USER + 1)

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
	// 메시지 핸들러를 선언합니다.
	afx_msg LRESULT OnUpdateActionList(WPARAM wParam, LPARAM lParam);
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
	void update(string notification) override
	{
		// UI 스레드로 메시지를 보내기 위해 CString을 동적으로 할당합니다.
		CString* pStr = new CString(notification.c_str());
		PostMessage(WM_UPDATE_ACTION_LIST, 0, (LPARAM)pStr);
	}
};
