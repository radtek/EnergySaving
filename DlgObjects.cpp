// DlgObjects.cpp: файл реализации
//

#include "stdafx.h"
#include "MFCApplication30.h"
#include "DlgObjects.h"
#include "afxdialogex.h"
#include "Data.h"
#include "DlgAddNewObject.h"
#include "DlgCalcRes.h"
#include "DlgRedistributionValue.h"

// диалоговое окно CDlgObjects

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


// обработчики сообщений CDlgObjects


//BOOL CDlgObjects::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
//{
//	// TODO: добавьте специализированный код или вызов базового класса
//
//	return CDialog::Create(lpszTemplateName, pParentWnd);
//}

BOOL CDlgObjects::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
	// TODO: добавьте специализированный код или вызов базового класса

	return CDialog::Create(nIDTemplate, pParentWnd);
}



int CDlgObjects::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Добавьте специализированный код создания

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

	// TODO:  Добавить дополнительную инициализацию

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
		s.Format(L"%s г.",db.m_szYear);
	}
	else
	{
		s.Format(L"%s %s г.",db.m_szQuart, db.m_szYear);
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

	//Добавление столбцов
	if((db.m_iStatus==PLAN)&&(StrCmp(db.m_szQuart,L"0")==0))
	{
		m_list1.InsertColumn(0,L"№п/п",LVCFMT_LEFT,50);
		m_list1.InsertColumn(1,L"Объем внедрения",LVCFMT_LEFT,600);
	}
	else
	{
		if(db.m_iStatus==PLAN)
		{
			m_list1.InsertColumn(0,L"№п/п				  ",LVCFMT_LEFT,50);
			m_list1.InsertColumn(1,L"Объем внедрения      ",LVCFMT_LEFT,200);
			m_list1.InsertColumn(2,L"Е.И.				  ",LVCFMT_LEFT,60);
			m_list1.InsertColumn(3,L"П.Эффект			  ",LVCFMT_LEFT,130);
			m_list1.InsertColumn(4,L"П.Эк.эффект, тыс.руб.",LVCFMT_LEFT,130);
			m_list1.InsertColumn(5,L"П.Затраты, тыс.руб.  ",LVCFMT_LEFT,130);
			m_list1.InsertColumn(6,L"П.Время работы, ч	  ",LVCFMT_LEFT,130);
		}
		else
			if((db.m_iStatus==FACT)&&(StrCmp(db.m_szMonth,L"0")!=0))
		{
			m_list1.InsertColumn(0,L"№п/п				  ",LVCFMT_LEFT,50);
			m_list1.InsertColumn(1,L"Объем внедрения      ",LVCFMT_LEFT,200);
			m_list1.InsertColumn(2,L"Е.И.				  ",LVCFMT_LEFT,60);
			m_list1.InsertColumn(3,L"Ф.Эффект			  ",LVCFMT_LEFT,130);
			m_list1.InsertColumn(4,L"Ф.Эк.эффект, тыс.руб.",LVCFMT_LEFT,130);
			m_list1.InsertColumn(5,L"Ф.Затраты, тыс.руб.  ",LVCFMT_LEFT,130);
			m_list1.InsertColumn(6,L"Ф.Время работы,ч	  ",LVCFMT_LEFT,130);
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
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


void CDlgObjects::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	//CString buf; buf.Format(L"%d",pNMItemActivate->iItem);
	

	// TODO: добавьте свой код обработчика уведомлений
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
		else  ///добавление контекстного меню перераспределить объемы на (основной <---> дополнительные) отчеты
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
	// TODO: добавьте свой код обработчика команд
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
			AfxMessageBox(L"Ошибка в методе DlgObjects::UpdateList() 258 страница");
			return 0;
		}
		switch(eu)
		{
		case 2:
			denominator=L"тыс.м3";
			break;
		case 3:
			denominator=L"тыс.кВт х ч";
			break;
		case 4:
			denominator=L"Гкал";
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
				AfxMessageBox(L"Ошибка в методе DlgObjects::UpdateList() 258 страница");
				return 0;
			}
			switch(eu)
			{
				case 2:
					denominator=L"тыс.м3";
					break;
				case 3:
					denominator=L"тыс.кВт х ч";
				break;
				case 4:
					denominator=L"Гкал";
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
	//AfxMessageBox(L"Удаляю объект");
	CString s;
	CRetQuery crq;
	// TODO: добавьте свой код обработчика команд
	if((iCurItem!=-1)&&(iCurSubItem!=-1))
	{

		CString ID_OBJECT;//ИД ОБЪЕКТА
		CString ID_PLACE;//ИД МЕСТА ВНЕДРЕНИЯ
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
	// TODO: добавьте свой код обработчика уведомлений
	theApp.Mes();
	CDialog::OnCancel();
}


void CDlgObjects::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	CDialog::OnLButtonDblClk(nFlags, point);
}


void CDlgObjects::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
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
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Оптимизация режимов работы компрессорных станций и газопроводов"															   )==0) return 1;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Выработка газа потребителям перед выполнением ремонтных работ"						     									   )==0) return 2;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Очистка (промывка) проточной части осевых компрессоров"																	   )==0) return 7;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Ремонт газоперекачивающих агрегатов"																						   )==0) return 8;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Применение безрасходной системы продувки пылеуловителей ГРС"																   )==0) return 12;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Оптимизация количества и времени продувок пылеуловителей"																       )==0) return 15;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Режимно-наладочные испытания котельных"					    															   )==0) return 17;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Перевод освещения промышленных и бытовых помещений на осветительное оборудование с использованием светодиодов"   		       )==0) return 20;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Оптимизация схемы электроснабжения путем вывода в резерв ТП и отключения силовых трансформаторов, работающих на холостом ходу")==0) return 23;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Оптимизация режимов работы вентиляторов АВО газа и АВО масла"																   )==0) return 25;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Очистка поверхностей теплообмена трубных пучков АВО газа и АВО масла"   													   )==0) return 26;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Использование устройств плавного пуска и частотно регулируемого электропривода АВО масла, АВО газа, насосов водоснабжения"	   )==0) return 27;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Установка конденсаторных батарей с автоматическим управлением"																   )==0) return 38;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Проведение тепловизионного обследования зданий и сооружений на выявление теплопотерь с их последующим устранением"		       )==0) return 42;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Использование утилизаторов тепла работающих газоперекачивающих агрегатов"												       )==0) return 43;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Регулирование температуры в тепловой сети отопления в зависимости от наружной температуры"								       )==0) return 48;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Химическая промывка трубных пучков утилизационных теплообменников ГПА"														   )==0) return 100;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Регулирование температуры в тепловой сети отопления в зависимости от наружной температуры1"								   )==0) return 481;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Использование утилизаторов тепла работающих газоперекачивающих агрегатов1"												       )==0) return 431;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Выработка (оттягивание) газа мобильными компрессорными установками из ремонтируемых участков газопроводов"				       )==0) return 101;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Выработка (оттягивание) газа ГПА КС из ремонтируемых участков газопроводов"												   )==0) return 4;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Ремонт газоперекачивающих агрегатов1"																						   )==0) return 81;
#else
	if(StrCmp(db.m_szEventName,L"Оптимизация режимов работы компрессорных станций и газопроводов"															   )==0) return 1;
	if(StrCmp(db.m_szEventName,L"Выработка газа потребителям перед выполнением ремонтных работ"						     									   )==0) return 2;
	if(StrCmp(db.m_szEventName,L"Очистка (промывка) проточной части осевых компрессоров"																	   )==0) return 7;
	if(StrCmp(db.m_szEventName,L"Ремонт газоперекачивающих агрегатов"																						   )==0) return 8;
	if(StrCmp(db.m_szEventName,L"Применение безрасходной системы продувки пылеуловителей ГРС"																   )==0) return 12;
	if(StrCmp(db.m_szEventName,L"Оптимизация количества и времени продувок пылеуловителей"																       )==0) return 15;
	if(StrCmp(db.m_szEventName,L"Режимно-наладочные испытания котельных"					    															   )==0) return 17;
	if(StrCmp(db.m_szEventName,L"Перевод освещения промышленных и бытовых помещений на осветительное оборудование с использованием светодиодов"   		       )==0) return 20;
	if(StrCmp(db.m_szEventName,L"Оптимизация схемы электроснабжения путем вывода в резерв ТП и отключения силовых трансформаторов, работающих на холостом ходу")==0) return 23;
	if(StrCmp(db.m_szEventName,L"Оптимизация режимов работы вентиляторов АВО газа и АВО масла"																   )==0) return 25;
	if(StrCmp(db.m_szEventName,L"Очистка поверхностей теплообмена трубных пучков АВО газа и АВО масла"   													   )==0) return 26;
	if(StrCmp(db.m_szEventName,L"Использование устройств плавного пуска и частотно регулируемого электропривода АВО масла, АВО газа, насосов водоснабжения"	   )==0) return 27;
	if(StrCmp(db.m_szEventName,L"Установка конденсаторных батарей с автоматическим управлением"																   )==0) return 38;
	if(StrCmp(db.m_szEventName,L"Проведение тепловизионного обследования зданий и сооружений на выявление теплопотерь с их последующим устранением"		       )==0) return 42;
	if(StrCmp(db.m_szEventName,L"Использование утилизаторов тепла работающих газоперекачивающих агрегатов"												       )==0) return 43;
	if(StrCmp(db.m_szEventName,L"Регулирование температуры в тепловой сети отопления в зависимости от наружной температуры"								       )==0) return 48;
	if(StrCmp(db.m_szEventName,L"Химическая промывка трубных пучков утилизационных теплообменников ГПА"														   )==0) return 100;
	if(StrCmp(db.m_szEventName,L"Регулирование температуры в тепловой сети отопления в зависимости от наружной температуры1"								   )==0) return 481;
	if(StrCmp(db.m_szEventName,L"Использование утилизаторов тепла работающих газоперекачивающих агрегатов1"												       )==0) return 431;
	if(StrCmp(db.m_szEventName,L"Выработка (оттягивание) газа мобильными компрессорными установками из ремонтируемых участков газопроводов"				       )==0) return 101;
	if(StrCmp(db.m_szEventName,L"Выработка (оттягивание) газа ГПА КС из ремонтируемых участков газопроводов"												   )==0) return 4;
	if(StrCmp(db.m_szEventName,L"Ремонт газоперекачивающих агрегатов1"																						   )==0) return 81;
#endif
	return 0;
}

//Не используется
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
	// TODO: добавьте свой код обработчика команд
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
