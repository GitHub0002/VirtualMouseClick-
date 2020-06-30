// PassWordAuthor.cpp: 实现文件
//

#include "pch.h"
#include "VirtualMouseClick.h"
#include "PassWordAuthor.h"
#include "afxdialogex.h"


// PassWordAuthor 对话框

IMPLEMENT_DYNAMIC(PassWordAuthor, CDialogEx)

PassWordAuthor::PassWordAuthor(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_REGISTER, pParent)
	, m_strPassWord(_T(""))
{

}

PassWordAuthor::~PassWordAuthor()
{
}

void PassWordAuthor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassWord);
}


BEGIN_MESSAGE_MAP(PassWordAuthor, CDialogEx)
	ON_BN_CLICKED(IDOK, &PassWordAuthor::OnBnClickedOk)
END_MESSAGE_MAP()


// PassWordAuthor 消息处理程序


void PassWordAuthor::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	UpdateData(TRUE);
	if (0 == StrCmp(L"7FF7DFAF-A1C4-455D-9702-9958DB77A714",m_strPassWord.GetString()))
	{
		EndDialog(5);
	}
	else
	{
		EndDialog(-1);
	}
}
