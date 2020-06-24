
// VirtualMouseClickDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "VirtualMouseClick.h"
#include "VirtualMouseClickDlg.h"
#include "afxdialogex.h"
#include <dwmapi.h>
#pragma comment (lib,"dwmapi.lib")

#include "Log.h"

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
public:
	virtual BOOL DestroyWindow();
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


volatile  bool CVirtualMouseClickDlg::g_bloop = true;
volatile  bool CVirtualMouseClickDlg::m_blbuttondown = false;
CVirtualMouseClickDlg::CVirtualMouseClickDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_VIRTUALMOUSECLICK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bTracking = false;
	m_hThread = NULL;
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
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()



BOOL SetTransparent(HWND hDlg, BOOL bTransparent)
{
	LONG lStyle;
	lStyle = GetWindowLong(hDlg, GWL_EXSTYLE);

	if (bTransparent)
	{
		SetWindowLong(hDlg, GWL_EXSTYLE, lStyle | WS_EX_TRANSPARENT | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hDlg, 0, 50, LWA_ALPHA); //设置半透明
		//SetWindowPos(hDlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		return TRUE;
	}
	else
	{
		lStyle &= ~WS_EX_TRANSPARENT;
		SetWindowLong(hDlg, GWL_EXSTYLE, lStyle);
		SetLayeredWindowAttributes(hDlg, 0, 255, LWA_ALPHA);
		//SetWindowPos(hDlg, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		return FALSE;
	}

}


BOOL CALLBACK EnumChildFunc(HWND hwnd, LPARAM lParam)
{
	CRgn *pRgn = (CRgn*)lParam;
	CRect rcChild;
	::GetWindowRect(hwnd, rcChild);
	CRgn rgnChild;
	CRgn rgnCopy;
	rgnCopy.CreateRectRgn(0, 0, 1, 1);
	rgnCopy.CopyRgn(pRgn);
	rgnChild.CreateRectRgn(rcChild.left, rcChild.top,
		rcChild.right, rcChild.bottom);
	pRgn->CombineRgn(&rgnCopy, &rgnChild, RGN_OR);
	return TRUE;
}


int SetBackTransparent(CWnd *pWnd, BOOL bClientOnly = TRUE)
{
	//pWnd->GetWindowRgn();
	CRgn rgn;
	if (bClientOnly)
	{
		CRgn rgnWindow, rgnClient;
		CRect rcWindow, rcClient, rcRgn;
		pWnd->GetWindowRect(rcWindow);
		pWnd->GetClientRect(rcClient);
		pWnd->ClientToScreen(rcClient);
		rgnWindow.CreateRectRgn(rcWindow.left, rcWindow.top,
			rcWindow.right, rcWindow.bottom);
		rgnClient.CreateRectRgn(rcClient.left, rcClient.top,
			rcClient.right, rcClient.bottom);
		rgn.CreateRectRgn(0, 0, 1, 1);
		rgn.CombineRgn(&rgnWindow, &rgnClient, RGN_DIFF);
	}
	else {
		rgn.CreateRectRgn(0, 0, 0, 0);
	}
	//::EnumChildWindows(pWnd->GetSafeHwnd(), (WNDENUMPROC)EnumChildFunc, (LPARAM)&rgn);
	return pWnd->SetWindowRgn(rgn, TRUE);
}

//全局hook函数
static HWND g_MouseStatus = NULL;
HHOOK g_hookMouse = NULL;
int idHook = WH_MOUSE_LL;
LRESULT CALLBACK LowLevelMouseProc(__in  int nCode, __in  WPARAM wParam, __in  LPARAM lParam)
{

	MSLLHOOKSTRUCT mInfo = *(MSLLHOOKSTRUCT*)lParam;
	CPoint mpt = mInfo.pt;
	CStringA ccchar;

	switch (wParam)
	{
		case WM_MOUSEMOVE:
			{
				//CStringA ccchar;
				ccchar.Format("%s,坐标(%d,%d)","鼠标移动",mpt.x,mpt.y);
				LOG(ccchar.GetString());
				
				//LOG("鼠标移动");
			}
			break;
	case WM_LBUTTONDOWN:
	{
		CVirtualMouseClickDlg::m_blbuttondown = true;
		//CStringA ccchar;
		ccchar.Format("%s,坐标(%d,%d)", "鼠标左键按下", mpt.x, mpt.y);
		LOG(ccchar.GetString());
		//LOG("鼠标左键按下");
	}
	break;
	case WM_LBUTTONUP:
	{
		CVirtualMouseClickDlg::m_blbuttondown = false;
		//CStringA ccchar;
		ccchar.Format("%s,坐标(%d,%d)", "鼠标左键弹起", mpt.x, mpt.y);
		LOG(ccchar.GetString());
		//LOG("鼠标左键弹起");
	}
	break;
	default:
		break;

	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
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


	CRect rectWnd;
	GetWindowRect(&rectWnd);//得到当前窗口（对话框）的坐标
	m_listRect.AddTail(&rectWnd);//将坐标添加到链表listRect的末尾（注意是CList listRect;类的成员变量）

	CWnd *pWndChild = GetWindow(GW_CHILD);//
	while (pWndChild)//依次得到对话框上控件的坐标，并将所有的控件坐标存储在链表中
	{
		pWndChild->GetWindowRect(&rectWnd);
		m_listRect.AddTail(&rectWnd);//由于依次将控件坐标添加到链表末尾，所以开头的坐标是对话框的坐标
		pWndChild = pWndChild->GetNextWindow();
	}
	//SetBackTransparent(FromHandle(this->m_hWnd), TRUE);
	//SetTransparent(this->m_hWnd, TRUE);
	//MARGINS mar;

	//mar.cxLeftWidth = -1;

	//mar.cxRightWidth = -1;

	//mar.cyTopHeight = -1;

	//mar.cyBottomHeight = -1;

	//CRect rectClient;

	//GetClientRect(rectClient);

	//mar.cyTopHeight = rectClient.Height();

	//DwmExtendFrameIntoClientArea(this->GetSafeHwnd(), &mar);

	ModifyStyleEx(0, WS_EX_LAYERED);

	::SetLayeredWindowAttributes(m_hWnd, RGB(0, 255, 0), 255, LWA_COLORKEY);//       //set color transparent，指定透明的颜色

	g_hookMouse = SetWindowsHookExA(idHook, LowLevelMouseProc, NULL, NULL);

	unsigned threadID;

	printf("Creating second thread...\n");

	// Create the second thread.
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, &ClickVir, this, 0, &threadID);
	//OnBnClickedOk();
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
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rcGlassArea;

	GetClientRect(&rcGlassArea);

	//dc.FillSolidRect(&rcGlassArea, RGB(1, 2, 2));
	dc.FillSolidRect(&rcGlassArea, RGB(0, 255, 0));             //填充指定透明的颜色
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

void virmouse_event(DWORD     dwFlags,DWORD     dx,DWORD     dy,DWORD     dwData,ULONG_PTR dwExtraInfo)
{
	CPoint ptCur;
	GetCursorPos(&ptCur);
	mouse_event(MOUSEEVENTF_MOVE| MOUSEEVENTF_ABSOLUTE, dx, dy, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_ABSOLUTE, dx, dy, dwData, dwExtraInfo);
	mouse_event(MOUSEEVENTF_LEFTUP | MOUSEEVENTF_ABSOLUTE, dx, dy, dwData, dwExtraInfo);
	mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, static_cast<long>(65535.0f / (GetSystemMetrics(SM_CXSCREEN) - 1) * ptCur.x), static_cast<long>(65535.0f / (GetSystemMetrics(SM_CYSCREEN) - 1) * ptCur.y) , 0, 0);
}
unsigned Counter;
unsigned __stdcall ClickVir(void* pArguments)
{
	printf("In second thread...\n");

	unsigned seed;  // Random generator seed
   // Use the time function to get a "seed” value for srand
	seed = time(0);
	srand(seed);

	CVirtualMouseClickDlg* pThis = (CVirtualMouseClickDlg*)pArguments;
	int x = 100;
	while (CVirtualMouseClickDlg::g_bloop)
	{
		//mouse_event(MOUSEEVENTF_LEFTDOWN, 400, 400, 0, 0);
		Sleep(rand()%100+1);
		//x += 100;
		//mouse_event(MOUSEEVENTF_LEFTUP, 400, 400, 0, 0);
		
		CRect rcClient;
		pThis->GetClientRect(rcClient);
		  CPoint leftTop = rcClient.TopLeft(); 
		  ClientToScreen(pThis->m_hWnd, &leftTop);
		  int  Width = rcClient.Width();
		  int  Height = rcClient.Height();

		MOUSEINPUT museInput;
		museInput.dwExtraInfo = 0;
		museInput.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
		int mx = (rand() % Width + 1)  + leftTop.x;
		museInput.dx = static_cast<long>(65535.0f / (GetSystemMetrics(SM_CXSCREEN) - 1) * mx);// mx;//
		int my = (rand() % Height + 1)  + leftTop.y;
		museInput.dy = static_cast<long>(65535.0f / (GetSystemMetrics(SM_CYSCREEN) - 1) * my);//my;//
		museInput.mouseData = 0;
		museInput.time = 0;


		INPUT inPut;
		inPut.type = INPUT_MOUSE;
		inPut.mi = museInput;

		if(!CVirtualMouseClickDlg::m_blbuttondown)
			SendInput(1, &inPut, sizeof(inPut));
		//virmouse_event(0, static_cast<long>(65535.0f / (GetSystemMetrics(SM_CXSCREEN) - 1) * mx) , static_cast<long>(65535.0f / (GetSystemMetrics(SM_CYSCREEN) - 1) *( my+50)) , 0, 0);
		
		x += 100;
		//virmouse_event(MOUSEEVENTF_LEFTUP, mx, my, 0, 0);
		//SendInput(1, &inPut, sizeof(inPut));
		Sleep(rand() % 500 + 1);




		if (x > 5000) CVirtualMouseClickDlg::g_bloop = false;
	}

	_endthreadex(0);
	return 0;
}

void CVirtualMouseClickDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialog::OnOK();
	

	// Wait until second thread terminates. If you comment out the line
	// below, Counter will not be correct because the thread has not
	// terminated, and Counter most likely has not been incremented to
	// 1000000 yet.

}


void CVirtualMouseClickDlg::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//m_bTracking = false;
	CDialog::OnMouseLeave();
}


void CVirtualMouseClickDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值//鼠标轨迹跟踪
	//if (!m_bTracking)
	//{
	//	TRACKMOUSEEVENT tme;
	//	tme.cbSize = sizeof(tme);
	//	tme.hwndTrack = m_hWnd;
	//	tme.dwFlags = TME_LEAVE | TME_HOVER; //抓取鼠标离开和鼠标停靠消息
	//	tme.dwHoverTime = 1;
	//	m_bTracking = _TrackMouseEvent(&tme);
	//}

	CDialog::OnMouseMove(nFlags, point);
}


BOOL CAboutDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialog::DestroyWindow();
}


BOOL CVirtualMouseClickDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	CVirtualMouseClickDlg::g_bloop = false;
	WaitForSingleObject(m_hThread, INFINITE);
	printf("Counter should be 1000000; it is-> %d\n", Counter);
	// Destroy the thread object.
	CloseHandle(m_hThread);
	UnhookWindowsHookEx(g_hookMouse);
	return CDialog::DestroyWindow();
}


void CVirtualMouseClickDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_blbuttondown = true;
	CDialog::OnLButtonDown(nFlags, point);
}


void CVirtualMouseClickDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_blbuttondown = false;
	CDialog::OnLButtonUp(nFlags, point);
}


void CVirtualMouseClickDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	float fRateScaleX;
	float fRateScaleY;
	if (m_listRect.GetCount() > 0)//看链表是否为空
	{
		CRect rectDlgNow;
		GetWindowRect(&rectDlgNow);//得到当前对话框的坐标
		POSITION mp = m_listRect.GetHeadPosition();//取得存储在链表中的头元素，其实就是前边的对话框坐标
		CRect rectDlgSaved;
		rectDlgSaved = m_listRect.GetNext(mp);
		ScreenToClient(rectDlgNow);
		fRateScaleX = (float)(rectDlgNow.right - rectDlgNow.left) / (rectDlgSaved.right - rectDlgSaved.left);//拖拉后的窗口大小与原来窗口大小的比例
		fRateScaleY = (float)(rectDlgNow.bottom - rectDlgNow.top) / (rectDlgSaved.bottom - rectDlgSaved.top);
		ClientToScreen(rectDlgNow);
		CRect rectChildSaved;
		CWnd *pWndChild = GetWindow(GW_CHILD);
		while (pWndChild)
		{
			rectChildSaved = m_listRect.GetNext(mp);
			rectChildSaved.left = rectDlgNow.left + (int)((rectChildSaved.left - rectDlgSaved.left)*fRateScaleX);
			rectChildSaved.top = rectDlgNow.top + (int)((rectChildSaved.top - rectDlgSaved.top)*fRateScaleY);
			rectChildSaved.right = rectDlgNow.right + (int)((rectChildSaved.right - rectDlgSaved.right)*fRateScaleX);
			rectChildSaved.bottom = rectDlgNow.bottom + (int)((rectChildSaved.bottom - rectDlgSaved.bottom)*fRateScaleY);
			ScreenToClient(rectChildSaved);
			pWndChild->MoveWindow(rectChildSaved);
			pWndChild = pWndChild->GetNextWindow();
		}
		//SetBackTransparent(this,TRUE);
	}

	Invalidate(); //强制重绘窗口
}
