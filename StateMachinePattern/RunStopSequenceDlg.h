// RunStopSequenceDlg.h : 헤더 파일
//

#pragma once

#include "AppContext.h" // AppContext 포함
#include "COPSwitch.h"
#include "CRobot.h"
#include "Scheduler.h"
#include "ctpl_stl.h"
#include <memory>
#include "IDio.h" // CMmceIo.h 대신 IDio.h를 포함합니다.

// CRunStopSequenceDlg 대화 상자
class CRunStopSequenceDlg : public CDialogEx, public IObserver
{
// 생성입니다.
public:
	CRunStopSequenceDlg(CWnd* pParent = NULL);
	virtual ~CRunStopSequenceDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RUNSTOPSEQUENCE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	HICON m_hIcon;

	// [수정] 개별 멤버 변수들을 AppContext로 대체
	std::unique_ptr<AppContext> m_pContext;

	// UI 제어를 위해 포인터만 임시로 보관 (소유권은 Context에 있음)
	std::shared_ptr<IOPSwitch> m_pStartSwitch; 

	CListBox m_ActionList;

	// 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedStop();
	afx_msg LRESULT OnUpdateActionList(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	// IObserver 인터페이스 구현
	void update(std::string notification) override;
};
