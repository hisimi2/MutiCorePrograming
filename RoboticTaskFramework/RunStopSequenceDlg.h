// RunStopSequenceDlg.h : 헤더 파일
//

#pragma once
#include "CRobot.h"
#include "CMmceIo.h"
#include "COPSwitch.h"

// CRunStopSequenceDlg 대화 상자
class CRunStopSequenceDlg : public CDialogEx
{
	CMmceIo			m_io;          // DIO 객체
	COPSwitch		m_StartSwitch; // 로봇의 내부 로직용 스위치
	CRobot 			m_Robot;          // 로봇 객체 (이제 스레드를 포함)

	
// 생성입니다.
public:
	CRunStopSequenceDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RUNSTOPSEQUENCE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedStop();
	afx_msg void OnDestroy(); // OnDestroy 핸들러 추가
	// UI 타이머 이벤트 핸들러
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
