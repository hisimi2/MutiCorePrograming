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

	// unique_ptr 멤버 변수 초기화
	m_pMmceIo = std::make_unique<CMmceIo>();
	COPSwitch::setIo(m_pMmceIo.get()); // static Io 포인터 설정

	m_pStartSwitch = std::make_unique<COPSwitch>();
	m_pStartSwitch->setOption(IOPSwitch::EType::TOGGLE);

	// m_pRobot 멤버 변수 선언 및 초기화
	m_pRobot = std::make_unique<CRobot>(*m_pStartSwitch);
}

CRunStopSequenceDlg::~CRunStopSequenceDlg()
{
	// 스케줄러 중지 -> 스레드 풀 중지 순서를 보장하기 위해 명시적으로 reset 호출
	if (m_pScheduler)
	{
		m_pScheduler->stop();
	}
	// unique_ptr은 자동으로 메모리를 해제하므로 명시적인 delete 호출이 필요 없습니다.
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

	// --- Observer 등록 ---
	m_pStartSwitch->attach(this); // m_StartSwitch에 대한 Observer 등록
	m_pRobot->attach(this);

	// --- 스레드 풀 및 스케줄러 초기화 ---
	// 1. CTPL 스레드 풀 생성 (예: 하드웨어 코어 수만큼)
	unsigned int core_count = std::thread::hardware_concurrency();
	m_pThreadPool = std::make_unique<ctpl::thread_pool>(core_count > 0 ? core_count : 4);

	// 2. 스케줄러 생성 (예: 10ms 주기)
	m_pScheduler = std::make_unique<Scheduler>(*m_pThreadPool, std::chrono::milliseconds(10));

	// 스케줄러에 모든 작업 등록
	// 수정 코드: unique_ptr에서 shared_ptr로 변환하여 전달
	m_pScheduler->addTask(std::shared_ptr<IPeriodicTask>((COPSwitch*)m_pStartSwitch.get(), [](IPeriodicTask*) {}));
	m_pScheduler->addTask(std::shared_ptr<IPeriodicTask>(m_pRobot.get(), [](IPeriodicTask*) {}));

	// 3. 스케줄러 시작
	m_pScheduler->start();

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
	// 다른 스레드에서 호출되므로, UI 스레드로 메시지를 보내 안전하게 처리
	// new를 사용하여 문자열을 힙에 복사하고 포인터를 전달
	char* msg = new char[notification.length() + 1];
	strcpy_s(msg, notification.length() + 1, notification.c_str());
	PostMessage(WM_UPDATE_ACTION_LIST, (WPARAM)msg);
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
	// 스위치 상태만 변경. sequence()는 스케줄러가 주기적으로 실행함.
	m_pStartSwitch->setSwitchStatus(true);
}

void CRunStopSequenceDlg::OnBnClickedStop()
{
	// 스위치 상태만 변경.
	m_pStartSwitch->setSwitchStatus(false);
}

