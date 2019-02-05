// SettingsStructureEvents.cpp: файл реализации
//

#include "stdafx.h"
#include "MFCApplication30.h"
#include "DlgAddEvents.h"
#include "afxdialogex.h"
#include "Data.h"

extern CData db;

// диалоговое окно DlgAddEvents

IMPLEMENT_DYNAMIC(DlgAddEvents, CDialog)

DlgAddEvents::DlgAddEvents(CWnd* pParent /*=NULL*/)
	: CDialog(DlgAddEvents::IDD, pParent)
	, m_pszAllEvents(NULL)
{

}

DlgAddEvents::~DlgAddEvents()
{
}

void DlgAddEvents::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, tab1);
	DDX_Control(pDX, IDOK, ButtonOk);
	DDX_Control(pDX, IDCANCEL, bCancel);
	DDX_Control(pDX, IDC_TAB2, tab2);
	DDX_Control(pDX, IDC_LIST1, listctrl1);
	DDX_Control(pDX, IDC_EDIT1, ectrlYear);
}


BEGIN_MESSAGE_MAP(DlgAddEvents, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &DlgAddEvents::OnTcnSelchangeTab2)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB2, &DlgAddEvents::OnTcnSelchangingTab2)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, &DlgAddEvents::OnTcnSelchangingTab1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &DlgAddEvents::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDOK, &DlgAddEvents::OnBnClickedOk)
END_MESSAGE_MAP()

// обработчики сообщений DlgAddEvents


BOOL DlgAddEvents::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitListsControls();

	InitEventsList();

	SetItemsListControl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


void DlgAddEvents::OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult)
{
	listctrl1.DeleteAllItems();
	SetItemsListControl();
}

void DlgAddEvents::CalcCheckTrueBoxes()
{
	for(int i=0;i<listctrl1.GetItemCount();i++)
	{
		if (listctrl1.GetCheck(i))
			for(int j=0;j<m_iCountElements;j++)
			{
				if(StrCmp(listctrl1.GetItemText(i,0),m_pszAllEvents[0][j])==0)
				{
					m_iEnableElements[j] = 1;
					m_piGenAddReport[j] = tab1.GetCurSel();
				}
			}
		else
			for(int j=0;j<m_iCountElements;j++)
			{
				if(StrCmp(listctrl1.GetItemText(i,0),m_pszAllEvents[0][j])==0)
				{
					m_iEnableElements[j] = 0;
					m_piGenAddReport[j] = -1;
				}
			}
	}
}

void DlgAddEvents::OnTcnSelchangingTab2(NMHDR *pNMHDR, LRESULT *pResult)
{
	CalcCheckTrueBoxes();
	/*
	for(int i=0;i<listctrl1.GetItemCount();i++)
	{
		if (listctrl1.GetCheck(i))
			for(int j=0;j<m_iCountElements;j++)
			{
				if(StrCmp(listctrl1.GetItemText(i,0),m_pszAllEvents[0][j])==0)
				{
					m_iEnableElements[j] = 1;
					m_piGenAddReport[j] = tab1.GetCurSel();
				}
			}
		else
			for(int j=0;j<m_iCountElements;j++)
			{
				if(StrCmp(listctrl1.GetItemText(i,0),m_pszAllEvents[0][j])==0)
				{
					m_iEnableElements[j] = 0;
					m_piGenAddReport[j] = -1;
				}
			}
	}
	*/
}


void DlgAddEvents::SetCheckLc(CListCtrl& reflistctrl, CArray<CString,CString>& refArray)
{
}


void DlgAddEvents::FillArray(CArray<CString,CString>& arr, CListCtrl& reflist)
{
}


void DlgAddEvents::OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: добавьте свой код обработчика уведомлений
	CalcCheckTrueBoxes();
	/*
	for(int i=0;i<listctrl1.GetItemCount();i++)
	{
		if (listctrl1.GetCheck(i))
			for(int j=0;j<m_iCountElements;j++)
			{
				if(StrCmp(listctrl1.GetItemText(i,0),m_pszAllEvents[0][j])==0)
				{
					m_iEnableElements[j] = 1;
					m_piGenAddReport[j] = tab1.GetCurSel();
				}
			}
		else
			for(int j=0;j<m_iCountElements;j++)
			{
				if(StrCmp(listctrl1.GetItemText(i,0),m_pszAllEvents[0][j])==0)
				{
					m_iEnableElements[j] = 0;
					m_piGenAddReport[j] = -1;
				}
			}
	}
	*/
	*pResult = 0;
}


void DlgAddEvents::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	listctrl1.DeleteAllItems();
	SetItemsListControl();
}

void DlgAddEvents::InitListsControls(void)
{
	CRetQuery crq;

	MoveWindow(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	CRect cr;
	GetClientRect(cr);

	tab1.MoveWindow(cr.left+10,cr.top+20, cr.Width()-20, cr.Height()-50);
	
	ButtonOk.MoveWindow(cr.Width()-180,cr.Height()-30,75,23);
	
	bCancel.MoveWindow(cr.Width()-90,cr.Height()-30,75,23);
	
	tab1.InsertItem(0,L"Основной отчет");
	tab1.InsertItem(1,L"Дополнителный отчет");
	
	tab2.MoveWindow(cr.left+15,cr.top+50, cr.Width()-30, cr.Height()-110);
	tab2.BringWindowToTop();
	tab2.InsertItem(0,L"Природный газ");
	tab2.InsertItem(1,L"Электроэнергия");
	tab2.InsertItem(2,L"Тепловая энергия");
	
	listctrl1.SetExtendedStyle(listctrl1.GetStyle()|LVS_EX_CHECKBOXES);
	listctrl1.InsertColumn(0,L"Наименование мероприятия",LVCFMT_FILL,1000);
	listctrl1.BringWindowToTop();
	listctrl1.MoveWindow(cr.top+15,cr.left+70, cr.Width()-32, cr.Height()-101);

	ectrlYear.MoveWindow(cr.Width()-100,cr.top+10,80,20);
	
	db.fQueryRead(L"SELECT MAX(YEAR_NAME) FROM YEARS",crq);

	CStatic* ptext=(CStatic*) this->GetDlgItem(IDC_STATIC);
	ptext->MoveWindow(cr.Width()-220,cr.top+10,100,20);
	if(crq.m_lRecCount!=0)
	{
		int y=_wtoi(crq.m_RetValues[0][0]); y+=1;
		CString s; s.Format(L"%d",y);
		ectrlYear.SetWindowTextW(s);
	}

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	ptext=NULL;
	delete ptext;
}

void DlgAddEvents::InitEventsList()
{
	CRetQuery crq;
	
	CString strQ = L"SELECT EVENT_NAME, ID_EVENT_TYPE FROM EVENTS ORDER BY EVENT_NAME";

	db.fQueryRead(strQ,crq);

#ifdef CONVERTING_ALIAS
	db.Aliases(crq,0,NAMES_TO_ALIAS);
#endif

	strQ.Format(L"Rows = %d, Columns = %d",crq.m_lRecCount,crq.m_lColCount);

	m_pszAllEvents = new CString*[2];
	m_pszAllEvents[0] = new CString[crq.m_lRecCount];
	m_pszAllEvents[1] = new CString[crq.m_lRecCount];
	m_iEnableElements = new int[crq.m_lRecCount];
	m_piGenAddReport = new int[crq.m_lRecCount];

	for(int i=0;i<crq.m_lRecCount;i++)
	{
		m_pszAllEvents[0][i] = crq.m_RetValues[i][0];
		m_pszAllEvents[1][i] = crq.m_RetValues[i][1];
		m_iEnableElements[i] = 0;
		m_piGenAddReport[i] = -1;
	}
	
	m_iCountElements=crq.m_lRecCount;
	
	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return;
}

void DlgAddEvents::SetItemsListControl(void)
{
	int selector = 0;
	if (tab2.GetCurSel()==0)
	{
		for(int i=0;i<m_iCountElements;i++)
		{
			if ((m_piGenAddReport[i]==tab1.GetCurSel())||(m_piGenAddReport[i]==-1))
			{
				if (!StrCmp(m_pszAllEvents[1][i],L"2"))
				{
					listctrl1.InsertItem(selector,m_pszAllEvents[0][i]);
					if (m_iEnableElements[i]==1)
						listctrl1.SetCheck(selector);
				}
			}
		}
	}
	else
	if(tab2.GetCurSel()==1)
	{

			for(int i=0;i<m_iCountElements;i++)
			{
				if ((m_piGenAddReport[i]==tab1.GetCurSel())||(m_piGenAddReport[i]==-1))
				{
					if (!StrCmp(m_pszAllEvents[1][i],L"3"))
					{
						listctrl1.InsertItem(selector,m_pszAllEvents[0][i]);
						if (m_iEnableElements[i]==1)
							listctrl1.SetCheck(selector);
					}
				}
			}
	}
	else
	if (tab2.GetCurSel()==2)
	{
		for(int i=0;i<m_iCountElements;i++)
		{
			if ((m_piGenAddReport[i]==tab1.GetCurSel())||(m_piGenAddReport[i]==-1))
			{
				if (!StrCmp(m_pszAllEvents[1][i],L"4"))
				{
					listctrl1.InsertItem(selector,m_pszAllEvents[0][i]);
					if (m_iEnableElements[i]==1)
						listctrl1.SetCheck(selector);
				}
			}
		}
	}
}

void DlgAddEvents::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	CalcCheckTrueBoxes();
	CString str;
	CString str2;
	CString str3;
	CString str4;
	int counter_quart = 1;
	int counter_month = 1;
	CString mesuar = L"0";
	CRetQuery crq;
	CString year;

	//AfxMessageBox(L"Привет!!!");

	ectrlYear.GetWindowTextW(year);
	
	if(year.IsEmpty())
	{
		AfxMessageBox(L"Error: Не заполнено поле Год, сохранение невозможно.");
		return;
	}
	else
	{
		str.Format(L"INSERT INTO YEARS(YEAR_NAME) VALUES (%s)",year);
		db.ExeSql(str);
	}

	for(int i=0;i<m_iCountElements;i++)
	{
		if(m_iEnableElements[i]==1)
		{
			str2.Format(L"%d",m_piGenAddReport[i]);

#ifdef CONVERTING_ALIAS
			str.Format(L"INSERT INTO PLAN_EVENT (NYEAR,EVENT,GENADD,ID_EVENT_TYPE) SELECT %s, '%s', %s, ID_EVENT_TYPE FROM EVENTS WHERE EVENT_NAME='%s'",year,
				db.Aliases(m_pszAllEvents[0][i],ALIAS_TO_NAMES),str2,db.Aliases(m_pszAllEvents[0][i],ALIAS_TO_NAMES));
#else
			str.Format(L"INSERT INTO PLAN_EVENT (NYEAR,EVENT,GENADD,ID_EVENT_TYPE) SELECT %s, '%s', %s, ID_EVENT_TYPE FROM EVENTS WHERE EVENT_NAME='%s'",year,
				m_pszAllEvents[0][i],str2,m_pszAllEvents[0][i]);
#endif
			db.ExeSql(str);

			str.Replace(L"PLAN_EVENT",L"FACT_EVENT");
			
			db.ExeSql(str);
			//1 Определим едииницу измерения


#ifdef CONVERTING_ALIAS
			str = L"SELECT MEASUARMENTS.MESUAR_NAME FROM (MEASUARMENTS INNER JOIN EVENTS ON MEASUARMENTS.ID_MESUAR = EVENTS.ID_MESUAR) WHERE EVENTS.EVENT_NAME = '"+db.Aliases(m_pszAllEvents[0][i],ALIAS_TO_NAMES)+L"'";
#else
			str = L"SELECT MEASUARMENTS.MESUAR_NAME FROM (MEASUARMENTS INNER JOIN EVENTS ON MEASUARMENTS.ID_MESUAR = EVENTS.ID_MESUAR) WHERE EVENTS.EVENT_NAME = '"+m_pszAllEvents[0][i]+L"'";
#endif
			
			db.fQueryRead(str,crq);

			mesuar = crq.m_RetValues[0][0];
			
			//2.Вставить строки для каждого месяца для каждого мероприятия
			for(int imonth=0;imonth<12;imonth++)
			{
				if(((imonth+1)>=1)&&((imonth+1)<=3)) 
				{
					counter_quart =1;
					str3.Format(L"%d",counter_quart);
				}
				if(((imonth+1)>3)&&((imonth+1)<=6)) 
				{
					counter_quart =2;
					str3.Format(L"%d",counter_quart);
				}
				if(((imonth+1)>6)&&((imonth+1)<=9)) 
				{
					counter_quart =3;
					str3.Format(L"%d",counter_quart);
				}
				if(((imonth+1)>9)&&((imonth+1)<=12)) 
				{
					counter_quart =4;
					str3.Format(L"%d",counter_quart);
				}

				str4.Format(L"%d",(imonth+1));

#ifdef CONVERTING_ALIAS
				str = L"INSERT INTO PLAN_INFO (NYEAR,NQUART,NMONTH,EVENT,MESUAR,SAVEDS,ECONOMY,CURCOSTS,CURTIME,GENADD) VALUES ("+
					year+
					","+
					str3+
					","+
					str4+
					L",'"+
					db.Aliases(m_pszAllEvents[0][i],ALIAS_TO_NAMES)+
					L"','"+
					mesuar+
					"',"+
					"0," +
					"0,"+
					"0,"+
					"0,"+
					str2+
					L")";
				//AfxMessageBox(str);
					db.ExeSql(str);

					str = L"INSERT INTO GENERAL_INFO (NYEAR,NQUART,NMONTH,EVENT,MESUAR,SAVEDS,ECONOMY,CURCOSTS,CURTIME,GENADD) VALUES ("+
					year+
					","+
					str3+
					","+
					str4+
					L",'"+
					db.Aliases(m_pszAllEvents[0][i],ALIAS_TO_NAMES)+
					L"','"+
					mesuar+
					"',"+
					"0," +
					"0,"+
					"0,"+
					"0,"+
					str2+
					L")";
					db.ExeSql(str);
#else
				str = L"INSERT INTO PLAN_INFO (NYEAR,NQUART,NMONTH,EVENT,MESUAR,SAVEDS,ECONOMY,CURCOSTS,CURTIME,GENADD) VALUES ("+
					year+
					","+
					str3+
					","+
					str4+
					L",'"+
					m_pszAllEvents[0][i]+
					L"','"+
					mesuar+
					"',"+
					"0," +
					"0,"+
					"0,"+
					"0,"+
					str2+
					L")";
				//AfxMessageBox(str);
					db.ExeSql(str);

					str = L"INSERT INTO GENERAL_INFO (NYEAR,NQUART,NMONTH,EVENT,MESUAR,SAVEDS,ECONOMY,CURCOSTS,CURTIME,GENADD) VALUES ("+
					year+
					","+
					str3+
					","+
					str4+
					L",'"+
					m_pszAllEvents[0][i]+
					L"','"+
					mesuar+
					"',"+
					"0," +
					"0,"+
					"0,"+
					"0,"+
					str2+
					L")";
					db.ExeSql(str);
#endif
			}
			
		}
	}
	
	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	CDialog::OnOK();
}
