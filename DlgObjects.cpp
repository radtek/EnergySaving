// DlgObjects.cpp: ���� ����������
//

#include "stdafx.h"
#include "MFCApplication30.h"
#include "DlgObjects.h"
#include "afxdialogex.h"
#include "Data.h"
#include "DlgAddNewObject.h"
#include "DlgCalcRes.h"
#include "DlgRedistributionValue.h"

// ���������� ���� CDlgObjects

IMPLEMENT_DYNAMIC(CDlgObjects, CDialog)

extern CData db;

CDlgObjects::CDlgObjects(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgObjects::IDD, pParent)
	, iCurItem(0)
	, iCurSubItem(0)
{

}

CDlgObjects::~CDlgObjects()
{
}

void CDlgObjects::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list1);
}


BEGIN_MESSAGE_MAP(CDlgObjects, CDialog)
	ON_WM_CREATE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDlgObjects::OnNMRClickList1)
	ON_COMMAND(ID_ADDOBJECTPLAN, &CDlgObjects::OnAddobjectplan)
	ON_COMMAND(ID_DELOBJECTPLAN, &CDlgObjects::OnDelobjectplan)
	ON_BN_CLICKED(IDCANCEL, &CDlgObjects::OnBnClickedCancel)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlgObjects::OnNMDblclkList1)
	ON_COMMAND(ID_TAKESTOADDITION, &CDlgObjects::OnTakestoaddition)
END_MESSAGE_MAP()


// ����������� ��������� CDlgObjects


//BOOL CDlgObjects::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
//{
//	// TODO: �������� ������������������ ��� ��� ����� �������� ������
//
//	return CDialog::Create(lpszTemplateName, pParentWnd);
//}

BOOL CDlgObjects::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
	// TODO: �������� ������������������ ��� ��� ����� �������� ������

	return CDialog::Create(nIDTemplate, pParentWnd);
}



int CDlgObjects::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �������� ������������������ ��� ��������

	return 0;
}


BOOL CDlgObjects::OnInitDialog()
{
	CDialog::OnInitDialog();

	

	//DWORD oldGenStyle = GetWindowLong(m_list1.m_hWnd,GWL_STYLE);
	//SetWindowLong(m_list1.m_hWnd,GWL_STYLE,oldGenStyle&~LVS_CHEC
	iCurItem=-1;
	iCurSubItem=-1;
	
	int H=GetSystemMetrics(SM_CYSCREEN);
	int W=GetSystemMetrics(SM_CXSCREEN);

	int dx=W/1.5;
	int dy=H/1.5;

	// TODO:  �������� �������������� �������������

	int x=W/2-dx/2;
	int y=H/2-dy/2;
	
	this->MoveWindow(x,y,dx,dy);

	CRect rect=NULL;
	CRect oldrect=NULL;

	this->GetWindowRect(rect);
	
	ScreenToClient(rect);
	
	CStatic* ptext = (CStatic*) this->GetDlgItem(IDC_STATIC1);
	ptext->SetWindowTextW(db.m_szEventName);
	
	ptext->GetWindowRect(oldrect);
	ScreenToClient(oldrect);
	ptext->MoveWindow(oldrect.left,oldrect.top,rect.Width()-oldrect.left,oldrect.Height());

	ptext = (CStatic*) this->GetDlgItem(IDC_STATIC2);
	ptext->SetWindowTextW(db.m_szPlace);

	ptext->GetWindowRect(oldrect);
	ScreenToClient(oldrect);
	ptext->MoveWindow(oldrect.left,oldrect.top,rect.Width()-oldrect.left,oldrect.Height());

	ptext = (CStatic*) this->GetDlgItem(IDC_STATIC3);

	CString s; 
	
	if(StrCmp(db.m_szQuart,L"0")==0)
	{
		s.Format(L"%s �.",db.m_szYear);
	}
	else
	{
		s.Format(L"%s %s �.",db.m_szQuart, db.m_szYear);
	}

	ptext->SetWindowTextW(s);

	ptext->GetWindowRect(oldrect);
	ScreenToClient(oldrect);
	ptext->MoveWindow(oldrect.left,oldrect.top,rect.Width()-oldrect.left,oldrect.Height());

	CButton* pbut=(CButton*) this->GetDlgItem(IDCANCEL);
	
	this->GetClientRect(rect);
	
	pbut->MoveWindow(rect.right-80,rect.bottom-50,60,30);

	ptext=(CStatic*) this->GetDlgItem(IDC_STATICX);
	
	ptext->GetWindowRect(rect);

	ScreenToClient(rect);

	//x=rect.left; y=rect.bottom+10;

	this->GetWindowRect(rect);

	ScreenToClient(rect);
	
	x=rect.left+20; y=rect.top+120;

	dx=rect.right-40; dy=rect.bottom-150;

	m_list1.MoveWindow(x,y,dx,dy);

	//���������� ��������
	if((db.m_iStatus==PLAN)&&(StrCmp(db.m_szQuart,L"0")==0))
	{
		m_list1.InsertColumn(0,L"��/�",LVCFMT_LEFT,50);
		m_list1.InsertColumn(1,L"����� ���������",LVCFMT_LEFT,600);
	}
	else
	{
		if(db.m_iStatus==PLAN)
		{
			m_list1.InsertColumn(0,L"��/�				  ",LVCFMT_LEFT,50);
			m_list1.InsertColumn(1,L"����� ���������      ",LVCFMT_LEFT,200);
			m_list1.InsertColumn(2,L"�.�.				  ",LVCFMT_LEFT,60);
			m_list1.InsertColumn(3,L"�.������			  ",LVCFMT_LEFT,130);
			m_list1.InsertColumn(4,L"�.��.������, ���.���.",LVCFMT_LEFT,130);
			m_list1.InsertColumn(5,L"�.�������, ���.���.  ",LVCFMT_LEFT,130);
			m_list1.InsertColumn(6,L"�.����� ������, �	  ",LVCFMT_LEFT,130);
		}
		else
			if((db.m_iStatus==FACT)&&(StrCmp(db.m_szMonth,L"0")!=0))
		{
			m_list1.InsertColumn(0,L"��/�				  ",LVCFMT_LEFT,50);
			m_list1.InsertColumn(1,L"����� ���������      ",LVCFMT_LEFT,200);
			m_list1.InsertColumn(2,L"�.�.				  ",LVCFMT_LEFT,60);
			m_list1.InsertColumn(3,L"�.������			  ",LVCFMT_LEFT,130);
			m_list1.InsertColumn(4,L"�.��.������, ���.���.",LVCFMT_LEFT,130);
			m_list1.InsertColumn(5,L"�.�������, ���.���.  ",LVCFMT_LEFT,130);
			m_list1.InsertColumn(6,L"�.����� ������,�	  ",LVCFMT_LEFT,130);
		}
	}

	DWORD oldStyle = m_list1.GetExStyle();
	m_list1.SetExtendedStyle((oldStyle|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT)&~LVS_EX_CHECKBOXES);

	pbut=NULL;

	ptext=NULL;

	delete ptext;

	delete pbut;

	UpdateList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����������: �������� ������� OCX ������ ���������� �������� FALSE
}


void CDlgObjects::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	//CString buf; buf.Format(L"%d",pNMItemActivate->iItem);
	

	// TODO: �������� ���� ��� ����������� �����������
	if(pNMItemActivate->iItem==-1)
	{
		if(StrCmp(db.m_szQuart,L"0")==0)
		{
			CPoint obCursorPoint = (0, 0); 

			CMenu* pPopupMenu=new CMenu;

			pPopupMenu->LoadMenuW(IDR_MENUADDOBJECTPLAN);
	
			GetCursorPos(&obCursorPoint);

			CMenu* pSub=pPopupMenu->GetSubMenu(0);

			if(pSub)
			{		
				pSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, obCursorPoint.x,obCursorPoint.y, this);
			}

			pPopupMenu=NULL;
			pSub=NULL;
			delete pPopupMenu;
			delete pSub;
		}
	}
	else
	{
		if(StrCmp(db.m_szQuart,L"0")==0)
		{
			iCurItem=pNMItemActivate->iItem;
			iCurSubItem=pNMItemActivate->iSubItem;

			CPoint obCursorPoint = (0, 0); 

			CMenu* pPopupMenu=new CMenu;

			pPopupMenu->LoadMenuW(IDR_MENUADDOBJECTPLAN);
	
			GetCursorPos(&obCursorPoint);

			CMenu* pSub=pPopupMenu->GetSubMenu(1);

			if(pSub)
			{		
				pSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, obCursorPoint.x,obCursorPoint.y, this);
			}

			pPopupMenu=NULL;
			pSub=NULL;
			delete pPopupMenu;
			delete pSub;
		}
		else  ///���������� ������������ ���� ���������������� ������ �� (�������� <---> ��������������) ������
		{
			if(StrCmp(db.m_szMonth,L"0")!=0)
			{
				iCurItem=pNMItemActivate->iItem;
				iCurSubItem=pNMItemActivate->iSubItem;

				CPoint obCursorPoint = (0, 0); 

				CMenu* pPopupMenu=new CMenu;

				pPopupMenu->LoadMenuW(IDR_TAKESBEETWEN);
	
				GetCursorPos(&obCursorPoint);

				CMenu* pSub=pPopupMenu->GetSubMenu(0);

				if(pSub)
				{		
					pSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, obCursorPoint.x,obCursorPoint.y, this);
				}

				pPopupMenu=NULL;
				pSub=NULL;
				delete pPopupMenu;
				delete pSub;
			}
		}
	}
	*pResult = 0;
}


void CDlgObjects::OnAddobjectplan()
{
	// TODO: �������� ���� ��� ����������� ������
	CDlgAddNewObject dlg;
	dlg.DoModal();
	UpdateList();
}


bool CDlgObjects::UpdateList(void)
{
	CString szBuffer;
	int eu;
	CString denominator;
	CRetQuery crq;
	CString buf2;

	m_list1.DeleteAllItems();
	
	if((db.m_iStatus==PLAN)&&(StrCmp(db.m_szYear,L"0")!=0)&&(StrCmp(db.m_szQuart,L"0")==0))
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT NAME FROM"
		L"(SELECT OP.OBJECT_NAME AS NAME, OP.ID_PLACE "
		L"FROM PLAN_INFO AS PI INNER JOIN OBJECTS_PLANS AS OP ON PI.ID_OBJECT=OP.ID_OBJECT "
		L"WHERE PI.NYEAR=%s AND PI.EVENT='%s' GROUP BY OP.OBJECT_NAME, OP.ID_PLACE) AS T1 "
		L"INNER JOIN PLACEOFUSEPLAN AS PUP ON T1.ID_PLACE=PUP.ID_PLACE WHERE PUP.NAME_PLACE='%s'",
		db.m_szYear,
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),
		db.m_szPlace); 
#else
		szBuffer.Format(
		L"SELECT NAME FROM"
		L"(SELECT OP.OBJECT_NAME AS NAME, OP.ID_PLACE "
		L"FROM PLAN_INFO AS PI INNER JOIN OBJECTS_PLANS AS OP ON PI.ID_OBJECT=OP.ID_OBJECT "
		L"WHERE PI.NYEAR=%s AND PI.EVENT='%s' GROUP BY OP.OBJECT_NAME, OP.ID_PLACE) AS T1 "
		L"INNER JOIN PLACEOFUSEPLAN AS PUP ON T1.ID_PLACE=PUP.ID_PLACE WHERE PUP.NAME_PLACE='%s'",
		db.m_szYear,
		db.m_szEventName,
		db.m_szPlace); 
#endif

		db.fQueryRead(szBuffer,crq);

		//CString n; n.Format(L"%d",crq.m_lRecCount);
		//AfxMessageBox(n);

		for(int i=0;i<crq.m_lRecCount;i++)
		{
			szBuffer.Format(L"%d",i+1);
			m_list1.InsertItem(i,szBuffer,0);
			m_list1.SetItemText(i,1,crq.m_RetValues[i][0]);
		}

		return true;
	}
	else
	if((db.m_iStatus==PLAN)&&(StrCmp(db.m_szQuart,L"0")!=0))
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT EVS.ID_EVENT_TYPE FROM "
		L"PLAN_INFO AS PI INNER JOIN EVENTS AS EVS "
		L"ON PI.EVENT=EVS.EVENT_NAME "
		L"WHERE PI.NYEAR=%s AND PI.NQUART=%d AND PI.EVENT='%s'",
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT EVS.ID_EVENT_TYPE FROM "
		L"PLAN_INFO AS PI INNER JOIN EVENTS AS EVS "
		L"ON PI.EVENT=EVS.EVENT_NAME "
		L"WHERE PI.NYEAR=%s AND PI.NQUART=%d AND PI.EVENT='%s'",
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	

	
		db.fQueryRead(szBuffer,crq);



		if(crq.m_lRecCount!=0)
		{
			eu=_wtoi(crq.m_RetValues[0][0]);
		}
		else
		{
			AfxMessageBox(L"������ � ������ DlgObjects::UpdateList() 258 ��������");
			return 0;
		}
		switch(eu)
		{
		case 2:
			denominator=L"���.�3";
			break;
		case 3:
			denominator=L"���.��� � �";
			break;
		case 4:
			denominator=L"����";
			break;
		default:
			break;
		}

#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		/*L"SELECT OP.OBJECT_NAME, '%s', PI.SAVEDS, PI.ECONOMY, PI.CURCOSTS, PI.CURTIME "
		L"FROM PLAN_INFO AS PI INNER JOIN OBJECTS_PLANS AS OP ON PI.ID_OBJECT=OP.ID_OBJECT "
		L"WHERE PI.NYEAR=%s AND PI.NQUART=%d AND PI.EVENT='%s'",*/
		L"SELECT T1.NAME, '%s', T1.PEF, T1.PEC, T1.PCOST, T1.PTIME FROM "
		L"(SELECT OP.OBJECT_NAME AS NAME, OP.ID_PLACE, PI.SAVEDS AS PEF, PI.ECONOMY AS PEC, PI.CURCOSTS AS PCOST, PI.CURTIME AS PTIME "
		L"FROM PLAN_INFO AS PI INNER JOIN OBJECTS_PLANS AS OP ON PI.ID_OBJECT=OP.ID_OBJECT "
		L"WHERE PI.NYEAR=%s AND NQUART=%d AND PI.EVENT='%s' GROUP BY OP.OBJECT_NAME, OP.ID_PLACE, PI.SAVEDS, PI.ECONOMY, PI.CURCOSTS, PI.CURTIME) AS T1 "
		L"INNER JOIN PLACEOFUSEPLAN AS PUP ON T1.ID_PLACE=PUP.ID_PLACE WHERE PUP.NAME_PLACE='%s'",
		denominator,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),
		db.m_szPlace); 
#else
		szBuffer.Format(
		/*L"SELECT OP.OBJECT_NAME, '%s', PI.SAVEDS, PI.ECONOMY, PI.CURCOSTS, PI.CURTIME "
		L"FROM PLAN_INFO AS PI INNER JOIN OBJECTS_PLANS AS OP ON PI.ID_OBJECT=OP.ID_OBJECT "
		L"WHERE PI.NYEAR=%s AND PI.NQUART=%d AND PI.EVENT='%s'",*/
		L"SELECT T1.NAME, '%s', T1.PEF, T1.PEC, T1.PCOST, T1.PTIME FROM "
		L"(SELECT OP.OBJECT_NAME AS NAME, OP.ID_PLACE, PI.SAVEDS AS PEF, PI.ECONOMY AS PEC, PI.CURCOSTS AS PCOST, PI.CURTIME AS PTIME "
		L"FROM PLAN_INFO AS PI INNER JOIN OBJECTS_PLANS AS OP ON PI.ID_OBJECT=OP.ID_OBJECT "
		L"WHERE PI.NYEAR=%s AND NQUART=%d AND PI.EVENT='%s' GROUP BY OP.OBJECT_NAME, OP.ID_PLACE, PI.SAVEDS, PI.ECONOMY, PI.CURCOSTS, PI.CURTIME) AS T1 "
		L"INNER JOIN PLACEOFUSEPLAN AS PUP ON T1.ID_PLACE=PUP.ID_PLACE WHERE PUP.NAME_PLACE='%s'",
		denominator,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName,
		db.m_szPlace); 
#endif

		db.fQueryRead(szBuffer,crq);

	}
	else
		if((db.m_iStatus==FACT)&&(StrCmp(db.m_szQuart,L"0")!=0)&&(StrCmp(db.m_szMonth,L"0")!=0))
		{
#ifdef CONVERTING_ALIAS
			szBuffer.Format(
			L"SELECT EVS.ID_EVENT_TYPE FROM "
			L"PLAN_INFO AS PI INNER JOIN EVENTS AS EVS "
			L"ON PI.EVENT=EVS.EVENT_NAME "
			L"WHERE PI.NYEAR=%s AND PI.NQUART=%d AND PI.EVENT='%s'",
			db.m_szYear,
			db.defineQuart(),
			db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
			szBuffer.Format(
			L"SELECT EVS.ID_EVENT_TYPE FROM "
			L"PLAN_INFO AS PI INNER JOIN EVENTS AS EVS "
			L"ON PI.EVENT=EVS.EVENT_NAME "
			L"WHERE PI.NYEAR=%s AND PI.NQUART=%d AND PI.EVENT='%s'",
			db.m_szYear,
			db.defineQuart(),
			db.m_szEventName);
#endif
			db.fQueryRead(szBuffer,crq);

			if(crq.m_lRecCount!=0)
			{
				eu=_wtoi(crq.m_RetValues[0][0]);
			}
			else
			{
				AfxMessageBox(L"������ � ������ DlgObjects::UpdateList() 258 ��������");
				return 0;
			}
			switch(eu)
			{
				case 2:
					denominator=L"���.�3";
					break;
				case 3:
					denominator=L"���.��� � �";
				break;
				case 4:
					denominator=L"����";
				break;
			default:
				break;
			}

#ifdef CONVERTING_ALIAS
			szBuffer.Format(
			/*L"SELECT OP.OBJECT_NAME, '%s', PI.SAVEDS, PI.ECONOMY, PI.CURCOSTS, PI.CURTIME "
			L"FROM FACT_INFO AS PI INNER JOIN OBJECTS_PLANS AS OP ON PI.ID_OBJECT=OP.ID_OBJECT "
			L"WHERE PI.NYEAR=%s AND PI.NQUART=%d AND PI.NMONTH=%d AND PI.EVENT='%s'",*/
			L"SELECT NAME, '%s', PEF, PEC, PCOST, PTIME FROM "
			L"(SELECT OP.OBJECT_NAME AS NAME, OP.ID_PLACE, PI.SAVEDS AS PEF, PI.ECONOMY AS PEC, PI.CURCOSTS AS PCOST, PI.CURTIME AS PTIME "
			L"FROM FACT_INFO AS PI INNER JOIN OBJECTS_PLANS AS OP ON PI.ID_OBJECT=OP.ID_OBJECT "
			L"WHERE PI.NYEAR=%s AND NQUART=%d AND NMONTH=%d AND PI.EVENT='%s') AS T1 "
			L"INNER JOIN PLACEOFUSEPLAN AS PUP ON T1.ID_PLACE=PUP.ID_PLACE WHERE PUP.NAME_PLACE='%s'",
			denominator,
			db.m_szYear,
			db.defineQuart(),
			db.defineMonth(),
			db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),
			db.m_szPlace); 
#else
			szBuffer.Format(
			/*L"SELECT OP.OBJECT_NAME, '%s', PI.SAVEDS, PI.ECONOMY, PI.CURCOSTS, PI.CURTIME "
			L"FROM FACT_INFO AS PI INNER JOIN OBJECTS_PLANS AS OP ON PI.ID_OBJECT=OP.ID_OBJECT "
			L"WHERE PI.NYEAR=%s AND PI.NQUART=%d AND PI.NMONTH=%d AND PI.EVENT='%s'",*/
			L"SELECT NAME, '%s', PEF, PEC, PCOST, PTIME FROM "
			L"(SELECT OP.OBJECT_NAME AS NAME, OP.ID_PLACE, PI.SAVEDS AS PEF, PI.ECONOMY AS PEC, PI.CURCOSTS AS PCOST, PI.CURTIME AS PTIME "
			L"FROM FACT_INFO AS PI INNER JOIN OBJECTS_PLANS AS OP ON PI.ID_OBJECT=OP.ID_OBJECT "
			L"WHERE PI.NYEAR=%s AND NQUART=%d AND NMONTH=%d AND PI.EVENT='%s') AS T1 "
			L"INNER JOIN PLACEOFUSEPLAN AS PUP ON T1.ID_PLACE=PUP.ID_PLACE WHERE PUP.NAME_PLACE='%s'",
			denominator,
			db.m_szYear,
			db.defineQuart(),
			db.defineMonth(),
			db.m_szEventName,
			db.m_szPlace); 
#endif

			buf2.Format(L"!!! %s",szBuffer);
			//AfxMessageBox(buf2);
			

			db.fQueryRead(szBuffer,crq);

			/*szBuffer.Format(
			L"SELECT OBJECTS_PLANS.OBJECT_NAME FROM (SELECT PLUSE.ID_PLACE FROM "
			L"(SELECT ID_EVENTPLAN FROM PLAN_EVENT WHERE NYEAR=%s AND EVENT='%s') AS T1 "
			L"INNER JOIN PLACEOFUSEPLAN AS PLUSE ON T1.ID_EVENTPLAN=PLUSE.ID_EVENTPLAN WHERE NAME_PLACE='%s') AS T2 INNER JOIN OBJECTS_PLANS "
			L"ON T2.ID_PLACE=OBJECTS_PLANS.ID_PLACE",db.m_szYear,db.m_szEventName,db.m_szPlace);
			db.fQueryRead(szBuffer,crq);*/
		}

		//CString s; s.Format(L"%d",crq.m_lRecCount);
		//AfxMessageBox(szBuffer);

	if(!szBuffer.IsEmpty())
	{
		for(int i=0;i<crq.m_lRecCount;i++)
		{
			szBuffer.Format(L"%d",i+1);
			m_list1.InsertItem(i,szBuffer,0);
			m_list1.SetItemText(i,1,crq.m_RetValues[i][0]);
			m_list1.SetItemText(i,2,crq.m_RetValues[i][1]);
			m_list1.SetItemText(i,3,crq.m_RetValues[i][2]);
			m_list1.SetItemText(i,4,crq.m_RetValues[i][3]);
			m_list1.SetItemText(i,5,crq.m_RetValues[i][4]);
			m_list1.SetItemText(i,6,crq.m_RetValues[i][5]);
		}
	}

	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
		delete [] crq.m_RetValues;

	return false;
}


void CDlgObjects::OnDelobjectplan()
{
	//AfxMessageBox(L"������ ������");
	CString s;
	CRetQuery crq;
	// TODO: �������� ���� ��� ����������� ������
	if((iCurItem!=-1)&&(iCurSubItem!=-1))
	{

		CString ID_OBJECT;//�� �������
		CString ID_PLACE;//�� ����� ���������
		CString szBuffer;

#ifdef CONVERTING_ALIAS
		s.Format(
		L"SELECT T2.ID_OBJECT FROM "
		L"(SELECT PLUSE.ID_PLACE FROM PLAN_EVENT AS PE INNER JOIN PLACEOFUSEPLAN AS PLUSE "
		L"ON PE.ID_EVENTPLAN=PLUSE.ID_EVENTPLAN "
		L"WHERE PE.NYEAR=%s AND PE.EVENT='%s' "
		L"AND PLUSE.NAME_PLACE='%s') T1 INNER JOIN OBJECTS_PLANS AS T2 "
		L"ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.OBJECT_NAME='%s'",
		db.m_szYear,
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),
		db.m_szPlace,
		m_list1.GetItemText(iCurItem,iCurSubItem));
#else
		s.Format(
		L"SELECT T2.ID_OBJECT FROM "
		L"(SELECT PLUSE.ID_PLACE FROM PLAN_EVENT AS PE INNER JOIN PLACEOFUSEPLAN AS PLUSE "
		L"ON PE.ID_EVENTPLAN=PLUSE.ID_EVENTPLAN "
		L"WHERE PE.NYEAR=%s AND PE.EVENT='%s' "
		L"AND PLUSE.NAME_PLACE='%s') T1 INNER JOIN OBJECTS_PLANS AS T2 "
		L"ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.OBJECT_NAME='%s'",
		db.m_szYear,
		db.m_szEventName,
		db.m_szPlace,
		m_list1.GetItemText(iCurItem,iCurSubItem));
#endif
		

		db.fQueryRead(s,crq);

		ID_OBJECT=crq.m_RetValues[0][0];

		s.Format(L"SELECT ID_PLACE FROM OBJECTS_PLANS WHERE ID_OBJECT=%s",ID_OBJECT);
		db.fQueryRead(s,crq);

		ID_PLACE=crq.m_RetValues[0][0];

		DelDataEvents(ID_OBJECT);

		s.Format(L"DELETE FROM PLAN_INFO WHERE ID_OBJECT=%s",ID_OBJECT);
		db.ExeSql(s);

		s.Format(L"DELETE FROM FACT_INFO WHERE ID_OBJECT=%s",ID_OBJECT);
		db.ExeSql(s);

		s.Format(L"DELETE FROM OBJECTS_PLANS WHERE ID_OBJECT=%s AND ID_PLACE=%s",ID_OBJECT,ID_PLACE);
		db.ExeSql(s);
		
		for (int i = 0; i < crq.m_lRecCount; i++)	
		{
			delete [] crq.m_RetValues[i];
		}
		if(crq.m_lRecCount>0)
			delete [] crq.m_RetValues;

		UpdateList();
	}
	//if(crq.m_lRecCount>0)
	///	for (int i = 0; i < crq.m_lRecCount; i++)	
	//	{
	//		delete [] crq.m_RetValues[i];
	//	}
	//if(crq.m_lRecCount>0)
	//delete [] crq.m_RetValues;
}


void CDlgObjects::OnBnClickedCancel()
{
	// TODO: �������� ���� ��� ����������� �����������
	theApp.Mes();
	CDialog::OnCancel();
}


void CDlgObjects::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �������� ���� ��� ����������� ��������� ��� ����� ������������
	CDialog::OnLButtonDblClk(nFlags, point);
}


void CDlgObjects::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �������� ���� ��� ����������� �����������
	if(StrCmp(db.m_szQuart,L"0")!=0)
	{
		if(pNMItemActivate->iItem==-1)
		{
			*pResult = 0;
			return;
		}
		else
		{
			db.m_szCurObject=m_list1.GetItemText(pNMItemActivate->iItem, pNMItemActivate->iSubItem);
			CDlgCalcRes dlg;
			dlg.DoModal();
			UpdateList();
			*pResult=0;
		}
	}

	*pResult = 0;
}


void CDlgObjects::DelDataEvents(CString OBJECT)
{
	CString s;
	CString szBuffer;

	//szBuffer.Format(L"SELECT ID_PLAN_INFO FROM PLAN_INFO WHERE ID_OBJECT=%s",OBJECT);

	//db.fQueryRead(szBuffer,crq);

	//for(int i=0;i<crq.m_lRecCount;i++)
	//{
		szBuffer.Format(L"DELETE FROM %d WHERE ID_OBJECT=%s",defineNumberEvent(),OBJECT);
		//AfxMessageBox(szBuffer);
		db.ExeSql(szBuffer);
	//}

	//szBuffer.Format(L"SELECT ID_FACT_INFO FROM FACT_INFO WHERE ID_OBJECT=%s",OBJECT);

	//db.fQueryRead(szBuffer,crq);

	//for(int i=0;i<crq.m_lRecCount;i++)
	//{
		szBuffer.Format(L"DELETE FROM %dF WHERE ID_OBJECT=%s",defineNumberEvent(),OBJECT);
		db.ExeSql(szBuffer);
	//}

	/*
	if(defineNumberEvent()!=0)
	{
		s.Format(L"DELETE FROM %d WHERE ID_OBJECT=%s",defineNumberEvent(),OBJECT);
		db.ExeSql(s);
		AfxMessageBox(s);
	}
	*/
	return;
}


int CDlgObjects::defineNumberEvent(void)
{
#ifdef CONVERTING_ALIAS
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"����������� ������� ������ ������������� ������� � ������������"															   )==0) return 1;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"��������� ���� ������������ ����� ����������� ��������� �����"						     									   )==0) return 2;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"������� (��������) ��������� ����� ������ ������������"																	   )==0) return 7;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"������ ������������������ ���������"																						   )==0) return 8;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"���������� ������������ ������� �������� �������������� ���"																   )==0) return 12;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"����������� ���������� � ������� �������� ��������������"																       )==0) return 15;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"�������-���������� ��������� ���������"					    															   )==0) return 17;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"������� ��������� ������������ � ������� ��������� �� ������������� ������������ � �������������� �����������"   		       )==0) return 20;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"����������� ����� ���������������� ����� ������ � ������ �� � ���������� ������� ���������������, ���������� �� �������� ����")==0) return 23;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"����������� ������� ������ ������������ ��� ���� � ��� �����"																   )==0) return 25;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"������� ������������ ����������� ������� ������ ��� ���� � ��� �����"   													   )==0) return 26;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"������������� ��������� �������� ����� � �������� ������������� �������������� ��� �����, ��� ����, ������� �������������"	   )==0) return 27;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"��������� �������������� ������� � �������������� �����������"																   )==0) return 38;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"���������� ��������������� ������������ ������ � ���������� �� ��������� ����������� � �� ����������� �����������"		       )==0) return 42;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"������������� ������������ ����� ���������� ������������������ ���������"												       )==0) return 43;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"������������� ����������� � �������� ���� ��������� � ����������� �� �������� �����������"								       )==0) return 48;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"���������� �������� ������� ������ �������������� ��������������� ���"														   )==0) return 100;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"������������� ����������� � �������� ���� ��������� � ����������� �� �������� �����������1"								   )==0) return 481;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"������������� ������������ ����� ���������� ������������������ ���������1"												       )==0) return 431;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"��������� (�����������) ���� ���������� �������������� ����������� �� ������������� �������� ������������"				       )==0) return 101;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"��������� (�����������) ���� ��� �� �� ������������� �������� ������������"												   )==0) return 4;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"������ ������������������ ���������1"																						   )==0) return 81;
#else
	if(StrCmp(db.m_szEventName,L"����������� ������� ������ ������������� ������� � ������������"															   )==0) return 1;
	if(StrCmp(db.m_szEventName,L"��������� ���� ������������ ����� ����������� ��������� �����"						     									   )==0) return 2;
	if(StrCmp(db.m_szEventName,L"������� (��������) ��������� ����� ������ ������������"																	   )==0) return 7;
	if(StrCmp(db.m_szEventName,L"������ ������������������ ���������"																						   )==0) return 8;
	if(StrCmp(db.m_szEventName,L"���������� ������������ ������� �������� �������������� ���"																   )==0) return 12;
	if(StrCmp(db.m_szEventName,L"����������� ���������� � ������� �������� ��������������"																       )==0) return 15;
	if(StrCmp(db.m_szEventName,L"�������-���������� ��������� ���������"					    															   )==0) return 17;
	if(StrCmp(db.m_szEventName,L"������� ��������� ������������ � ������� ��������� �� ������������� ������������ � �������������� �����������"   		       )==0) return 20;
	if(StrCmp(db.m_szEventName,L"����������� ����� ���������������� ����� ������ � ������ �� � ���������� ������� ���������������, ���������� �� �������� ����")==0) return 23;
	if(StrCmp(db.m_szEventName,L"����������� ������� ������ ������������ ��� ���� � ��� �����"																   )==0) return 25;
	if(StrCmp(db.m_szEventName,L"������� ������������ ����������� ������� ������ ��� ���� � ��� �����"   													   )==0) return 26;
	if(StrCmp(db.m_szEventName,L"������������� ��������� �������� ����� � �������� ������������� �������������� ��� �����, ��� ����, ������� �������������"	   )==0) return 27;
	if(StrCmp(db.m_szEventName,L"��������� �������������� ������� � �������������� �����������"																   )==0) return 38;
	if(StrCmp(db.m_szEventName,L"���������� ��������������� ������������ ������ � ���������� �� ��������� ����������� � �� ����������� �����������"		       )==0) return 42;
	if(StrCmp(db.m_szEventName,L"������������� ������������ ����� ���������� ������������������ ���������"												       )==0) return 43;
	if(StrCmp(db.m_szEventName,L"������������� ����������� � �������� ���� ��������� � ����������� �� �������� �����������"								       )==0) return 48;
	if(StrCmp(db.m_szEventName,L"���������� �������� ������� ������ �������������� ��������������� ���"														   )==0) return 100;
	if(StrCmp(db.m_szEventName,L"������������� ����������� � �������� ���� ��������� � ����������� �� �������� �����������1"								   )==0) return 481;
	if(StrCmp(db.m_szEventName,L"������������� ������������ ����� ���������� ������������������ ���������1"												       )==0) return 431;
	if(StrCmp(db.m_szEventName,L"��������� (�����������) ���� ���������� �������������� ����������� �� ������������� �������� ������������"				       )==0) return 101;
	if(StrCmp(db.m_szEventName,L"��������� (�����������) ���� ��� �� �� ������������� �������� ������������"												   )==0) return 4;
	if(StrCmp(db.m_szEventName,L"������ ������������������ ���������1"																						   )==0) return 81;
#endif
	return 0;
}

//�� ������������
CString CDlgObjects::DelCalcs(void)
{
//	CRetQuery crq;
	switch (defineNumberEvent())
	{
	case 2:
		return L"2";
		break;
	default:
		break;
	}
	return false;
}


void CDlgObjects::OnTakestoaddition()
{
	// TODO: �������� ���� ��� ����������� ������
	CString szBuffer;

	CListCtrl* pReport = (CListCtrl*) this->GetDlgItem(IDC_LIST1);
	
	POSITION pos = pReport->GetFirstSelectedItemPosition();
	int n=pReport->GetNextSelectedItem(pos);

	CDlgRedistributionValue dlg(pReport->GetItemText(n,3),pReport->GetItemText(n,1));
	dlg.DoModal();
	
	//LPWSTR szValue(L"");//=(LPWSTR*) malloc(sizeof(LPWSTR));
	//
	//CListCtrl* pReport = (CListCtrl*) this->GetDlgItem(IDC_LIST1);

	//POSITION pos = pReport->GetFirstSelectedItemPosition();
	//
	//int n=pReport->GetNextSelectedItem(pos);



	////pReport->GetItemText(pReport->selecte
	//pReport=NULL; delete pReport;
	//szValue=NULL; delete szValue;
}
