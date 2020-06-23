
// VirtualMouseClickDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "VirtualMouseClick.h"
#include "VirtualMouseClickDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CVirtualMouseClickDlg 对话框



CVirtualMouseClickDlg::CVirtualMouseClickDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_VIRTUALMOUSECLICK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVirtualMouseClickDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVirtualMouseClickDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CVirtualMouseClickDlg::OnBnClickedOk)
END_MESSAGE_MAP()



BOOL SetTransparent(HWND hDlg, BOOL bTransparent)
{
	LONG lStyle;
	lStyle = GetWindowLong(hDlg, GWL_EXSTYLE);

	if (bTransparent)
	{
		SetWindowLong(hDlg, GWL_EXSTYLE, lStyle | WS_EX_TRANSPARENT | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hDlg, 0, 255, LWA_ALPHA); //设置半透明
		SetWindowPos(hDlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		return TRUE;
	}
	else
	{
		lStyle &= ~WS_EX_TRANSPARENT;
		SetWindowLong(hDlg, GWL_EXSTYLE, lStyle);
		SetLayeredWindowAttributes(hDlg, 0, 255, LWA_ALPHA);
		SetWindowPos(hDlg, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		return FALSE;
	}

}

// CVirtualMouseClickDlg 消息处理程序

BOOL CVirtualMouseClickDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	////加入WS_EX_LAYERED扩展属性
	//SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE,
	//	GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) ^ 0x80000);
	//HINSTANCE hInst = LoadLibrary(L"User32.DLL");
	//if (hInst)
	//{
	//	typedef BOOL(WINAPI* MYFUNC)(HWND, COLORREF, BYTE, DWORD);
	//	MYFUNC fun = NULL;
	//	//取得SetLayeredWindowAttributes函数指针
	//	fun = (MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
	//	if (fun)fun(this->GetSafeHwnd(), 0, 50, 2);
	//	FreeLibrary(hInst);
	//}

	SetTransparent(this->m_hWnd, TRUE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVirtualMouseClickDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVirtualMouseClickDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVirtualMouseClickDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


unsigned Counter;
unsigned __stdcall ClickVir(void* pArguments)
{
	printf("In second thread...\n");

	volatile bool bloop = true;
	int x = 100;
	while (bloop)
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN, 400, 400, 0, 0);
		Sleep(100);
		x += 100;
		mouse_event(MOUSEEVENTF_LEFTUP, 400, 400, 0, 0);

		if (x > 5000) bloop = false;
	}

	_endthreadex(0);
	return 0;
}

void CVirtualMouseClickDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialog::OnOK();
	HANDLE hThread;
	unsigned threadID;

	printf("Creating second thread...\n");

	// Create the second thread.
	hThread = (HANDLE)_beginthreadex(NULL, 0, &ClickVir, NULL, 0, &threadID);

	// Wait until second thread terminates. If you comment out the line
	// below, Counter will not be correct because the thread has not
	// terminated, and Counter most likely has not been incremented to
	// 1000000 yet.
	WaitForSingleObject(hThread, INFINITE);
	printf("Counter should be 1000000; it is-> %d\n", Counter);
	// Destroy the thread object.
	CloseHandle(hThread);
}
