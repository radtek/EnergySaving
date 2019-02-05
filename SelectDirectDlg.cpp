// SelectDirectDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "MFCApplication30.h"
#include "SelectDirectDlg.h"
#include "afxdialogex.h"


// диалоговое окно CSelectDirectDlg

IMPLEMENT_DYNAMIC(CSelectDirectDlg, CDialog)

CSelectDirectDlg::CSelectDirectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectDirectDlg::IDD, pParent)
	, radiobut_Plan(FALSE)
{

}

CSelectDirectDlg::~CSelectDirectDlg()
{
}

void CSelectDirectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, radiobut_Plan);
}


BEGIN_MESSAGE_MAP(CSelectDirectDlg, CDialog)
END_MESSAGE_MAP()


// обработчики сообщений CSelectDirectDlg
