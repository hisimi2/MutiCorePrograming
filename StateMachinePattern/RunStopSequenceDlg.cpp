// RunStopSequenceDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "RunStopSequence.h"
#include "RunStopSequenceDlg.h"
#include "afxdialogex.h"


#include "CMmceIo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 사용자 정의 메시지
#define WM_UPDATE_ACTION_LIST (WM_USER + 1)

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

CRunStopSequenceDlg::CRunStopSequenceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RUNSTOPSEQUENCE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    
    // 생성자에서는 아무것도 하지 않거나, Context를 여기서 생성할 수도 있음
}

CRunStopSequenceDlg::~CRunStopSequenceDlg()
{
    // AppContext 소멸자가 호출되면서 스케줄러 정지 및 자원 해제가 자동으로 수행됨
}

void CRunStopSequenceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ACTION, m_ActionList);
}

BEGIN_MESSAGE_MAP(CRunStopSequenceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RUN, &CRunStopSequenceDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_STOP, &CRunStopSequenceDlg::OnBnClickedStop)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_UPDATE_ACTION_LIST, &CRunStopSequenceDlg::OnUpdateActionList)
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

	// 1. AppContext 생성 및 초기화 (모든 시스템 객체가 여기서 생성됨)
    m_pContext = std::make_unique<AppContext>();

    // 2. 필요한 객체 참조 가져오기
    auto pRobot = m_pContext->GetRobot();
	auto pStartSwitch = m_pContext->GetStartSwitch();

	// 3. Observer 등록 (UI 업데이트를 위해)
    // IOPSwitch 인터페이스에는 attach가 없으므로 dynamic_cast 사용 (또는 IOPSwitch 수정 후 직접 호출)
	if (auto pSubject = dynamic_cast<CSubject*>(pStartSwitch.get()))
	{
		pSubject->attach(this);
	}
	pRobot->attach(this);

	// 4. 스케줄러 시작
    m_pContext->StartScheduler();

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

// IObserver의 update 메서드 구현
void CRunStopSequenceDlg::update(string notification)
{
	// 프로그램 종료 시 윈도우가 파괴된 후 스레드에서 호출될 경우를 대비하여 핸들 확인
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	// 다른 스레드에서 호출되므로, UI 스레드로 메시지를 보내 안전하게 처리
	// new를 사용하여 문자열을 힙에 복사하고 포인터를 전달
	char* msg = new char[notification.length() + 1];
	strcpy_s(msg, notification.length() + 1, notification.c_str());
	
	// PostMessage 실패 시(예: 윈도우 소멸 직후) 메모리 해제하여 누수 방지
	if (!PostMessage(WM_UPDATE_ACTION_LIST, (WPARAM)msg))
	{
		delete[] msg;
	}
}

// 사용자 메시지 핸들러 구현
afx_msg LRESULT CRunStopSequenceDlg::OnUpdateActionList(WPARAM wParam, LPARAM lParam)
{
	// 전달받은 문자열 포인터를 CString으로 변환
	char* msg = (char*)wParam;
	CString str(msg);
	
	// 리스트박스에 추가
	int nIndex = m_ActionList.AddString(str);

	// 마지막으로 추가된 항목이 보이도록 스크롤합니다.
	if (nIndex >= 0)
	{
		m_ActionList.SetTopIndex(nIndex);
	}

	// 동적으로 할당된 메모리 해제
	delete[] msg;

	return 0;
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

void CRunStopSequenceDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
}

void CRunStopSequenceDlg::OnBnClickedRun()
{
	auto pStartSwitch = m_pContext->GetStartSwitch();
	if (pStartSwitch)
    {
		pStartSwitch->setSwitchStatus(true);
    }
}

void CRunStopSequenceDlg::OnBnClickedStop()
{
	auto pStartSwitch = m_pContext->GetStartSwitch();
	if (pStartSwitch)
    {
		pStartSwitch->setSwitchStatus(false);
    }
}
