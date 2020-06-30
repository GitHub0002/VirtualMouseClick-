#pragma once


// PassWordAuthor 对话框

class PassWordAuthor : public CDialogEx
{
	DECLARE_DYNAMIC(PassWordAuthor)

public:
	PassWordAuthor(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PassWordAuthor();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REGISTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPassWord;
	afx_msg void OnBnClickedOk();
};
