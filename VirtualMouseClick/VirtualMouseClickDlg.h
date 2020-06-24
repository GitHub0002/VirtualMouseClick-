
// VirtualMouseClickDlg.h: 头文件
//

#pragma once


// CVirtualMouseClickDlg 对话框
class CVirtualMouseClickDlg : public CDialog
{
// 构造
public:
	CVirtualMouseClickDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIRTUALMOUSECLICK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	bool m_bTracking;
	volatile static bool g_bloop ;
	volatile static bool m_blbuttondown;
	HANDLE m_hThread;
	CList<CRect> m_listRect;
	afx_msg void OnBnClickedOk();
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL DestroyWindow();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
unsigned __stdcall ClickVir(void* pArguments);
void virmouse_event(
	DWORD     dwFlags,
	DWORD     dx,
	DWORD     dy,
	DWORD     dwData,
	ULONG_PTR dwExtraInfo
);