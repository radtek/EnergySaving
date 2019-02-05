// DlgEditsYearsAdd.cpp: ���� ����������
//

#include "stdafx.h"
#include "MFCApplication30.h"
#include "DlgEditsYearsAdd.h"
#include "afxdialogex.h"
#include "Data.h"



// ���������� ���� CDlgEditsYearsAdd

IMPLEMENT_DYNAMIC(CDlgEditsYearsAdd, CDialogEx)

extern CData db;

CDlgEditsYearsAdd::CDlgEditsYearsAdd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgEditsYearsAdd::IDD, pParent)
{

}

CDlgEditsYearsAdd::~CDlgEditsYearsAdd()
{
}

void CDlgEditsYearsAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(CDlgEditsYearsAdd, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgEditsYearsAdd::OnBnClickedOk)
END_MESSAGE_MAP()


// ����������� ��������� CDlgEditsYearsAdd


void CDlgEditsYearsAdd::OnBnClickedOk()
{
	// TODO: �������� ���� ��� ����������� �����������
	CString szBuffer;
	CString szBuffer2;
	m_edit.GetWindowTextW(szBuffer);
	if(szBuffer.IsEmpty())
	{
		AfxMessageBox(L"��������� ���� ���");
		return;
	}

	szBuffer2.Format(L"INSERT INTO YEARS(YEAR_NAME) VALUES(%s)", szBuffer);
	db.ExeSql(szBuffer2);

	CDialogEx::OnOK();
}
