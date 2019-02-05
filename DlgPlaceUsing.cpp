// DlgPlaceUsing.cpp: файл реализации
//

#include "stdafx.h"
#include "MFCApplication30.h"
#include "DlgPlaceUsing.h"
#include "afxdialogex.h"
#include "Data.h"


// диалоговое окно CDlgPlaceUsing

IMPLEMENT_DYNAMIC(CDlgPlaceUsing, CDialog)

CDlgPlaceUsing::CDlgPlaceUsing(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPlaceUsing::IDD, pParent)
{

}

CDlgPlaceUsing::~CDlgPlaceUsing()
{
}

void CDlgPlaceUsing::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(CDlgPlaceUsing, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgPlaceUsing::OnBnClickedOk)
END_MESSAGE_MAP()


// обработчики сообщений CDlgPlaceUsing

extern CData db;

void CDlgPlaceUsing::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	CString query, s; m_edit.GetWindowTextW(s);
//	CRetQuery crq;

	//s.Format(L"Y=%s E=%s",db.m_szYear, db.m_szEventName);

#ifdef CONVERTING_ALIAS
	query.Format(
	L"INSERT INTO PLACEOFUSEPLAN ( NAME_PLACE, ID_EVENTPLAN) "
	L"SELECT '%s', ID_EVENTPLAN "
	L"FROM PLAN_EVENT "
	L"WHERE EVENT='%s' AND NYEAR=%s",s,db.Aliases(db.m_szEventName,ALIAS_TO_NAMES), db.m_szYear);
#else
	query.Format(
	L"INSERT INTO PLACEOFUSEPLAN ( NAME_PLACE, ID_EVENTPLAN) "
	L"SELECT '%s', ID_EVENTPLAN "
	L"FROM PLAN_EVENT "
	L"WHERE EVENT='%s' AND NYEAR=%s",s,db.m_szEventName, db.m_szYear);
#endif


	db.ExeSql(query);

	theApp.Mes();
	theApp.UpdateTree();

	CDialog::OnOK();
}
