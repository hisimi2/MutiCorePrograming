// RunStopSequenceDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include <vector>
#include <memory>
#include "ctpl_stl.h"
#include "CMmceIo.h"
#include "Scheduler.h"
#include "COPSwitch.h"
#include "CRobot.h"
#include "IObserver.h" // IObserver 인터페이스 포함

// CRunStopSequenceDlg 대화 상자
class CRunStopSequenceDlg : public CDialogEx, public IObserver
{
// 생성입니다.
public:
	CRunStopSequenceDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	virtual ~CRunStopSequenceDlg();

	// IObserver 인터페이스의 update 메서드 구현
	void update(string notification) override;

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
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedStop();
	DECLARE_MESSAGE_MAP()

private:
	// --- 비즈니스 로직 객체들 ---
	CMmceIo m_mmceIo;
	COPSwitch m_StartSwitch;
	CRobot m_Robot;
	std::vector<std::unique_ptr<COPSwitch>> m_switches;

	// --- 스케줄링 관련 멤버 ---
	std::unique_ptr<ctpl::thread_pool> m_pThreadPool;
	std::unique_ptr<Scheduler> m_pScheduler;

	CListBox m_ActionList;

	// UI 업데이트를 위한 사용자 메시지 핸들러
	afx_msg LRESULT OnUpdateActionList(WPARAM wParam, LPARAM lParam);
};
