// DlgEditResIndexes.cpp: ���� ����������
//

#include "stdafx.h"
#include "MFCApplication30.h"
#include "DlgEditResIndexes.h"
#include "afxdialogex.h"


// ���������� ���� DlgEditResIndexes

IMPLEMENT_DYNAMIC(DlgEditResIndexes, CDialogEx)

DlgEditResIndexes::DlgEditResIndexes(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgEditResIndexes::IDD, pParent)
{

}

DlgEditResIndexes::~DlgEditResIndexes()
{
}

void DlgEditResIndexes::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgEditResIndexes, CDialogEx)
	ON_BN_CLICKED(IDOK, &DlgEditResIndexes::OnBnClickedOk)
END_MESSAGE_MAP()


// ����������� ��������� DlgEditResIndexes


BOOL DlgEditResIndexes::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//����� ������� ������������� ���� ������������� ����� �� ����� ��� �������������� ����� ������������� �����
	db.m_bReconfiguredPlanGTU=true;
	/*
#5312

	return TRUE;  // return TRUE unless you set the focus to a control
#5313

#5312

	return TRUE;  // return TRUE unless you set the focus to a control
#5313
	// TODO:  �������� �������������� �������������

	return TRUE;  // return TRUE unless you set the focus to a control
#5313
	return TRUE;  // return TRUE unless you set the focus to a control
#5313
	// ����������: �������� ������� OCX ������ ���������� �������� FALSE
	*/
	CString m_szYear = db.m_szYear;
	CString m_szEventName = db.m_szEventName;
	CRect rect=NULL;
	CRect oldrect=NULL;
	CStatic *buf;
	//��������� �������

	this->GetWindowRect(rect);
	ScreenToClient(rect);
	
	//buf= (CStatic*) this->GetDlgItem(IDC_STATIC1);
	//ptext->GetWindowRect(oldrect);
	//ScreenToClient(oldrect);
	//ptext->MoveWindow(oldrect.left,oldrect.top,rect.Width()-oldrect.left,oldrect.Height());

	this->SetWindowTextW(L"������������� �����");
	
	buf=(CStatic*) (GetDlgItem(IDC_STATIC_PERIOD));
	buf->SetWindowTextW(L"������: ");

	buf =(CStatic*) (GetDlgItem(IDC_STATIC_EVENT));
	buf->SetWindowTextW(L"�����������: ");

	////////////////////��������� ��������� ���������///////////////////////////////////////////////
	//��� ��������� �������� ����� ������������� ������������ �������� ������������ ������ "������:"
	buf= (CStatic*) this->GetDlgItem(IDC_STATIC_PERIOD);
	buf->GetWindowRect(oldrect);
	ScreenToClient(oldrect);
	buf->MoveWindow(oldrect.left,oldrect.top,rect.Width()-oldrect.left,oldrect.Height());

	buf= (CStatic*) this->GetDlgItem(IDC_STATIC_EVENT);
	//buf->GetWindowRect(oldrect);
	//ScreenToClient(oldrect);
	buf->MoveWindow(oldrect.left,oldrect.top+20, rect.Width()-oldrect.left,oldrect.Height());

	buf= (CStatic*) this->GetDlgItem(IDC_STATIC_PERIOD_VALUE);
	//buf->GetWindowRect(oldrect);
	//ScreenToClient(oldrect);
	buf->MoveWindow(oldrect.left+100,oldrect.top, rect.Width()-oldrect.left,oldrect.Height());

	buf->SetWindowTextW(db.m_szYear);

	buf= (CStatic*) this->GetDlgItem(IDC_STATIC_EVENT_VALUE);
	//buf->GetWindowRect(oldrect);
	//ScreenToClient(oldrect);
	buf->MoveWindow(oldrect.left+100,oldrect.top+20, rect.Width()-oldrect.left,oldrect.Height());

	buf->SetWindowTextW(db.m_szReconfigEvent);
	////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////��������� ���� ���������//////////////////////////////////////////////////
	m_list1=new CListCtrlObj;
	m_list1->Create(WS_VISIBLE|WS_BORDER|LVS_REPORT|LVS_EDITLABELS,CRect(10,10,300,300),this,IDC_LIST1);
	
	DWORD oldStyle=GetWindowLong(m_list1->m_hWnd,GWL_EXSTYLE);
	m_list1->SetExtendedStyle(oldStyle|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	
	int cx=GetSystemMetrics(SM_CXSCREEN);
	int cy=GetSystemMetrics(SM_CYSCREEN);

	MoveWindow(0,0,cx,cy);
	
	CRect r;
	
	this->GetClientRect(r);
	
	m_list1->MoveWindow(10,10,r.Width()-20,r.Height()-20);
	//////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////�������� �������� ���� � LISTCONTROLS/////////////////////////////
	m_list1->InsertColumn(0,L"����������",LVCFMT_LEFT,430);
	m_list1->InsertColumn(1,L"�.�."	     ,LVCFMT_LEFT,80);
	m_list1->InsertColumn(2,L"1 �������" ,LVCFMT_LEFT,80);
	m_list1->InsertColumn(3,L"2 �������" ,LVCFMT_LEFT,80);
	m_list1->InsertColumn(4,L"3 �������" ,LVCFMT_LEFT,80);
	m_list1->InsertColumn(5,L"4 �������" ,LVCFMT_LEFT,80);
	
	m_list1->InsertItem(0,L"��������� ������"						  ,0); m_list1->SetItemText(0,1,L"����/���");
	m_list1->InsertItem(1,L"�������"								  ,0); m_list1->SetItemText(1,1,L"���.���.");
	m_list1->InsertItem(2,L"����� ������ ����� ���������� �����������",0); m_list1->SetItemText(2,1,L"�"	   );
	m_list1->InsertItem(3,L"������������� ������"					  ,0); m_list1->SetItemText(3,1,L"���.���.");


	m_list1->GetItemRect(m_list1->GetItemCount()-1,rect,LVIR_BOUNDS);
	m_list1->MoveWindow(10,60,rect.right,rect.bottom);

	//m_list2.GetItemRect(m_list2.GetItemCount()-1,rect2,LVIR_BOUNDS);
	//m_list2.MoveWindow(rect.right+20,100,rect2.right+20,rect2.bottom+5);
	
	
	CRect rw;
	//buf=(CStatic*) this->GetDlgItem(IDC_STATIC_INITDATA);
	//buf->GetWindowRect(rw);

	//buf=(CStatic*) this->GetDlgItem(IDC_STATIC_RESULTCALC);
	//buf->SetWindowTextW(L"��������� �������");
	//buf->MoveWindow(rect.right + 20,rw.top-30,100,20);

	ClientToScreen(rect);
	//ClientToScreen(rect2);

	CButton* bOK;
	CButton* bCANCEL;

	bOK=(CButton*) this->GetDlgItem(IDOK);
	bCANCEL=(CButton*) this->GetDlgItem(IDCANCEL);

	LONG W = rect.right + 50;
	LONG H = rect.bottom + 130;

	this->MoveWindow((cx/2)-W/2,(cy/2)-H/2,W,H);
	bCANCEL->MoveWindow(rect.right-70,rect.bottom+50,70,30);
	bOK->MoveWindow(rect.right-170,rect.bottom+50,70,30);
	
	//else
	//{
	//	this->MoveWindow(0,0,rect.right + 50 + rect2.right,rect2.bottom+200);
	//	bCANCEL->MoveWindow(rect.right + rect2.right-50,rect2.bottom+100,70,40);
	//	bOK->MoveWindow(rect.right + rect2.right-150,rect2.bottom+100,70,40);
	//}

	//AfxMessageBox(L"BOOL DlgEditResIndexes::OnInitDialog()");

	bOK=NULL;
	bCANCEL=NULL;
	delete bOK;
	delete bCANCEL;
	
	buf=NULL;
	delete buf;
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////������ � ���� ������ �� ����� ����������� ������///////////////////
	if(StrCmp(db.m_szReconfigEvent,L"0")!=0)
	{UpdateListCntr();}
	else 
	{AfxMessageBox(L"DlgEditResIndexes::DlgEditResIndexes() - Error: db.m_szReconfigEvent");}
	/*
	CString szBuffer;
	CRetQuery crq;

		for(int i = 2;i<6;i++)
		{
			szBuffer.Format(L"SELECT SAVEDS, CURCOSTS, CURTIME FROM PLAN_INFO WHERE EVENT='%s' AND NYEAR=%s AND NQUART=%d",L"���������� �������� ������� ������ �������������� ���������������",L"2019",i-1);
			AfxMessageBox(szBuffer);
			db.fQueryRead(szBuffer,crq);

			if(crq.m_lRecCount!=0)
			{
				m_list1->SetItemText(0,i,crq.m_RetValues[0][0]);
				m_list1->SetItemText(1,i,crq.m_RetValues[0][1]);
				m_list1->SetItemText(2,i,crq.m_RetValues[0][2]);
			}
		}
		*/
	////////////////////////////////////////////////////////////////////////////////////////////////////
	

	
		return TRUE;
}

void DlgEditResIndexes::OnBnClickedOk()
{
	// TODO: �������� ���� ��� ����������� �����������
	//���������� ������� � ���� ������ � ������������ � ���� ���������
	CString szBuffer;
	CRetQuery crq;

	for(int i=1;i<5;i++)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(L"SELECT ID_PLAN_INFO FROM PLAN_INFO WHERE EVENT='%s' AND NYEAR=%s AND NQUART=%d",
			db.Aliases(db.m_szReconfigEvent,ALIAS_TO_NAMES),db.m_szYear,i);
		db.fQueryRead(szBuffer,crq);
#else
		szBuffer.Format(L"SELECT ID_PLAN_INFO FROM PLAN_INFO WHERE EVENT='%s' AND NYEAR=%s AND NQUART=%d",
		db.m_szReconfigEvent,db.m_szYear,i);
		db.fQueryRead(szBuffer,crq);
#endif
		//������� �������� ��� �������� �� ������� (��������� ��������)
		if(i==1)
		{
#ifdef CONVERTING_ALIAS
			szBuffer.Format(L"UPDATE PLAN_INFO SET SAVEDS=%s, CURCOSTS=%s, CURTIME=%s, ECONOMY=%s WHERE EVENT='%s' AND NYEAR=%s AND NQUART=%d",
			m_list1->GetItemText(0,i+1),
			m_list1->GetItemText(1,i+1),
			m_list1->GetItemText(2,i+1),
			m_list1->GetItemText(3,i+1),
			db.Aliases(db.m_szReconfigEvent,ALIAS_TO_NAMES),db.m_szYear,i);
			db.ExeSql(szBuffer);
#else
			szBuffer.Format(L"UPDATE PLAN_INFO SET SAVEDS=%s, CURCOSTS=%s, CURTIME=%s, ECONOMY=%s WHERE EVENT='%s' AND NYEAR=%s AND NQUART=%d",
			m_list1->GetItemText(0,i+1),
			m_list1->GetItemText(1,i+1),
			m_list1->GetItemText(2,i+1),
			m_list1->GetItemText(3,i+1),
			db.m_szReconfigEvent ,db.m_szYear,i);
			db.ExeSql(szBuffer);
#endif
		}

#ifdef CONVERTING_ALIAS
		szBuffer.Format(L"UPDATE PLAN_INFO SET SAVEDS=%s, CURCOSTS=%s, CURTIME=%s, ECONOMY=%s WHERE EVENT='%s' AND NYEAR=%s AND NQUART=%d AND ID_PLAN_INFO=%s",
		m_list1->GetItemText(0,i+1),
		m_list1->GetItemText(1,i+1),
		m_list1->GetItemText(2,i+1),
		m_list1->GetItemText(3,i+1),
		db.Aliases(db.m_szReconfigEvent,ALIAS_TO_NAMES),db.m_szYear,i,crq.m_RetValues[0][0]);
		db.ExeSql(szBuffer);
#else
		szBuffer.Format(L"UPDATE PLAN_INFO SET SAVEDS=%s, CURCOSTS=%s, CURTIME=%s, ECONOMY=%s WHERE EVENT='%s' AND NYEAR=%s AND NQUART=%d AND ID_PLAN_INFO=%s",
		m_list1->GetItemText(0,i+1),
		m_list1->GetItemText(1,i+1),
		m_list1->GetItemText(2,i+1),
		m_list1->GetItemText(3,i+1),
		db.m_szReconfigEvent ,db.m_szYear,i,crq.m_RetValues[0][0]);
		db.ExeSql(szBuffer);
#endif
	}
	db.m_bReconfiguredPlanGTU=false;

	//���������� ��� crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	CDialogEx::OnOK();

}

bool DlgEditResIndexes::UpdateListCntr(void)
{
	/////////////////////////////////������ � ���� ������ �� ����� ����������� ������///////////////////
	CString szBuffer;
	CRetQuery crq;
#ifdef CONVERTING_ALIAS
	szBuffer.Format(L"SELECT ID_EVENT_TYPE FROM EVENTS WHERE EVENT_NAME='%s'",db.Aliases(db.m_szReconfigEvent,ALIAS_TO_NAMES));
#else
	szBuffer.Format(L"SELECT ID_EVENT_TYPE FROM EVENTS WHERE EVENT_NAME='%s'",db.m_szReconfigEvent);
#endif
	db.fQueryRead(szBuffer,crq);

	if(crq.m_lRecCount>0)
	{
		switch(_wtoi(crq.m_RetValues[0][0]))
		{
		case 2:
			m_list1->SetItemText(0,1,L"���.�3");
			break;
		case 3:
			m_list1->SetItemText(0,1,L"���.��� � �");
			break;
		case 4:
			m_list1->SetItemText(0,1,L"����");
			break;
		default:
			break;
		}
	}

	for(int i = 2;i<6;i++)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(L"SELECT SUM(SAVEDS), SUM(CURCOSTS), SUM(CURTIME), SUM(ECONOMY) FROM PLAN_INFO WHERE EVENT='%s' AND NYEAR=%s AND NQUART=%d",db.Aliases(db.m_szReconfigEvent,ALIAS_TO_NAMES),db.m_szYear,i-1);
#else
		szBuffer.Format(L"SELECT SUM(SAVEDS), SUM(CURCOSTS), SUM(CURTIME), SUM(ECONOMY) FROM PLAN_INFO WHERE EVENT='%s' AND NYEAR=%s AND NQUART=%d",db.m_szReconfigEvent,db.m_szYear,i-1);
#endif
		//AfxMessageBox(szBuffer);
		db.fQueryRead(szBuffer,crq);
		if(crq.m_lRecCount!=0)
		{
			m_list1->SetItemText(0,i,crq.m_RetValues[0][0]);
			m_list1->SetItemText(1,i,crq.m_RetValues[0][1]);
			m_list1->SetItemText(2,i,crq.m_RetValues[0][2]);
			m_list1->SetItemText(3,i,crq.m_RetValues[0][3]);
		}
		else
		{
			m_list1->SetItemText(0,i,L"0.0");
			m_list1->SetItemText(1,i,L"0.0");
			m_list1->SetItemText(2,i,L"0.0");
			m_list1->SetItemText(3,i,L"0.0");
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////

	//���������� ��� crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}
