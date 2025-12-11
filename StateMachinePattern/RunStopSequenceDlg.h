// RunStopSequenceDlg.h : 헤더 파일
//

#pragma once
#include "ctpl_stl.h" // CTPL 헤더
#include "Scheduler.h"
#include "CMmceIo.h"
#include "COPSwitch.h"
#include "CRobot.h" // CRobot 헤더 추가
#include <memory>
#include <vector>

// CRunStopSequenceDlg 대화 상자
class CRunStopSequenceDlg : public CDialogEx
{
// 생성입니다.
public:
	CRunStopSequenceDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	~CRunStopSequenceDlg(); // 소멸자 추가

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
	// --- 스레드 풀 아키텍처 멤버 ---
	std::unique_ptr<ctpl::thread_pool> m_pThreadPool;
	std::unique_ptr<Scheduler> m_pScheduler;

	// --- 작업 객체들 ---
	CMmceIo m_mmceIo;
	COPSwitch m_StartSwitch;
	CRobot m_Robot;
	std::vector<std::unique_ptr<COPSwitch>> m_switches; // 복사 불가능하므로 unique_ptr 사용
public:
	afx_msg void OnBnClickedStop2();
	afx_msg void OnBnClickedLoadRecipe();
};
