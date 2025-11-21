// RunStopSequenceDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "RunStopSequence.h"
#include "RunStopSequenceDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRunStopSequenceDlg 대화 상자



void CRunStopSequenceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRunStopSequenceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RUN, &CRunStopSequenceDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_STOP, &CRunStopSequenceDlg::OnBnClickedStop)
	ON_WM_DESTROY()
	// ON_WM_TIMER()  // 더 이상 외부 타이머로 sequence() 호출하지 않음
END_MESSAGE_MAP()


// CRunStopSequenceDlg 메시지 처리기

BOOL CRunStopSequenceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// 각 객체의 백그라운드 스레드를 비동기적으로 시작합니다.
	// 이 함수들은 즉시 반환되므로 UI 스레드를 막지 않습니다.
	m_Robot.resume();     // CRobot 스레드 시작

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CRunStopSequenceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{

		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CRunStopSequenceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CRunStopSequenceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CRunStopSequenceDlg::CRunStopSequenceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RUNSTOPSEQUENCE_DIALOG, pParent)
	, m_StartSwitch(m_io) // m_StartSwitch 초기화
	, m_Robot(m_StartSwitch) // m_Robot은 m_StartSwitch를 참조하여 생성
{
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRunStopSequenceDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// 다이얼로그 종료 시 모든 스레드에 종료 신호를 보냅니다.
	// 각 객체의 소멸자가 스레드가 완전히 끝날 때까지 안전하게 기다립니다.
	m_io.setEnd();
	m_StartSwitch.setEnd();
	m_Robot.setEnd();
}

void CRunStopSequenceDlg::OnBnClickedRun()
{
	// 스위치 상태만 변경. sequence()는 COPSwitch 내부 스레드가 주기적으로 실행함.
	m_StartSwitch.setSwitchStatus(true);
}

void CRunStopSequenceDlg::OnBnClickedStop()
{
	// 스위치 상태만 변경.
	m_StartSwitch.setSwitchStatus(false);
}
