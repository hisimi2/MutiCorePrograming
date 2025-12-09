// thread_poolDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "thread_pool.h"
#include "thread_poolDlg.h"
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
	DECLARE_MESSAGE_MAP()
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


// Cthread_poolDlg 대화 상자



Cthread_poolDlg::Cthread_poolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_THREAD_POOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cthread_poolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cthread_poolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_THREAD, &Cthread_poolDlg::OnBnClickedThread)
	ON_BN_CLICKED(IDC_POOL, &Cthread_poolDlg::OnBnClickedPool)
END_MESSAGE_MAP()


// Cthread_poolDlg 메시지 처리기

BOOL Cthread_poolDlg::OnInitDialog()
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void Cthread_poolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void Cthread_poolDlg::OnPaint()
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

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR Cthread_poolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 스레드에서 실행될 함수
void task(int id) {
	std::cout << "작업 " << id << " 시작" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "작업 " << id << " 종료" << std::endl;
}

void Cthread_poolDlg::OnBnClickedThread()
{
	// task 함수를 실행하는 스레드 2개 생성
	std::thread t1(task, 1);
	std::thread t2(task, 2);

	// main 스레드는 t1과 t2가 끝날 때까지 기다립니다.
	t1.join();
	t2.join();

	std::cout << "모든 작업 완료" << std::endl;
}


// 간단한 작업 함수 (결과값 없음)
void simple_task(int id) {
	std::cout << "단순 작업 " << id << " 시작 (스레드 ID: " << std::this_thread::get_id() << ")" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	std::cout << "단순 작업 " << id << " 종료" << std::endl;
}

// 결과값을 반환하는 작업 함수
int task_with_return(int id) {
	std::cout << "반환 작업 " << id << " 시작 (스레드 ID: " << std::this_thread::get_id() << ")" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "반환 작업 " << id << " 종료" << std::endl;
	return id * id;
}

void Cthread_poolDlg::OnBnClickedPool()
{
	// 시스템의 하드웨어 스레드(코어) 수를 가져옵니다.
	// 이 값은 프로그램이 실행되는 PC의 CPU 사양에 따라 달라집니다.
	const unsigned int num_threads = std::thread::hardware_concurrency();

	// 만약 hardware_concurrency()가 0을 반환하는 경우(코어 수를 알 수 없는 경우)를 대비해
	// 최소 1개의 스레드는 보장하도록 합니다.
	const unsigned int pool_size = (num_threads > 0) ? num_threads : 1;

	// 최적의 스레드 수로 스레드 풀 생성
	ThreadPool pool(pool_size);

	CString msg;
	msg.Format(_T("%d개의 스레드로 풀 생성됨"), pool_size);
	AfxMessageBox(msg);


	// 결과값이 없는 작업들을 큐에 추가
	for (int i = 0; i < 8; ++i) {
		pool.enqueue(simple_task, i);
	}

	// 결과값이 있는 작업들을 큐에 추가하고, future를 벡터에 저장
	std::vector<std::future<int>> results;
	for (int i = 0; i < 8; ++i) {
		results.emplace_back(pool.enqueue(task_with_return, i));
	}

	// future를 통해 작업 결과들을 받음
	// get() 함수는 작업이 완료될 때까지 대기합니다.
	for (auto&& result : results) {
		std::cout << "결과 수신: " << result.get() << std::endl;
	}

	std::cout << "모든 작업이 스레드 풀에 제출되었습니다." << std::endl;
	// main 함수가 종료되면 ThreadPool의 소멸자가 호출되어 모든 스레드가 안전하게 종료됩니다.
}
