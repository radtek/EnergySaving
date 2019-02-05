// DlgAddEvent2.cpp: ���� ����������
//

#include "stdafx.h"
#include "MFCApplication30.h"
#include "DlgAddEvent2.h"
#include "afxdialogex.h"
#include "Data.h"


// ���������� ���� CDlgAddEvent2

IMPLEMENT_DYNAMIC(CDlgAddEvent2, CDialog)

CDlgAddEvent2::CDlgAddEvent2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddEvent2::IDD, pParent)
{

}

CDlgAddEvent2::~CDlgAddEvent2()
{
}

void CDlgAddEvent2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list1);
}


BEGIN_MESSAGE_MAP(CDlgAddEvent2, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAddEvent2::OnBnClickedOk)
END_MESSAGE_MAP()


// ����������� ��������� CDlgAddEvent2

extern CData db;

BOOL CDlgAddEvent2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �������� �������������� �������������
	//��������
	m_list1.SetExtendedStyle(m_list1.GetExStyle()|LVS_EX_CHECKBOXES);
	
	//�������
	m_list1.InsertColumn(0,L"������������ �����������", LVCFMT_LEFT,500);

	CRetQuery crq;

	CString s;

	int iResource=0;
	
	if(StrCmp(db.m_szTypeResource,L"��������� ���")==0)
	{
		iResource=2;
	}
	else
		if(StrCmp(db.m_szTypeResource,L"��������������")==0)
		{
			iResource=3;
		}
		else
			if(StrCmp(db.m_szTypeResource,L"�������� �������")==0)
			{
				iResource=4;
			}

	s.Format(L"SELECT EVS.EVENT_NAME FROM EVENTS EVS "
			 L"LEFT OUTER JOIN (SELECT EVENT FROM PLAN_EVENT WHERE NYEAR=%s) AS T1 "
			 L"ON EVS.EVENT_NAME=T1.EVENT "
			 L"WHERE T1.EVENT IS NULL "
			 L"AND EVS.ID_EVENT_TYPE=%d "
			 L"ORDER BY EVS.EVENT_NAME",db.m_szYear, iResource);

	
	
	db.fQueryRead(s,crq);

#ifdef CONVERTING_ALIAS
	db.Aliases(crq,0,NAMES_TO_ALIAS);
#endif

	if(crq.m_lRecCount!=-0)
	{
		for(int i =0;i<crq.m_lRecCount;i++)
		{
			m_list1.InsertItem(i,crq.m_RetValues[i][0],0);
		}
	}
	
	//���������� ��� crq
					for (int i = 0; i < crq.m_lRecCount; i++)	
					{
						delete [] crq.m_RetValues[i];
					}
					if(crq.m_lRecCount>0)
					delete [] crq.m_RetValues;

	//this->SetIcon(IDR_MAINFRAME,false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����������: �������� ������� OCX ������ ���������� �������� FALSE
}


void CDlgAddEvent2::OnBnClickedOk()
{
	// TODO: �������� ���� ��� ����������� �����������
	int countItems = m_list1.GetItemCount();
	
	CStringArray strArr;

	CString s,b;

	for(int i=0;i<countItems;i++)
	{
		if(m_list1.GetCheck(i))
			strArr.Add(m_list1.GetItemText(i,0));
	}

	b.Format(L"%s",db.m_szYear);

	for(int i=0;i<strArr.GetCount();i++)
	{
#ifdef CONVERTING_ALIAS
		s.Format(L"INSERT INTO PLAN_EVENT (NYEAR, EVENT, ID_EVENT_TYPE) "
		L"SELECT %s AS ���������1, EVENTS.EVENT_NAME, EVENTS.ID_EVENT_TYPE "
		L"FROM EVENTS "
		L"WHERE (((EVENTS.[EVENT_NAME])='%s'))",b,db.Aliases(strArr.GetAt(i),ALIAS_TO_NAMES));
#else
		s.Format(L"INSERT INTO PLAN_EVENT (NYEAR, EVENT, ID_EVENT_TYPE) "
		L"SELECT %s AS ���������1, EVENTS.EVENT_NAME, EVENTS.ID_EVENT_TYPE "
		L"FROM EVENTS "
		L"WHERE (((EVENTS.[EVENT_NAME])='%s'))",b,strArr.GetAt(i));
#endif

		db.ExeSql(s);
		
#ifdef CONVERTING_ALIAS
		s.Format(L"UPDATE PLAN_EVENT SET GENADD=%d WHERE EVENT='%s' AND NYEAR=%s",((StrCmp(db.m_szGenAdd,L"�������� �����")==0)?0:1),db.Aliases(strArr.GetAt(i),ALIAS_TO_NAMES),b);
#else
		s.Format(L"UPDATE PLAN_EVENT SET GENADD=%d WHERE EVENT='%s' AND NYEAR=%s",((StrCmp(db.m_szGenAdd,L"�������� �����")==0)?0:1),strArr.GetAt(i),b);
#endif

		db.ExeSql(s);
	}

	theApp.Mes();
	theApp.UpdateTree();

	strArr.RemoveAll();

	CDialog::OnOK();
}
