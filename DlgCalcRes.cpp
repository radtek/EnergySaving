// DlgCalcRes.cpp: файл реализации
//

#include "stdafx.h"
#include "MFCApplication30.h"
#include "DlgCalcRes.h"
#include "afxdialogex.h"
#include "Data.h"


// диалоговое окно CDlgCalcRes

IMPLEMENT_DYNAMIC(CDlgCalcRes, CDialog)


CDlgCalcRes* pDlgCalcres;

CDlgCalcRes::CDlgCalcRes(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCalcRes::IDD, pParent)
	, cx(0)
	, cy(0)
	, m_dblTCOST(-1)
	, m_dblTSAVEDS(-1)
	, m_dblTTIME(-1)
	, m_dblTECONOMY(-1)
{
	pDlgCalcres=this;
}

extern CData db;

CDlgCalcRes::~CDlgCalcRes()
{
}

void CDlgCalcRes::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list2);
}

BEGIN_MESSAGE_MAP(CDlgCalcRes, CDialog)
	ON_MESSAGE(WM_UPDATERESULT, &CDlgCalcRes::OnUpdateresult)
	ON_BN_CLICKED(IDOK, &CDlgCalcRes::OnBnClickedOk)
END_MESSAGE_MAP()

// обработчики сообщений CDlgCalcRes

bool CDlgCalcRes::SetStaticText(void)
{
	this->SetWindowTextW(db.m_szEventName);
	CStatic* buf=(CStatic*) (GetDlgItem(IDC_STATIC_HEADERPERIOD));
	buf->SetWindowTextW(L"Период: ");
	buf =(CStatic*) (GetDlgItem(IDC_STATIC_HEADEROBJECT));
	buf->SetWindowTextW(L"Объект: ");
	buf =(CStatic*) (GetDlgItem(IDC_STATIC_OBJECT));
	buf->SetWindowTextW(m_szObject);
	buf =(CStatic*) (GetDlgItem(IDC_STATIC_PERIOD));
	buf->SetWindowTextW(m_szPeriod);
	buf =(CStatic*) (GetDlgItem(IDC_STATIC_INITDATA));
	buf->SetWindowTextW(L"Исходные данные");

	buf=NULL;
	delete buf;

	return false;
}

BOOL CDlgCalcRes::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию

	// Задает значок для этого диалогового окна. Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым

	CString szbuffer;

	m_szNameWindow=db.m_szEventName;

	//Настройка текстов
	CRect rect=NULL;
	CRect oldrect=NULL;

	this->GetWindowRect(rect);
	
	ScreenToClient(rect);
	
	CStatic* ptext = (CStatic*) this->GetDlgItem(IDC_STATIC_PERIOD);
	//ptext->SetWindowTextW(db.m_szEventName);
	
	ptext->GetWindowRect(oldrect);
	ScreenToClient(oldrect);
	ptext->MoveWindow(oldrect.left,oldrect.top,rect.Width()-oldrect.left,oldrect.Height());
	//
	ptext = (CStatic*) this->GetDlgItem(IDC_STATIC_INITDATA);
	//ptext->SetWindowTextW(db.m_szEventName);
	
	ptext->GetWindowRect(oldrect);
	ScreenToClient(oldrect);
	ptext->MoveWindow(oldrect.left,oldrect.top,rect.Width()-oldrect.left,oldrect.Height());
	
	ptext = (CStatic*) this->GetDlgItem(IDC_STATIC_OBJECT);
	//ptext->SetWindowTextW(db.m_szEventName);
	
	ptext->GetWindowRect(oldrect);
	ScreenToClient(oldrect);
	ptext->MoveWindow(oldrect.left,oldrect.top,rect.Width()-oldrect.left,oldrect.Height());


	szbuffer.Format(L"%s %s год",db.m_szQuart, db.m_szYear);

	if(db.m_iStatus==FACT)
	{
		szbuffer.Format(L"%s %s %s год",db.m_szMonth,db.m_szQuart, db.m_szYear);
	}

	m_szPeriod=szbuffer;

	m_szObject=db.m_szCurObject;

	m_inumEvent=EventMatchFunction();

	if(!m_inumEvent)
	{
		//DestroyWindow();
		AfxMessageBox(L"Такого мероприятия нет!!!");
	}
	
	//1 НАСТРОЙКА СТАТИЧЕСКОГО ТЕКСТА
	SetStaticText();

	//2 НАСТРОЙКА LISTCTRL-ов
	m_list1=new CListCtrlObj;
	m_list1->Create(WS_VISIBLE|WS_BORDER|LVS_REPORT|LVS_EDITLABELS,CRect(10,10,300,300),this,IDC_LIST1);
	
	DWORD oldStyle=GetWindowLong(m_list1->m_hWnd,GWL_EXSTYLE);
	m_list1->SetExtendedStyle(oldStyle|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	
	cx=GetSystemMetrics(SM_CXSCREEN);
	cy=GetSystemMetrics(SM_CYSCREEN);

	MoveWindow(0,0,cx,cy);
	
	CRect r;
	
	this->GetClientRect(r);
	
	m_list1->MoveWindow(10,10,r.Width()-20,r.Height()-20);

	DWORD oldStlList2=GetWindowLong(m_list2.m_hWnd,GWL_EXSTYLE);
	m_list2.SetExtendedStyle(LVS_EX_GRIDLINES);

	SetItemsList1();
	
	ptext=NULL;
	delete ptext;

	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}

bool CDlgCalcRes::SetItemsList1(void)
{
	//AfxMessageBox(L"SetItemList1");
	CRect rect;
	CRect rect2;
	CStatic* pstext;
	CRetQuery crq;
//	int bottomRow;
	CString s;
	int count=0;
	CString ID_INFO;
	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		s.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		s.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		s.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') AS T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		s.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') AS T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
#endif
	}
	
	db.fQueryRead(s,crq);

	ID_INFO=crq.m_RetValues[0][0];
	
	//AfxMessageBox(ID_INFO);

	switch(m_inumEvent)
	{
	case 100:
		//AfxMessageBox(L"100");
		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,250);
		m_list1->InsertColumn(1,L"Е.И."	   ,LVCFMT_LEFT,115);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,110);

		s.Format(
			L"SELECT " 
			L"KUTE, PUT, N, PRICE, COSTS, CURTIME, RESULT, ECONOMY FROM 100 "
			L"WHERE ID_PLAN_INFO=%s",
			ID_INFO);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 100",L"FROM 100F");
		}

		db.fQueryRead(s,crq);

		count=crq.m_lRecCount;

		m_list1->InsertItem(0,L"Коэффициент эффективности промывки"		  ,0); 
		m_list1->InsertItem(1,L"Производительность секций утилизаторов"	  ,0); 
		m_list1->InsertItem(2,L"Продолжительность периода"				  ,0); 
		m_list1->InsertItem(3,L"Стоимость 1 Гкал"						  ,0);  
		m_list1->InsertItem(4,L"Затраты на выполненное мероприятие"		  ,0);  
		m_list1->InsertItem(5,L"Время работы после проведения мероприятия",0);

		m_list1->SetItemText(0,1,L"-"	    ); 
		m_list1->SetItemText(1,1,L"Гкал/ч"	); 
		m_list1->SetItemText(2,1,L"ч"		); 
		m_list1->SetItemText(3,1,L"тыс.руб."); 
		m_list1->SetItemText(4,1,L"тыс.руб."); 
		m_list1->SetItemText(5,1,L"ч."); 


		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0]:L"0"));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1]:L"0"));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2]:L"0"));
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3]:L"0"));
		m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][4]:L"0"));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][4]:L"0"));

		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,300);
		m_list2.InsertColumn(1,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(2,L"Значение",LVCFMT_LEFT,115);

		m_list2.InsertItem(0,L"Фактическая экономия тепла	",0);
		m_list2.InsertItem(1,L"Экономия в денежном выражении",0);
	
		m_list2.SetItemText(0,1,L"Гкал"	   ); m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][5]:L"0"	));
		m_list2.SetItemText(1,1,L"тыс.руб."); m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][6]:L"0"));

		e100();
		break;
	case 1:
#pragma region 1
		m_list1->InsertColumn(0,L" ",LVCFMT_LEFT,150);
		m_list1->InsertColumn(1,L"№ГПА",LVCFMT_LEFT,70);
		m_list1->InsertColumn(2,L"Расход Т.Г., м3/ч",LVCFMT_LEFT,115);
		m_list1->InsertColumn(3,L"Время работы, ч",LVCFMT_LEFT,110);

		s.Format(
		L"SELECT "
		L"Q1b, Q2b, Q3b, Q4b, Q5b, Q6b, Q7b, Q8b, "
		L"Q1a, Q2a, Q3a, Q4a, Q5a, Q6a, Q7a, Q8a, "
		L"T1b, T2b, T3b, T4b, T5b, T6b, T7b, T8b, "
		L"T1a, T2a, T3a, T4a, T5a, T6a, T7a, T8a, "
		L"PRICE, COSTS, dQ0, dQ1, RESULT, ECONOMY, CURTIME FROM 1 WHERE ID_PLAN_INFO=%s",
		ID_INFO);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 1",L"FROM 1F");
		}
		
		db.fQueryRead(s,crq);

		count=crq.m_lRecCount;

		m_list1->InsertItem(0,L"До оптимизации",0);
		m_list1->InsertItem(1,L"ГПА ст.№",0); 
		m_list1->InsertItem(2,L"ГПА ст.№",0); 
		m_list1->InsertItem(3,L"ГПА ст.№",0); 
		m_list1->InsertItem(4,L"ГПА ст.№",0); 
		m_list1->InsertItem(5,L"ГПА ст.№",0); 
		m_list1->InsertItem(6,L"ГПА ст.№",0);
		m_list1->InsertItem(7,L"ГПА ст.№",0); 
		m_list1->InsertItem(8,L"ГПА ст.№",0); 
		m_list1->InsertItem(9,L"После оптимизации",0);
		m_list1->InsertItem(10,L"ГПА ст.№",0); 
		m_list1->InsertItem(11,L"ГПА ст.№",0); 
		m_list1->InsertItem(12,L"ГПА ст.№",0); 
		m_list1->InsertItem(13,L"ГПА ст.№",0); 
		m_list1->InsertItem(14,L"ГПА ст.№",0); 
		m_list1->InsertItem(15,L"ГПА ст.№",0); 
		m_list1->InsertItem(16,L"ГПА ст.№",0); 
		m_list1->InsertItem(17,L"ГПА ст.№",0); 
		m_list1->InsertItem(18,L"Стоимость 1 тыс.м3 газа",0); 
		m_list1->InsertItem(19,L"Затраты",0); 
		m_list1->InsertItem(20,L"Время работы в ОП",0); 

		m_list1->SetItemText(1,1,L"21"); m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][0]:L"2700")); m_list1->SetItemText(1,3,((count!=0)?crq.m_RetValues[0][16]:L"450"));
		m_list1->SetItemText(2,1,L"22"); m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][1]:L"2600")); m_list1->SetItemText(2,3,((count!=0)?crq.m_RetValues[0][17]:L"450"));
		m_list1->SetItemText(3,1,L"23"); m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][2]:L"2700")); m_list1->SetItemText(3,3,((count!=0)?crq.m_RetValues[0][18]:L"450"));
		m_list1->SetItemText(4,1,L"24"); m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][3]:L"2800")); m_list1->SetItemText(4,3,((count!=0)?crq.m_RetValues[0][19]:L"440"));
		m_list1->SetItemText(5,1,L"25"); m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][4]:L"2750")); m_list1->SetItemText(5,3,((count!=0)?crq.m_RetValues[0][20]:L"430"));
	    m_list1->SetItemText(6,1,L"26"); m_list1->SetItemText(6,2,((count!=0)?crq.m_RetValues[0][5]:L"2650")); m_list1->SetItemText(6,3,((count!=0)?crq.m_RetValues[0][21]:L"460"));
		m_list1->SetItemText(7,1,L"27"); m_list1->SetItemText(7,2,((count!=0)?crq.m_RetValues[0][6]:L"2730")); m_list1->SetItemText(7,3,((count!=0)?crq.m_RetValues[0][22]:L"475"));
		m_list1->SetItemText(8,1,L"28"); m_list1->SetItemText(8,2,((count!=0)?crq.m_RetValues[0][7]:L"2710")); m_list1->SetItemText(8,3,((count!=0)?crq.m_RetValues[0][23]:L"420"));
		
		m_list1->SetItemText(10,1,L"21"); m_list1->SetItemText(10,2,((count!=0)?crq.m_RetValues[0][8]:L"2700")); m_list1->SetItemText(10,3,((count!=0)?crq.m_RetValues[0][24]:L"450"));
		m_list1->SetItemText(11,1,L"22"); m_list1->SetItemText(11,2,((count!=0)?crq.m_RetValues[0][9]:L"2700")); m_list1->SetItemText(11,3,((count!=0)?crq.m_RetValues[0][25]:L"450"));
		m_list1->SetItemText(12,1,L"23"); m_list1->SetItemText(12,2,((count!=0)?crq.m_RetValues[0][10]:L"2700")); m_list1->SetItemText(12,3,((count!=0)?crq.m_RetValues[0][26]:L"450"));
		m_list1->SetItemText(13,1,L"24"); m_list1->SetItemText(13,2,((count!=0)?crq.m_RetValues[0][11]:L"2700")); m_list1->SetItemText(13,3,((count!=0)?crq.m_RetValues[0][27]:L"450"));
		m_list1->SetItemText(14,1,L"25"); m_list1->SetItemText(14,2,((count!=0)?crq.m_RetValues[0][12]:L"2700")); m_list1->SetItemText(14,3,((count!=0)?crq.m_RetValues[0][28]:L"450"));
		m_list1->SetItemText(15,1,L"26"); m_list1->SetItemText(15,2,((count!=0)?crq.m_RetValues[0][13]:L"2700")); m_list1->SetItemText(15,3,((count!=0)?crq.m_RetValues[0][29]:L"450"));
		m_list1->SetItemText(16,1,L"27"); m_list1->SetItemText(16,2,((count!=0)?crq.m_RetValues[0][14]:L"2700")); m_list1->SetItemText(16,3,((count!=0)?crq.m_RetValues[0][30]:L"450"));
		m_list1->SetItemText(17,1,L"28"); m_list1->SetItemText(17,2,((count!=0)?crq.m_RetValues[0][15]:L"2700")); m_list1->SetItemText(17,3,((count!=0)?crq.m_RetValues[0][31]:L"450"));

		m_list1->SetItemText(18,1,L" "); m_list1->SetItemText(18,2,L"тыс.руб"); m_list1->SetItemText(18,3,((count!=0)?crq.m_RetValues[0][32]:L"3799,0"));
		m_list1->SetItemText(19,1,L" "); m_list1->SetItemText(19,2,L"тыс.руб"); m_list1->SetItemText(19,3,((count!=0)?crq.m_RetValues[0][33]:L"0")	   );
		m_list1->SetItemText(20,1,L" "); m_list1->SetItemText(20,2,L"ч"		 ); m_list1->SetItemText(20,3,L"744"									   );
		

		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,250);
		m_list2.InsertColumn(1,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(2,L"Значение",LVCFMT_LEFT,115);

		m_list2.InsertItem(0,L"Расход ТГ до опт."	,0);
		m_list2.InsertItem(1,L"Расход ТГ после опт.",0);
		m_list2.InsertItem(2,L"Экономия газа"		,0);
		m_list2.InsertItem(3,L"Экономия (денеж.)"	,0);
	
		
		m_list2.SetItemText(0,1,L"тыс.м3"); m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][34]:L"0"));
		m_list2.SetItemText(1,1,L"тыс.м3"); m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][35]:L"0"));
		m_list2.SetItemText(2,1,L"тыс.м3"); m_list2.SetItemText(2,2,((count!=0)?crq.m_RetValues[0][36]:L"0"));
		m_list2.SetItemText(3,1,L"тыс.руб."); m_list2.SetItemText(3,2,((count!=0)?crq.m_RetValues[0][37]:L"0"));
		
		e1();
#pragma endregion 1
		break;
#pragma region 2
	case 2:
		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,250);
		m_list1->InsertColumn(1,L"Е.И."	   ,LVCFMT_LEFT,115);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,110);

		s.Format(
			L"SELECT " 
			L"PINIT, PLIM, TINIT, TLIM, P0,"
			L"DIN, L, PRICE, COSTS, CURTIME, DV, VGP,"
			L"ZINIT, ZLIM, RESULT, ECONOMY "
			L"FROM 2 "
			L"WHERE ID_PLAN_INFO=%s",
			ID_INFO);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 2",L"FROM 2F");
		}

		db.fQueryRead(s,crq);

		count=crq.m_lRecCount;

		m_list1->InsertItem(0,L"Рабс газа в трубопроводе до выработки"	 ,0); 
		m_list1->InsertItem(1,L"Рабс газа в трубопроводе после выработки",0); 
		m_list1->InsertItem(2,L"Т газа в трубопроводе до выработки"      ,0); 
		m_list1->InsertItem(3,L"Т газа в трубопроводе после выработки"	 ,0);  
		m_list1->InsertItem(4,L"Плотность прир.газа при стнд.условиях"	 ,0);  
		m_list1->InsertItem(5,L"Внутренний диаметр трубопровода"		 ,0);  
		m_list1->InsertItem(6,L"Длина участка трубопровода"				 ,0);
		m_list1->InsertItem(7,L"Стоимость 1 тыс.м3 газа"				 ,0);  
		m_list1->InsertItem(8,L"Затраты"								 ,0); 
		m_list1->InsertItem(9,L"Время работы после мероприятия "	     ,0);  

		m_list1->SetItemText(0,1,L"МПа"	    ); 
		m_list1->SetItemText(1,1,L"МПа"	    ); 
		m_list1->SetItemText(2,1,L"К"		); 
		m_list1->SetItemText(3,1,L"К"	    ); 
		m_list1->SetItemText(4,1,L"кг/м3"	); 
		m_list1->SetItemText(5,1,L"м"		); 
		m_list1->SetItemText(6,1,L"м"	    ); 
		m_list1->SetItemText(7,1,L"тыс.руб."); 
		m_list1->SetItemText(8,1,L"тыс.руб.");  
		m_list1->SetItemText(9,1,L"ч"		); 

		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0]:L"5,30"));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1]:L"1,20"));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2]:L"285,0"));
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3]:L"285,0"));
		m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][4]:L"0,68"));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][5]:L"1,20"));
		m_list1->SetItemText(6,2,((count!=0)?crq.m_RetValues[0][6]:L"9520,0"));
		m_list1->SetItemText(7,2,((count!=0)?crq.m_RetValues[0][7]:L"3947"));
		m_list1->SetItemText(8,2,((count!=0)?crq.m_RetValues[0][8]:L"0"));
		m_list1->SetItemText(9,2,((count!=0)?crq.m_RetValues[0][9]:L"744"));

		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,300);
		m_list2.InsertColumn(2,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(3,L"Значение",LVCFMT_LEFT,115);

		m_list2.InsertItem(0,L"Относительная плотность газа по воздуху",0);
		m_list2.InsertItem(1,L"Геометрический участок газопровода"	   ,0);
		m_list2.InsertItem(2,L"Коэффициент сжимаемости газа при Рн"	   ,0);
		m_list2.InsertItem(3,L"Коэффициент сжимаемости газа при Рк"	   ,0);
		m_list2.InsertItem(4,L"Экономия"							   ,0);
		m_list2.InsertItem(5,L"Экономия (денеж.)"					   ,0);
	
		m_list2.SetItemText(0,1,L"-"	  ); m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][10]:L"0,57"));
		m_list2.SetItemText(1,1,L"м3"	  ); m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][11]:L"10761,41"));
		m_list2.SetItemText(2,1,L"-"	  ); m_list2.SetItemText(2,2,((count!=0)?crq.m_RetValues[0][12]:L"0,89"));
		m_list2.SetItemText(3,1,L"-"	  ); m_list2.SetItemText(3,2,((count!=0)?crq.m_RetValues[0][13]:L"0,97"));
		m_list2.SetItemText(4,1,L"тыс.м3" ); m_list2.SetItemText(4,2,((count!=0)?crq.m_RetValues[0][14]:L"516,52"));
		m_list2.SetItemText(5,1,L"тыс.руб"); m_list2.SetItemText(4,2,((count!=0)?crq.m_RetValues[0][14]:L"2038,70"));

		e2();
		
		break;

#pragma endregion 2
	case 4:

		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,400);
		m_list1->InsertColumn(1,L"Е.И."	   ,LVCFMT_LEFT,115);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,110);

		s.Format(
			L"SELECT " 
			L"P0, P1, T0, T1,"
			L"Q0,DIN,L,PRICE,COSTS,"
			L"CURTIME,DQ,V,Z0,Z1,RESULT,"
			L"ECONOMY "
			L"FROM 4 "
			L"WHERE ID_PLAN_INFO=%s",
			ID_INFO);

		

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 4",L"FROM 4F");
		}
		//AfxMessageBox(s);
		db.fQueryRead(s,crq);

		//AfxMessageBox(L"!!!");

		count=crq.m_lRecCount;

		

		m_list1->InsertItem(0,L"Р газа в трубопроводе до предвар.снижения в рем.участке"   ,0); 
		m_list1->InsertItem(1,L"P газа в трубопроводе после предвар.снижения в рем.участке",0); 
		m_list1->InsertItem(2,L"Т газа в трубопроводе до предвар.снижения в рем.участке"   ,0);  
		m_list1->InsertItem(3,L"Т газа в трубопроводе после предвар.снижения в рем.участке",0);  
		m_list1->InsertItem(4,L"Плотность прир.газа при стд.условиях"					   ,0);  
		m_list1->InsertItem(5,L"Внутренний диаметр трубопровода"						   ,0);
		m_list1->InsertItem(6,L"Длина участка трубопровода"								   ,0);
		m_list1->InsertItem(7,L"Стоимость 1 тыс.м3 газа"								   ,0);  
		m_list1->InsertItem(8,L"Затраты"												   ,0); 
		m_list1->InsertItem(9,L"Время работы после мероприятия"							   ,0);  

		m_list1->SetItemText(0,1,L"МПа"	    ); 
		m_list1->SetItemText(1,1,L"МПа"	    ); 
		m_list1->SetItemText(2,1,L"К"		); 
		m_list1->SetItemText(3,1,L"К"	    ); 
		m_list1->SetItemText(4,1,L"кг/м3"	); 
		m_list1->SetItemText(5,1,L"м"		); 
		m_list1->SetItemText(6,1,L"м"	    ); 
		m_list1->SetItemText(7,1,L"тыс.руб."); 
		m_list1->SetItemText(8,1,L"тыс.руб.");  
		m_list1->SetItemText(9,1,L"ч"		); 

		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0]:L"5.60"));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1]:L"4.10"));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2]:L"283.00"));
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3]:L"283.00"));
		m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][4]:L"0.684"));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][5]:L"1.40"));
		m_list1->SetItemText(6,2,((count!=0)?crq.m_RetValues[0][6]:L"5000.00"));
		m_list1->SetItemText(7,2,((count!=0)?crq.m_RetValues[0][7]:L"3947"));
		m_list1->SetItemText(8,2,((count!=0)?crq.m_RetValues[0][8]:L"0"));
		m_list1->SetItemText(9,2,((count!=0)?crq.m_RetValues[0][9]:L"744"));

		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,300);
		m_list2.InsertColumn(2,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(3,L"Значение",LVCFMT_LEFT,115);

		m_list2.InsertItem(0,L"Относительная плотность газа по воздуху",0);
		m_list2.InsertItem(1,L"Геометрический участок газопровода"	   ,0);
		m_list2.InsertItem(2,L"Коэффициент сжимаемости газа при Рн"	   ,0);
		m_list2.InsertItem(3,L"Коэффициент сжимаемости газа при Рк"	   ,0);
		m_list2.InsertItem(4,L"Экономия"							   ,0);
		m_list2.InsertItem(5,L"Экономия (денеж.)"					   ,0);
	
		m_list2.SetItemText(0,1,L"-"	  ); m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][10]:L"0,57"));
		m_list2.SetItemText(1,1,L"м3"	  ); m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][11]:L"7693,00"));
		m_list2.SetItemText(2,1,L"-"	  ); m_list2.SetItemText(2,2,((count!=0)?crq.m_RetValues[0][12]:L"0,88"));
		m_list2.SetItemText(3,1,L"-"	  ); m_list2.SetItemText(3,2,((count!=0)?crq.m_RetValues[0][13]:L"0,91"));
		m_list2.SetItemText(4,1,L"тыс.м3" ); m_list2.SetItemText(4,2,((count!=0)?crq.m_RetValues[0][14]:L"146,25"));
		m_list2.SetItemText(5,1,L"тыс.руб"); m_list2.SetItemText(5,2,((count!=0)?crq.m_RetValues[0][15]:L"555,62"));

		e4();

		break;
	case 7:
#pragma region 7
		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,350);
		m_list1->InsertColumn(1,L"Е.И."	   ,LVCFMT_LEFT,115);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,110);

		s.Format(
			L"SELECT NMONTH,NPROM,KLOAD,N,Kdepression,Q,COUNT,PRICE,COSTS,CURTIME,KECTG,L,RESULT,ECONOMY "
			L"FROM 7 "
			L"WHERE ID_PLAN_INFO=%s",
			ID_INFO);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 7",L"FROM 7F");
		}

		db.fQueryRead(s,crq);

		count=crq.m_lRecCount;

		m_list1->InsertItem(0,L"Наработка ГПА за текущий месяц"				,0); 
		m_list1->InsertItem(1,L"Наработка ГПА после промывки"				,0); 
		m_list1->InsertItem(2,L"Коэффициент загрузки ГПА (данные ДС)"		,0);  
		m_list1->InsertItem(3,L"Номинальная мощность ГПА"					,0);  
		m_list1->InsertItem(4,L"Снижение удельного расхода ТГ"				,0);  
		m_list1->InsertItem(5,L"Плотность природного газа при стд. условиях",0);  
		m_list1->InsertItem(6,L"Количество ГПА"								,0); 
		m_list1->InsertItem(7,L"Стоимость 1 тыс.м3 газа"				    ,0);  
		m_list1->InsertItem(8,L"Затраты на выполнение мероприятия"	   	    ,0); 
		m_list1->InsertItem(9,L"Время работы в отчетном периоде"			,0);

		m_list1->SetItemText(0,1,L"ч"	      ); 
		m_list1->SetItemText(1,1,L"ч"	      ); 
		m_list1->SetItemText(2,1,L"-"		  ); 
		m_list1->SetItemText(3,1,L"кВт"		  ); 
		m_list1->SetItemText(4,1,L"кг/кВт х ч"); 
		m_list1->SetItemText(5,1,L"кг/м3"	  ); 
		m_list1->SetItemText(6,1,L"шт."		  ); 
		m_list1->SetItemText(7,1,L"тыс.руб."  );  
		m_list1->SetItemText(8,1,L"тыс.руб."  ); 
		m_list1->SetItemText(9,1,L"ч"		  );

		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0]:L"0"));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1]:L"0"));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2]:L"0"));
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3]:L"0"));
		m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][4]:L"0"));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][5]:L"0"));
		m_list1->SetItemText(6,2,((count!=0)?crq.m_RetValues[0][6]:L"0"));
		m_list1->SetItemText(7,2,((count!=0)?crq.m_RetValues[0][7]:L"0"));
		m_list1->SetItemText(8,2,((count!=0)?crq.m_RetValues[0][8]:L"0"));
		m_list1->SetItemText(9,2,((count!=0)?crq.m_RetValues[0][9]:L"0"));

		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,350);
		m_list2.InsertColumn(2,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(3,L"Значение",LVCFMT_LEFT,115);

		m_list2.InsertItem(0,L"Коэффициент экономии ТГ в зависимости от наработки ГПА",0);
		m_list2.InsertItem(1,L"Средняя загрзука ГПА по мощности"					  ,0);
		m_list2.InsertItem(2,L"Фактическая экономия газа"							  ,0);
		m_list2.InsertItem(3,L"Экономия (денеж.)"									  ,0);
	
		m_list2.SetItemText(0,1,L"-"	   ); m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][10]:L"0"));
		m_list2.SetItemText(1,1,L"кВт"	   ); m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][11]:L"0"));
		m_list2.SetItemText(2,1,L"тыс.м3"  ); m_list2.SetItemText(2,2,((count!=0)?crq.m_RetValues[0][12]:L"0"));
		m_list2.SetItemText(3,1,L"тыс.руб."); m_list2.SetItemText(3,2,((count!=0)?crq.m_RetValues[0][13]:L"0"));

		e7();

#pragma endregion 7
		break;
	case 8:
#pragma region 8
		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,230);
		m_list1->InsertColumn(1,L"Е.И.",LVCFMT_LEFT,115);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,110);

		s.Format(
			L"SELECT "
			L"NMONTH, NREPAIR, KLOAD, NOM, Kdepression, QP, PRICE, COSTS, CURTIME, KECTG, LOAD, RESULT, ECONOMY "
			L"FROM 8 "
			L"WHERE ID_PLAN_INFO=%s",
			ID_INFO);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"FROM 8",L"FROM 8F");
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
		}

		db.fQueryRead(s,crq);

		count=crq.m_lRecCount;

		m_list1->InsertItem(0,L"Наработка ГПА за текущий месяц"					   ,0); 
		m_list1->InsertItem(1,L"Наработка ГПА после ремонта"					   ,0); 
		m_list1->InsertItem(2,L"Коэффициент загрузки ГПА"						   ,0);
		m_list1->InsertItem(3,L"Номинальная мощность ГПА"						   ,0);
		m_list1->InsertItem(4,L"Снижение удельного расхода ТГ"					   ,0); 
		m_list1->InsertItem(5,L"Плотность природного газа"						   ,0); 
		m_list1->InsertItem(6,L"Стоимость 1 тыс.м3 газа"						   ,0); 
		m_list1->InsertItem(7,L"Затраты"										   ,0); 
		m_list1->InsertItem(8,L"Время работы в отчетном периоде"   			       ,0); 

		m_list1->SetItemText(0,1,L"ч"		  ); 
		m_list1->SetItemText(1,1,L"ч"		  ); 
		m_list1->SetItemText(2,1,L"-"		  );
		m_list1->SetItemText(3,1,L"кВт"		  ); 
		m_list1->SetItemText(4,1,L"кг/кВт х ч"); 
		m_list1->SetItemText(5,1,L"кг/м3"     ); 
		m_list1->SetItemText(6,1,L"тыс.руб."  ); 
		m_list1->SetItemText(7,1,L"тыс.руб."  ); 
		m_list1->SetItemText(8,1,L"ч"		  ); 

		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0]:L"0"));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1]:L"0"));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2]:L"0"));
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3]:L"0"));
		m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][4]:L"0"));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][5]:L"0"));
		m_list1->SetItemText(6,2,((count!=0)?crq.m_RetValues[0][6]:L"0"));
		m_list1->SetItemText(7,2,((count!=0)?crq.m_RetValues[0][7]:L"0"));
		m_list1->SetItemText(8,2,((count!=0)?crq.m_RetValues[0][8]:L"0"));

		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,250);
		m_list2.InsertColumn(1,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(2,L"Значение",LVCFMT_LEFT,115);

		m_list2.InsertItem(0,L"К экономии ТГ в зависимости от наработки",0); m_list2.SetItemText(0,1,L"-"		); 
		m_list2.InsertItem(1,L"Средняя загрузка ГПА по мощности"		,0); m_list2.SetItemText(1,1,L"кВт"		); 
		m_list2.InsertItem(2,L"Фактическая экономия газа"				,0); m_list2.SetItemText(2,1,L"тыс.м3"  ); 
		m_list2.InsertItem(3,L"Экономия (денеж.)"						,0); m_list2.SetItemText(3,1,L"тыс.руб."); 

		m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][7]:L"0"));
		m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][8]:L"0"));
		m_list2.SetItemText(2,2,((count!=0)?crq.m_RetValues[0][9]:L"0"));
		m_list2.SetItemText(3,2,((count!=0)?crq.m_RetValues[0][10]:L"0"));

		e8();
#pragma endregion 8
		break;
	case 12:
#pragma region 12
		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,300);
		m_list1->InsertColumn(1,L"Е.И."	   ,LVCFMT_LEFT,115);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,110);
		
		s.Format(
		L"SELECT "
		L"T,P,P0,P1,T0,T1,QP,V1,V2,V3,n,PRICE,COSTS,CURTIME,"
		L"DQP,TSR0,TSR1,PSR0,PSR1,Z0,Z1,Q0,Q1,Q01,Q11,RESULT,ECONOMY,ID_PLAN_INFO FROM 12 "
		L"WHERE ID_PLAN_INFO=%s",
		ID_INFO);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 12",L"FROM 12F");
		}

		db.fQueryRead(s,crq);

		count=crq.m_lRecCount;

		m_list1->InsertItem(0,  L"Т газа при стандартных условиях"								   ,0);	m_list1->SetItemText(0,1, L"град.Ц."  ); 
		m_list1->InsertItem(1,  L"P газа при стандартных условиях"								   ,0);	m_list1->SetItemText(1,1, L"МПа"	  ); 
		m_list1->InsertItem(2,  L"P газа в емкости до выравнивания давлений"					   ,0);	m_list1->SetItemText(2,1, L"кгс/см2"  ); 
		m_list1->InsertItem(3,  L"P газа в емкости после выравнивания давлений"					   ,0);	m_list1->SetItemText(3,1, L"кгс/см2"  ); 
		m_list1->InsertItem(4,  L"Tг на входе ГРС"												   ,0);	m_list1->SetItemText(4,1, L"град.Ц."  );  
		m_list1->InsertItem(5,  L"Tг на выходе ГРС"												   ,0);	m_list1->SetItemText(5,1, L"град.Ц."  ); 
		m_list1->InsertItem(6,  L"Плотность природного газа при стд.усл."						   ,0);	m_list1->SetItemText(6,1, L"кг/м3"    ); 
		m_list1->InsertItem(7,  L"V емкости"													   ,0);	m_list1->SetItemText(7,1, L"м3"       );
		m_list1->InsertItem(8,  L"V коммуникаций м/у дрен.краном и емкостью"					   ,0);	m_list1->SetItemText(8,1, L"м3"       );
		m_list1->InsertItem(9,  L"V м/у емкостью и свечным краном"								   ,0);	m_list1->SetItemText(9,1, L"м3"		  );
		m_list1->InsertItem(10, L"Количество продувок"											   ,0);	m_list1->SetItemText(10,1,L"шт"       );
		m_list1->InsertItem(11, L"Цена 1 тыс.м3"												   ,0);	m_list1->SetItemText(11,1,L"тыс.руб"  );
		m_list1->InsertItem(12, L"Затраты"														   ,0);	m_list1->SetItemText(12,1,L"тыс.руб." );
		m_list1->InsertItem(13, L"Время работы"													   ,0);	m_list1->SetItemText(13,1,L"ч"		  ); 

		m_list1->SetItemText(0,2, ((count!=0)?crq.m_RetValues[0][0]: L"1.00"));
		m_list1->SetItemText(1,2, ((count!=0)?crq.m_RetValues[0][1]: L"2.00"));
		m_list1->SetItemText(2,2, ((count!=0)?crq.m_RetValues[0][2]: L"3.00"));
		m_list1->SetItemText(3,2, ((count!=0)?crq.m_RetValues[0][3]: L"4.00"));
		m_list1->SetItemText(4,2, ((count!=0)?crq.m_RetValues[0][4]: L"5.00"));
		m_list1->SetItemText(5,2, ((count!=0)?crq.m_RetValues[0][5]: L"6.00"));
		m_list1->SetItemText(6,2, ((count!=0)?crq.m_RetValues[0][6]: L"7.00"));
		m_list1->SetItemText(7,2, ((count!=0)?crq.m_RetValues[0][7]: L"8.00"));
		m_list1->SetItemText(8,2, ((count!=0)?crq.m_RetValues[0][8]: L"9.00"));
		m_list1->SetItemText(9,2, ((count!=0)?crq.m_RetValues[0][9]: L"10.00"));
		m_list1->SetItemText(10,2,((count!=0)?crq.m_RetValues[0][10]:L"11.00"));
		m_list1->SetItemText(11,2,((count!=0)?crq.m_RetValues[0][11]:L"12.00"));
		m_list1->SetItemText(12,2,((count!=0)?crq.m_RetValues[0][12]:L"13.00"));
		m_list1->SetItemText(13,2,((count!=0)?crq.m_RetValues[0][13]:L"14.00"));

		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,250);
		m_list2.InsertColumn(1,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(2,L"Значение",LVCFMT_LEFT,115);

		m_list2.InsertItem(0,L"Отн. плотность газа по воздуху		"	,0); m_list2.SetItemText(0,1, L"б/р     "); 
		m_list2.InsertItem(1,L"Тср газа в контуре - 1*				"	,0); m_list2.SetItemText(1,1, L"К       "); 
		m_list2.InsertItem(2,L"Тср газа в контуре - 2*				"	,0); m_list2.SetItemText(2,1, L"К       "); 
		m_list2.InsertItem(3,L"Рср газа в контуре - 1*				"	,0); m_list2.SetItemText(3,1, L"МПа     ");
		m_list2.InsertItem(4,L"Рср газа в контуре - 2*				"	,0); m_list2.SetItemText(4,1, L"МПа     "); 
		m_list2.InsertItem(5,L"Коэффициент сжимаемости - 1          " 	,0); m_list2.SetItemText(5,1, L"б/р     ");
		m_list2.InsertItem(6,L"Коэффициент сжимаемости - 2          "	,0); m_list2.SetItemText(6,1, L"б/р     ");
		m_list2.InsertItem(7,L"V стр-го газа до продувки одного ПУ-1"   ,0); m_list2.SetItemText(7,1, L"тыс.м3  ");
		m_list2.InsertItem(8,L"V стр-го газа до продувки одного ПУ-2"   ,0); m_list2.SetItemText(8,1, L"б/р     ");
		m_list2.InsertItem(9,L"Общий объем стравленного газа - 1   "	,0); m_list2.SetItemText(9,1,L"тыс.м3  ");
		m_list2.InsertItem(10,L"Общий объем стравленного газа - 2   "	,0); m_list2.SetItemText(10,1,L"тыс.м3  ");
		m_list2.InsertItem(11,L"Эффект								"	,0); m_list2.SetItemText(11,1,L"тыс.м3  ");
		m_list2.InsertItem(12,L"Эффект (денеж.)	 					"	,0); m_list2.SetItemText(12,1,L"тыс.руб ");

		m_list2.SetItemText(0,2, ((count!=0)?crq.m_RetValues[0][14]:L"0" ));
		m_list2.SetItemText(1,2, ((count!=0)?crq.m_RetValues[0][15]:L"0" ));
		m_list2.SetItemText(2,2, ((count!=0)?crq.m_RetValues[0][16]:L"0" ));
		m_list2.SetItemText(3,2, ((count!=0)?crq.m_RetValues[0][17]:L"0" ));
		m_list2.SetItemText(4,2, ((count!=0)?crq.m_RetValues[0][18]:L"0" ));
		m_list2.SetItemText(5,2, ((count!=0)?crq.m_RetValues[0][19]:L"0" ));
		m_list2.SetItemText(6,2, ((count!=0)?crq.m_RetValues[0][20]:L"0" ));
		m_list2.SetItemText(7,2, ((count!=0)?crq.m_RetValues[0][21]:L"0" ));
		m_list2.SetItemText(8,2, ((count!=0)?crq.m_RetValues[0][22]:L"0" ));
		m_list2.SetItemText(9,2, ((count!=0)?crq.m_RetValues[0][23]:L"0" ));
		m_list2.SetItemText(10,2,((count!=0)?crq.m_RetValues[0][24]:L"0" ));
		m_list2.SetItemText(11,2,((count!=0)?crq.m_RetValues[0][25]:L"0" ));
		m_list2.SetItemText(12,2,((count!=0)?crq.m_RetValues[0][26]:L"0" ));

		e12();
#pragma endregion 12
		break;
	case 15:
#pragma region 15
		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,380);
		m_list1->InsertColumn(1,L"Е.И."	   ,LVCFMT_LEFT,115);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,110);

		m_list1->InsertItem(0,L"Исходное давление газа"									  ,0); m_list1->SetItemText(0,1,L"МПа"		  );
		m_list1->InsertItem(1,L"Исходный диаметр трубы"									  ,0); m_list1->SetItemText(1,1,L"м"		  ); 
		m_list1->InsertItem(2,L"Исходная длина дренажной линии"							  ,0); m_list1->SetItemText(2,1,L"м"		  ); 
		m_list1->InsertItem(3,L"Исходная температура газа в пылеуловителе"				  ,0); m_list1->SetItemText(3,1,L"К"		  ); 
		m_list1->InsertItem(4,L"Газовая постоянная природного газа"						  ,0); m_list1->SetItemText(4,1,L"Дж/(кг х К)"); 
		m_list1->InsertItem(5,L"Коэф. учитывающий местное сопротивление газа или задвижки",0); m_list1->SetItemText(5,1,L"б/р"		  ); 
		m_list1->InsertItem(6,L"Шероховатость трубы"									  ,0); m_list1->SetItemText(6,1,L"мм"		  ); 
		m_list1->InsertItem(7,L"Атмосферное давление"									  ,0); m_list1->SetItemText(7,1,L"МПа"		  );
		m_list1->InsertItem(8,L"Плотность природного газа"								  ,0); m_list1->SetItemText(8,1,L"кг/м3"	  ); 
		m_list1->InsertItem(9,L"Среднее время продувки"									  ,0); m_list1->SetItemText(9,1,L"с"		  ); 
		m_list1->InsertItem(10,L"Количество продуваемых пылеуловителей"					  ,0); m_list1->SetItemText(10,1,L"-"		  ); 
		m_list1->InsertItem(11,L"Нормативное количество продувок в месяц"				  ,0); m_list1->SetItemText(11,1,L"-"		  ); 
		m_list1->InsertItem(12,L"Фактическое количество продувок в месяц"				  ,0); m_list1->SetItemText(12,1,L"-"		  );  
		m_list1->InsertItem(13,L"Стоимость 1 тыс.м3 газа"								  ,0); m_list1->SetItemText(13,1,L"тыс.руб"   ); 
		m_list1->InsertItem(14,L"Затраты"												  ,0); m_list1->SetItemText(14,1,L"тыс.руб."  ); 
		m_list1->InsertItem(15,L"Время работы"											  ,0); m_list1->SetItemText(15,1,L"ч"		  ); 

		
		s.Format(L"SELECT "
		L"P0, D0, L0, T0, G, KRESIST, SHEROH, PATM, PPG, TPROD,"
		L"NEMK, NOMNPROD, FACTNPROD, PRICE, COSTS, CURTIME " 
		L"FROM 15 WHERE ID_PLAN_INFO=%s",ID_INFO);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 15",L"FROM 15F");
		}

		//AfxMessageBox(s);

		db.fQueryRead(s,crq);
		
		count=crq.m_lRecCount;

		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0]: L"1.00"));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1]: L"2.00"));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2]: L"3.00"));
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3]: L"4.00"));
		m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][4]: L"5.00"));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][5]: L"6.00"));
		m_list1->SetItemText(6,2,((count!=0)?crq.m_RetValues[0][6]: L"7.00"));
		m_list1->SetItemText(7,2,((count!=0)?crq.m_RetValues[0][7]: L"8.00"));
		m_list1->SetItemText(8,2,((count!=0)?crq.m_RetValues[0][8]: L"9.00"));
		m_list1->SetItemText(9,2,((count!=0)?crq.m_RetValues[0][9]: L"10.00"));
		m_list1->SetItemText(10,2,((count!=0)?crq.m_RetValues[0][10]: L"11.00"));
		m_list1->SetItemText(11,2,((count!=0)?crq.m_RetValues[0][11]: L"12.00"));
		m_list1->SetItemText(12,2,((count!=0)?crq.m_RetValues[0][12]: L"13.00"));
		m_list1->SetItemText(13,2,((count!=0)?crq.m_RetValues[0][13]: L"14.00"));
		m_list1->SetItemText(14,2,((count!=0)?crq.m_RetValues[0][14]: L"15.00"));
		m_list1->SetItemText(15,2,((count!=0)?crq.m_RetValues[0][15]: L"16.00"));

		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,400);
		m_list2.InsertColumn(1,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(2,L"Значение",LVCFMT_LEFT,115);

		m_list2.InsertItem(0,L"Поперечное сечение дренажной линии"								   ,0); 
		m_list2.InsertItem(1,L"К гидравлического сопротивления дрен.линии"						   ,0); 
		m_list2.InsertItem(2,L"Расход газа для исходных условий"								   ,0); 
		m_list2.InsertItem(3,L"Поправочный коэффициент, учит.отклон. P от исходного"			   ,0); 
		m_list2.InsertItem(4,L"Поправочный коэффициент, учит.отклон. Т от исходного"			   ,0); 
		m_list2.InsertItem(5,L"Поправочный коэффициент, учит.отклон. длины дрен.линии от исходного",0);
		m_list2.InsertItem(6,L"Поправочный коэффициент, учит.отклон. диаметра от исходного"		   ,0); 
		m_list2.InsertItem(7,L"Экономия"														   ,0);
		m_list2.InsertItem(8,L"Экономия (денеж.)"												   ,0);

		e15();
#pragma endregion 15
		break;
	case 17:
		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,380);
		m_list1->InsertColumn(1,L"Е.И."	   ,LVCFMT_LEFT,115);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,110);

		m_list1->InsertItem(0,L"Часовой расход натурального топлива при 'фотографии' М.Г."    ,0); m_list1->SetItemText(0,1,L"нм3/час М.Г.");
		m_list1->InsertItem(1,L"Низшая теплотворная способность топлива М.Г."				  ,0); m_list1->SetItemText(1,1,L"ккал/м3"	   ); 
		m_list1->InsertItem(2,L"КПД (брутто) при 'фотографии' М.Г."							  ,0); m_list1->SetItemText(2,1,L"-"		   ); 
		m_list1->InsertItem(3,L"КПД (брутто) после наладки М.Г."				              ,0); m_list1->SetItemText(3,1,L"-"		   ); 
		m_list1->InsertItem(4,L"Число часов работы котла в год/месяц М.Г."           		  ,0); m_list1->SetItemText(4,1,L"ч"		   ); 
		m_list1->InsertItem(5,L"Часовой расход натурального топлива при 'фотографии' Б.Г."    ,0); m_list1->SetItemText(5,1,L"нм3/час М.Г.");
		m_list1->InsertItem(6,L"Низшая теплотворная способность топлива Б.Г."				  ,0); m_list1->SetItemText(6,1,L"ккал/м3"	   ); 
		m_list1->InsertItem(7,L"КПД (брутто) при 'фотографии' Б.Г."							  ,0); m_list1->SetItemText(7,1,L"-"		   ); 
		m_list1->InsertItem(8,L"КПД (брутто) после наладки Б.Г."				              ,0); m_list1->SetItemText(8,1,L"-"		   ); 
		m_list1->InsertItem(9,L"Число часов работы котла в год/месяц Б.Г."           		  ,0); m_list1->SetItemText(9,1,L"ч"		   );
		m_list1->InsertItem(10,L"Стоимость 1 тыс.м3 газа"									  ,0); m_list1->SetItemText(10,1,L"тыс.руб."	   ); 
		m_list1->InsertItem(11,L"Затраты на выполнение мероприятия"							  ,0); m_list1->SetItemText(11,1,L"тыс.руб."	   ); 
		m_list1->InsertItem(12,L"Время работы после проведения мероприятия" 	       	      ,0); m_list1->SetItemText(12,1,L"ч"		   );

		
		s.Format(L"SELECT "
		L"BTMG, QNMG, NFMG, NMG, TMG, BTBG, QNBG, NFBG, NBG, TBG, PRICE, COSTS, CURTIME, VN0, VN1, VE, ECONOMY "
		L"FROM 17 WHERE ID_PLAN_INFO=%s",ID_INFO);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 17",L"FROM 17F");
		}

		db.fQueryRead(s,crq);
		
		count=crq.m_lRecCount;

		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0]: L"35.0"		));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1]: L"8151.0"	));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2]: L"86.46"	));
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3]: L"86.6"	));
		m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][4]: L"720.0"		));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][5]: L"86.0"		));
		m_list1->SetItemText(6,2,((count!=0)?crq.m_RetValues[0][6]: L"8151.0"	));
		m_list1->SetItemText(7,2,((count!=0)?crq.m_RetValues[0][7]: L"92.29"	));
		m_list1->SetItemText(8,2,((count!=0)?crq.m_RetValues[0][8]: L"92.57"	));
		m_list1->SetItemText(9,2,((count!=0)?crq.m_RetValues[0][9]: L"0.0"		));
		m_list1->SetItemText(10,2,((count!=0)?crq.m_RetValues[0][10]: L"3947.0"	));
		m_list1->SetItemText(11,2,((count!=0)?crq.m_RetValues[0][11]: L"0.0"	));
		m_list1->SetItemText(12,2,((count!=0)?crq.m_RetValues[0][12]: L"744.0"	));

		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,400);
		m_list2.InsertColumn(1,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(2,L"Значение",LVCFMT_LEFT,115);

		m_list2.InsertItem(0,L"Экономия условного топлива при малом горении"					   ,0); 
		m_list2.InsertItem(1,L"Экономия условного топлива при большом горении"  				   ,0); 
		m_list2.InsertItem(2,L"Фактическая экономия газа"          								   ,0); 
		m_list2.InsertItem(3,L"Экономия в денежном выражении"	                	        	   ,0); 

		m_list2.SetItemText(0,1,L"тыс.м3");  m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][13]:  L"0.05"));
		m_list2.SetItemText(1,1,L"тыс.м3");  m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][14]:  L"0.0"));
		m_list2.SetItemText(2,1,L"тыс.м3");	 m_list2.SetItemText(2,2,((count!=0)?crq.m_RetValues[0][15]: L"0.05"));
		m_list2.SetItemText(3,1,L"тыс.руб"); m_list2.SetItemText(3,2,((count!=0)?crq.m_RetValues[0][16]: L"0.19"));

		e17();

		break;
	case 20:
		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,380);
		m_list1->InsertColumn(1,L"Е.И."	   ,LVCFMT_LEFT,115);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,110);

		m_list1->InsertItem(0,L"Мощность заменяемых ламп"							  ,0); m_list1->SetItemText(0,1,L"кВт"	   );
		m_list1->InsertItem(1,L"Мощность устанавливаемых ламп (по паспортным данным)" ,0); m_list1->SetItemText(1,1,L"кВт"	   ); 
		m_list1->InsertItem(2,L"Длительность работы ламп"							  ,0); m_list1->SetItemText(2,1,L"ч"	   ); 
		m_list1->InsertItem(3,L"Количество ламп"							          ,0); m_list1->SetItemText(3,1,L"шт."	   );
		m_list1->InsertItem(4,L"Стоимость 1 кВт х ч"								  ,0); m_list1->SetItemText(4,1,L"руб."	   ); 
		m_list1->InsertItem(5,L"Затраты на выполнение мероприятия"           		  ,0); m_list1->SetItemText(5,1,L"тыс.руб."); 
		m_list1->InsertItem(6,L"Время работы в отчетном периоде"					  ,0); m_list1->SetItemText(6,1,L"ч"	   );

		s.Format(L"SELECT "
		L"NZAM, NUST, T, N, PRICE, COSTS, CURTIME, DW0,  RESULT, ECONOMY "
		L"FROM 20 WHERE ID_PLAN_INFO=%s",ID_INFO);
		//AfxMessageBox(s);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 20",L"FROM 20F");
		}

		db.fQueryRead(s,crq);
		
		count=crq.m_lRecCount;

		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0]: L"80.0"  ));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1]: L"25.0"  ));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2]: L"325.00"));
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3]: L"5"	 ));
		m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][4]: L"3.80"	 ));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][5]: L"0"	 ));
		m_list1->SetItemText(6,2,((count!=0)?crq.m_RetValues[0][6]: L"744"	 ));

		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,400);
		m_list2.InsertColumn(1,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(2,L"Значение",LVCFMT_LEFT,115);

		m_list2.InsertItem(0,L"Экономия электроэнергии для каждого типа ламп",0); 
		m_list2.InsertItem(1,L"Фактическая экономия электроэнергии"          ,0); 
		m_list2.InsertItem(2,L"Экономия в денежном выражении"	             ,0); 

		m_list2.SetItemText(0,1,L"тыс.кВт х ч"); m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][7]: L"17.88"));
		m_list2.SetItemText(1,1,L"тыс.кВт х ч"); m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][8]: L"17.88"));
		m_list2.SetItemText(2,1,L"тыс.руб."	  ); m_list2.SetItemText(2,2,((count!=0)?crq.m_RetValues[0][9]: L"0.07" ));

		e20();

		break;

	case 23:

		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,380);
		m_list1->InsertColumn(1,L"Е.И."	   ,LVCFMT_LEFT,115);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,110);

		m_list1->InsertItem(0,L"Потери мошности холостого хода трансформатора"			,0); m_list1->SetItemText(0,1,L"кВт"	 );
		m_list1->InsertItem(1,L"Планируемое время отключения трансформатора"			,0); m_list1->SetItemText(1,1,L"ч"	     ); 
		m_list1->InsertItem(2,L"Напряжение на высшей стороне трансформатора"			,0); m_list1->SetItemText(2,1,L"кВ"	     ); 
		m_list1->InsertItem(3,L"Номинальное напряжение высшей обмотки трансформатора"	,0); m_list1->SetItemText(3,1,L"кВ"	     );
		m_list1->InsertItem(4,L"Количество отключаемых ТП и силовых трансформаторов"	,0); m_list1->SetItemText(4,1,L"шт."	 );
		m_list1->InsertItem(5,L"Стоимость 1 кВт х ч"           							,0); m_list1->SetItemText(5,1,L"руб."	 );
		m_list1->InsertItem(6,L"Затраты на выполнение мероприятия"           		    ,0); m_list1->SetItemText(6,1,L"тыс.руб."); 
		m_list1->InsertItem(7,L"Время работы в отчетном периоде"						,0); m_list1->SetItemText(7,1,L"ч"	     );

		s.Format(L"SELECT "
		L"DPIDLE, T, U, UNOM, N, PRICE, CURCOSTS, CURTIME, RESULT, ECONOMY "
		L"FROM 23 WHERE ID_PLAN_INFO=%s",ID_INFO);
		//AfxMessageBox(s);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 23",L"FROM 23F");
		}

		db.fQueryRead(s,crq);
		
		count=crq.m_lRecCount;

		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0]: L"1.05"  ));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1]: L"744.00"));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2]: L"6.00"	 ));
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3]: L"6.00"	 ));
		m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][4]: L"1"	 ));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][5]: L"3.60"	 ));
		m_list1->SetItemText(6,2,((count!=0)?crq.m_RetValues[0][6]: L"0"	 ));
		m_list1->SetItemText(7,2,((count!=0)?crq.m_RetValues[0][7]: L"744"	 ));

		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,400);
		m_list2.InsertColumn(1,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(2,L"Значение",LVCFMT_LEFT,115);

		m_list2.InsertItem(0,L"Фактическая экономия электроэнергии",0); 
		m_list2.InsertItem(1,L"Экономия в денежном выражении"      ,0); 

		m_list2.SetItemText(0,1,L"тыс.кВт х ч"); m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][8]: L"0.78"));
		m_list2.SetItemText(1,1,L"тыс.руб."	  ); m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][9]: L"2.81"));

		e23();

		break;

	case 25:
#pragma region 25
		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,320);
		m_list1->InsertColumn(1,L"Е.И."	   ,LVCFMT_LEFT,80);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,80);

		m_list1->InsertItem(0,L"Номинальная мощность э/дв АВО газа"			   ,0); m_list1->SetItemText(0,1,L"кВт"     );  
		m_list1->InsertItem(1,L"Суммарное количество часов отключений э/дв АВО",0); m_list1->SetItemText(1,1,L"ч"       ); 
		m_list1->InsertItem(2,L"КПД э/дв АВО газа"							   ,0); m_list1->SetItemText(2,1,L" "       ); 
		m_list1->InsertItem(3,L"Стоимость 1 кВт х ч"						   ,0); m_list1->SetItemText(3,1,L"руб."    ); 
		m_list1->InsertItem(4,L"Затраты"									   ,0); m_list1->SetItemText(4,1,L"тыс.руб."); 
		m_list1->InsertItem(5,L"Время работы"								   ,0); m_list1->SetItemText(5,1,L"б/р"     ); 

		s.Format(L"SELECT "
		L"N0, NSD, KPD, PRICE, COSTS, CURTIME, "
		L"DW1, RESULT, ECONOMY "
		L"FROM 25 WHERE ID_PLAN_INFO=%s",ID_INFO);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 25",L"FROM 25F");
		}

		//AfxMessageBox(s);

		db.fQueryRead(s,crq);
		
		count=crq.m_lRecCount;

		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0]: L"1.00"));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1]: L"2.00"));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2]: L"3.00"));
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3]: L"4.00"));
		m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][4]: L"5.00"));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][5]: L"6.00"));

		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,300);
		m_list2.InsertColumn(1,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(2,L"Значение",LVCFMT_LEFT,80);

		m_list2.InsertItem(0,L"Экономия электроэнергии для каждого вентилятора",0); 
		m_list2.InsertItem(1,L"Экономия",0); 
		m_list2.InsertItem(2,L"Экономия (денеж.)",0); 

		m_list2.SetItemText(0,1,L"тыс.кВт х ч"); m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][6]: L"7.00"));
		m_list2.SetItemText(1,1,L"тыс.кВт х ч"); m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][7]: L"7.00"));
		m_list2.SetItemText(2,1,L"тыс.руб");	 m_list2.SetItemText(2,2,((count!=0)?crq.m_RetValues[0][8]: L"9.00"));

		e25();
#pragma endregion 25
		break;
	case 26:
		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,320);
		m_list1->InsertColumn(1,L"Е.И."	   ,LVCFMT_LEFT,80);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,80);

		m_list1->InsertItem(0,L"Номинальная мощность э/дв АВО газа (масла)"						 ,0); m_list1->SetItemText(0,1,L"кВт"     );  
		m_list1->InsertItem(1,L"Время работы э.дв. АВО гааз за отчетный период"					 ,0); m_list1->SetItemText(1,1,L"ч"       ); 
		m_list1->InsertItem(2,L"КПД электродвигателя"											 ,0); m_list1->SetItemText(2,1,L"б/р"     ); 
		m_list1->InsertItem(3,L"Коэффициент ухудшения теплоотдачи АВО газа при засорении трубок" ,0); m_list1->SetItemText(3,1,L"б/р"     ); 
		m_list1->InsertItem(4,L"Стоимость 1 кВт"												 ,0); m_list1->SetItemText(4,1,L"руб."    );
		m_list1->InsertItem(5,L"Затраты"														 ,0); m_list1->SetItemText(5,1,L"тыс.руб.");
		m_list1->InsertItem(6,L"Время работы"													 ,0); m_list1->SetItemText(6,1,L"ч"       ); 

		s.Format(L"SELECT "
		L"N0, T, KPD, K, PRICE, COSTS, CURTIME, RESULT, ECONOMY "
		L"FROM 26 WHERE ID_PLAN_INFO=%s",ID_INFO);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 26",L"FROM 26F");
		}

		db.fQueryRead(s,crq);
		
		count=crq.m_lRecCount;

		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0]: L"5.50"  ));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1]: L"480.00"));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2]: L"0.85"  ));
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3]: L"0.09"  ));
		m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][4]: L"3.80"  ));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][5]: L"0"     ));
		m_list1->SetItemText(6,2,((count!=0)?crq.m_RetValues[0][6]: L"744"   ));


		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,300);
		m_list2.InsertColumn(1,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(2,L"Значение",LVCFMT_LEFT,80);

		m_list2.InsertItem(0,L"Фактическая экономия электроэнергии",0); 
		m_list2.InsertItem(1,L"Экономия в денежном выражении",0); 
		//m_list2.InsertItem(2,L"Экономия (денеж.)",0); 

		m_list2.SetItemText(0,1,L"тыс.кВт х ч"); m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][7]: L"0.28"));
		m_list2.SetItemText(1,1,L"тыс.руб"	  ); m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][8]: L"1.07"));
		
		e26();

		break;

	case 27:
#pragma region 27
		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,430);
		m_list1->InsertColumn(1,L"Е.И."	   ,LVCFMT_LEFT,80);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,80);

		m_list1->InsertItem(0,L"Потребляемая мощность электродвигателя АВО"						  ,0); m_list1->SetItemText(0,1,L"кВт"	   ); 
		m_list1->InsertItem(1,L"Максимальная потребляемая мощность электродвигателя АВО"		  ,0); m_list1->SetItemText(1,1,L"кВт"	   ); 
		m_list1->InsertItem(2,L"Разность температур газа на входе и на выходе из АВОГ"			  ,0); m_list1->SetItemText(2,1,L"град.Ц." ); 
		m_list1->InsertItem(3,L"Макс.возможное охлаждение газа, достигаемое при макс. мощности N0",0); m_list1->SetItemText(3,1,L"град.Ц." ); 
		m_list1->InsertItem(4,L"Показатель степени"												  ,0); m_list1->SetItemText(4,1,L"б/р"	   ); 
		m_list1->InsertItem(5,L"Суммарная мощность всех двигателей в 'батарее' АВОг"		      ,0); m_list1->SetItemText(5,1,L"кВт"	   ); 
		m_list1->InsertItem(6,L"Суммарная планируемая годовая наработка АВОг"					  ,0); m_list1->SetItemText(6,1,L"ч"	   ); 
		m_list1->InsertItem(7,L"Стоимость 1 кВт х ч"											  ,0); m_list1->SetItemText(7,1,L"руб."	   ); 
		m_list1->InsertItem(8,L"Затраты"														  ,0); m_list1->SetItemText(8,1,L"тыс.руб."); 
		m_list1->InsertItem(9,L"Время работы"													  ,0); m_list1->SetItemText(9,1,L"ч"	   ); 
		
		s.Format(L"SELECT "
		L"NAVO,NAVOMAX,DTINOUT,DTMAX,b,"
		L"N,T,PRICE,COSTS,CURTIME,"
		L"KLOADNH,KLOADND,DRES,RESULT,ECONOMY "
		L"FROM 27 WHERE ID_PLAN_INFO=%s",ID_INFO);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 27",L"FROM 27F");
		}

		db.fQueryRead(s,crq);
		
		count=crq.m_lRecCount;

		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0]: L"1.00"));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1]: L"2.00"));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2]: L"3.00"));
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3]: L"4.00"));
		m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][4]: L"5.00"));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][5]: L"6.00"));
		m_list1->SetItemText(6,2,((count!=0)?crq.m_RetValues[0][6]: L"7.00"));
		m_list1->SetItemText(7,2,((count!=0)?crq.m_RetValues[0][7]: L"8.00"));
		m_list1->SetItemText(8,2,((count!=0)?crq.m_RetValues[0][8]: L"9.00"));
		m_list1->SetItemText(9,2,((count!=0)?crq.m_RetValues[0][9]: L"10.00"));
		
		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,300);
		m_list2.InsertColumn(1,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(2,L"Значение",LVCFMT_LEFT,80);

		m_list2.InsertItem(0,L"К загрузки мощности при частотном регулировании" ,0); 
		m_list2.InsertItem(1,L"К загрузки мощности при дискретном регулировании",0); 
		m_list2.InsertItem(2,L"Экономия электроэнергии"						    ,0);
		m_list2.InsertItem(3,L"Экономия"										,0);
		m_list2.InsertItem(4,L"Экономия (денеж.)"								,0); 

		m_list2.SetItemText(0,1,L"б/р"		  ); m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][9]:  L"10.00"));
		m_list2.SetItemText(1,1,L"б/р"		  ); m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][10]: L"11.00"));
		m_list2.SetItemText(2,1,L"тыс.кВт х ч"); m_list2.SetItemText(2,2,((count!=0)?crq.m_RetValues[0][11]: L"12.00"));
		m_list2.SetItemText(3,1,L"тыс.кВт х ч"); m_list2.SetItemText(3,2,((count!=0)?crq.m_RetValues[0][12]: L"13.00"));
		m_list2.SetItemText(4,1,L"тыс.руб."	  ); m_list2.SetItemText(4,2,((count!=0)?crq.m_RetValues[0][13]: L"14.00"));

		e27();
#pragma endregion 27
		break;
	case 38:
#pragma region 38
		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,430);
		m_list1->InsertColumn(1,L"Е.И.",LVCFMT_LEFT,80);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,80);

		m_list1->InsertItem(0,L"Экономический коэффициент реактивной мощности"		       ,0); m_list1->SetItemText(0,1,L"б/р"	    ); 
		m_list1->InsertItem(1,L"Величина потребления электроэнергии КЦ за расчетный период",0); m_list1->SetItemText(1,1,L"кВт х ч" ); 
		m_list1->InsertItem(2,L"Тангенс угла ф1 до установки конденсаторных батарей"	   ,0); m_list1->SetItemText(2,1,L"-"	    ); 
		m_list1->InsertItem(3,L"Тангенс угла ф2 после установки конденсаторных батарей"	   ,0); m_list1->SetItemText(3,1,L"-"	    ); 
		m_list1->InsertItem(4,L"Стоимость 1 кВт х ч"									   ,0); m_list1->SetItemText(4,1,L"руб."    ); 
		m_list1->InsertItem(5,L"Затраты"												   ,0); m_list1->SetItemText(5,1,L"тыс.руб."); 
		m_list1->InsertItem(6,L"Время работы"											   ,0); m_list1->SetItemText(6,1,L"ч"		); 

		s.Format(L"SELECT "
		L"EKRN,WKC,TGANGLEF1,TGANGLEF2,PRICE,"
		L"COSTS,CURTIME,RESULT,ECONOMY "
		L"FROM 38 WHERE ID_PLAN_INFO=%s",ID_INFO);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 38",L"FROM 38F");
		}

		db.fQueryRead(s,crq);
		
		count=crq.m_lRecCount;

		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0]: L"1.00"));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1]: L"2.00"));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2]: L"3.00"));
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3]: L"4.00"));
		m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][4]: L"5.00"));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][5]: L"6.00"));
		m_list1->SetItemText(6,2,((count!=0)?crq.m_RetValues[0][6]: L"7.00"));

		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,300);
		m_list2.InsertColumn(1,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(2,L"Значение",LVCFMT_LEFT,80);

		m_list2.InsertItem(0,L"Экономия"		 ,0); m_list2.SetItemText(0,1,L"тыс.кВт х ч"); 
		m_list2.InsertItem(1,L"Экономия (денеж.)",0); m_list2.SetItemText(1,1,L"тыс.руб."	); 

		m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][7]: L"8.00"));
		m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][8]: L"9.00"));

		e38();
#pragma endregion 38
		break;
	case 42:
#pragma region 42
		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,430);
		m_list1->InsertColumn(1,L"Е.И.",LVCFMT_LEFT,80);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,80);

		m_list1->InsertItem(0,L"Коэффициент теплопроводности"								   ,0); m_list1->SetItemText(0,1,L"Вт/м2 х C"); 
		m_list1->InsertItem(1,L"Площадь поверхности, через которую происходят тепловые потери" ,0); m_list1->SetItemText(1,1,L"м2"		 ); 
		m_list1->InsertItem(2,L"Наружная измеренная Т воздуха в момент проведения обследования",0); m_list1->SetItemText(2,1,L"град.Ц."	 ); 
		m_list1->InsertItem(3,L"Т исследуемой поверхности"									   ,0); m_list1->SetItemText(3,1,L"град.Ц."	 ); 
		m_list1->InsertItem(4,L"Количество дней сезона отопления"							   ,0); m_list1->SetItemText(4,1,L"дн."		 ); 
		m_list1->InsertItem(5,L"Стоимость 1 Гкал"											   ,0); m_list1->SetItemText(5,1,L"руб."	 ); 
		m_list1->InsertItem(6,L"Затраты"													   ,0); m_list1->SetItemText(6,1,L"тыс.руб." ); 
		m_list1->InsertItem(7,L"Время работы"												   ,0); m_list1->SetItemText(7,1,L"ч"		 ); 

		s.Format(L"SELECT "
		L"KT,S,TAIR,TMAT,NTEP,"
		L"PRICE,COSTS,CURTIME,RESULT,ECONOMY "
		L"FROM 42 WHERE ID_PLAN_INFO=%s",ID_INFO);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 42",L"FROM 42F");
		}

		db.fQueryRead(s,crq);
		
		count=crq.m_lRecCount;

		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0]: L"1.00"));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1]: L"2.00"));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2]: L"3.00"));
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3]: L"4.00"));
		m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][4]: L"5.00"));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][5]: L"6.00"));
		m_list1->SetItemText(6,2,((count!=0)?crq.m_RetValues[0][6]: L"7.00"));
		m_list1->SetItemText(7,2,((count!=0)?crq.m_RetValues[0][7]: L"8.00"));

		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,300);
		m_list2.InsertColumn(1,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(2,L"Значение",LVCFMT_LEFT,80);

		m_list2.InsertItem(0,L"Экономия"		 ,0); m_list2.SetItemText(0,1,L"Гкал"); 
		m_list2.InsertItem(1,L"Экономия (денеж.)",0); m_list2.SetItemText(1,1,L"руб."); 

		m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][8]: L"9.00"));
		m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][9]: L"10.00"));

		e42();
#pragma endregion 42
		break;
	case 43:

		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,430);
		m_list1->InsertColumn(1,L"Е.И.",LVCFMT_LEFT,80);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,80);

		m_list1->InsertItem(0,L"Количество тепла выработанное утилизатором"											 ,0); m_list1->SetItemText(0,1,L"Гкал"	 ); 
		m_list1->InsertItem(1,L"Низшая теплота сгорания газа"														 ,0); m_list1->SetItemText(1,1,L"ккал/м3"); 
		m_list1->InsertItem(2,L"Средневзвешенный КПД котлов (результат РНИ)"										 ,0); m_list1->SetItemText(2,1,L"-"		 ); 
		m_list1->InsertItem(3,L"Дополнительная мощность (для компенсации увел.сопротивления выхл.тракта ГПА с i УТ)" ,0); m_list1->SetItemText(3,1,L"кВт"	 ); 
		m_list1->InsertItem(4,L"Номинальный эффективный КПД ГТУ"													 ,0); m_list1->SetItemText(4,1,L"-"		 ); 
		m_list1->InsertItem(5,L"Наработка ГТУ с УТ"																	 ,0); m_list1->SetItemText(5,1,L"ч"		 ); 
		m_list1->InsertItem(6,L"Коэффициент загрузки ГТУ"															 ,0); m_list1->SetItemText(6,1,L"-"		 ); 
		m_list1->InsertItem(7,L"Аэродинамическое сопортивление утилизатора"											 ,0); m_list1->SetItemText(7,1,L"Па"	 ); 
		m_list1->InsertItem(8,L"Массовый расход продуктов сгорания через утилизатор"								 ,0); m_list1->SetItemText(8,1,L"кг/с"	 ); 
		m_list1->InsertItem(9,L"Средняя температура продуктов сгорания через УТ"									 ,0); m_list1->SetItemText(9,1,L"град.Ц."); 
		m_list1->InsertItem(10,L"Стоимость 1 Гкал"																	 ,0); m_list1->SetItemText(10,1,L"тыс.руб"); 
		m_list1->InsertItem(11,L"Затраты на выполненное мероприятия"												 ,0); m_list1->SetItemText(11,1,L"тыс.руб"); 
		m_list1->InsertItem(12,L"Время работы в отчетном периоде"												     ,0); m_list1->SetItemText(12,1,L"ч"      ); 

		s.Format(L"SELECT "
		L"QUT,"     //0
		L"QRN,"		//1
		L"KPDK,"    //2
		L"NADD,"    //3
		L"KPDGTA,"  //4
		L"TGTA,"    //5
		L"KLOAD,"   //6
		L"DPUT,"    //7
		L"G,"       //8
		L"TEMP,"    //9
		L"PRICE,"   //10
		L"CURCOSTS,"//11
		L"CURTIME," //12

		L"Q,"       //13
		L"RASH,"    //14
		L"NADD2,"   //15
		L"QGTAI,"   //16
		L"QGTA,"    //17
		L"QK,"      //18
		L"RESULT,"  //19
		L"ECONOMY " //20
		L"FROM 43 WHERE ID_PLAN_INFO=%s",ID_INFO);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 43",L"FROM 43F");
		}

		db.fQueryRead(s,crq);
		
		count=crq.m_lRecCount;

		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0] : L"100.00" ));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1] : L"8211.00"));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2] : L"0.91"   ));
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3] : L"50.00"  ));
		m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][4] : L"0.276"  ));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][5] : L"7.00"   ));
		m_list1->SetItemText(6,2,((count!=0)?crq.m_RetValues[0][6] : L"2.00"   ));
		m_list1->SetItemText(7,2,((count!=0)?crq.m_RetValues[0][7] : L"23.00"  ));
		m_list1->SetItemText(8,2,((count!=0)?crq.m_RetValues[0][8] : L"300.000"));
		m_list1->SetItemText(9,2,((count!=0)?crq.m_RetValues[0][9] : L"650.00" ));
		m_list1->SetItemText(10,2,((count!=0)?crq.m_RetValues[0][10]: L"1.23"   ));
		m_list1->SetItemText(11,2,((count!=0)?crq.m_RetValues[0][11]: L"0.00"   ));
		m_list1->SetItemText(12,2,((count!=0)?crq.m_RetValues[0][12]: L"744.00" ));

		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,300);
		m_list2.InsertColumn(1,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(2,L"Значение",LVCFMT_LEFT,80);

		m_list2.InsertItem(0,L"Плотность продуктов сгорания"																					,0); m_list2.SetItemText(0,1,L"кг/м3"   ); 
		m_list2.InsertItem(1,L"Расход продуктов сгорания ГТУ через утилизатор"																	,0); m_list2.SetItemText(1,1,L"м3/с"    ); 
		m_list2.InsertItem(2,L"Дополнительная мощность"																							,0); m_list2.SetItemText(2,1,L"кВт"     ); 
		m_list2.InsertItem(3,L"Доп. расход ТГ, вызванный увеличением сопротивления выхлопного тракта при установке утилизаторов для каждого ГТУ",0); m_list2.SetItemText(3,1,L"тыс.м3"  ); 
		m_list2.InsertItem(4,L"Дополнительный расход топливного газа (общий)"																	,0); m_list2.SetItemText(4,1,L"тыс.м3"  ); 
		m_list2.InsertItem(5,L"Расход газа замещаемой котельной"																				,0); m_list2.SetItemText(5,1,L"тыс.м3"  ); 
		m_list2.InsertItem(6,L"Фактическая экономия тепла"																						,0); m_list2.SetItemText(6,1,L"Гкал"); 
		m_list2.InsertItem(7,L"Экономия в денежном выражении"																					,0); m_list2.SetItemText(7,1,L"тыс.руб."); 

		m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][13]: L"9.00"));
		m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][14]: L"10.00"));
		m_list2.SetItemText(2,2,((count!=0)?crq.m_RetValues[0][15]: L"9.00"));
		m_list2.SetItemText(3,2,((count!=0)?crq.m_RetValues[0][16]: L"10.00"));
		m_list2.SetItemText(4,2,((count!=0)?crq.m_RetValues[0][17]: L"10.00"));
		m_list2.SetItemText(5,2,((count!=0)?crq.m_RetValues[0][18]: L"9.00"));
		m_list2.SetItemText(6,2,((count!=0)?crq.m_RetValues[0][19]: L"10.00"));
		m_list2.SetItemText(7,2,((count!=0)?crq.m_RetValues[0][20]: L"10.00"));

		e43();

		break;

	case 48:
#pragma region 48
			
		//AfxMessageBox(L"48");
		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,430);
		m_list1->InsertColumn(1,L"Е.И."	   ,LVCFMT_LEFT,80);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,80);

		m_list1->InsertItem(0,L"Фактическое годовое потребление тепловой энергии"			  ,0); m_list1->SetItemText(0,1,L"Гкал/год"); 
		m_list1->InsertItem(1,L"Внутренняя расчетная Т воздуха в помещении"					  ,0); m_list1->SetItemText(1,1,L"град.Ц." ); 
		m_list1->InsertItem(2,L"Средняя Т наружного воздуха за отопительный период"			  ,0); m_list1->SetItemText(2,1,L"град.Ц." ); 
		m_list1->InsertItem(3,L"Расчетная температура наружного воздуха для системы отопления",0); m_list1->SetItemText(3,1,L"град.Ц." ); 
		m_list1->InsertItem(4,L"Низшая рабочая теплота сгорания природного газа"			  ,0); m_list1->SetItemText(4,1,L"ккал/м3"  ); 
		m_list1->InsertItem(5,L"КПД теплоагрегатов"											  ,0); m_list1->SetItemText(5,1,L"-"	   ); 
		m_list1->InsertItem(6,L"Стоимость 1 Гкал"											  ,0); m_list1->SetItemText(6,1,L"тыс.руб."); 
		m_list1->InsertItem(7,L"Затраты"													  ,0); m_list1->SetItemText(7,1,L"тыс.руб."); 
		m_list1->InsertItem(8,L"Время работы"												  ,0); m_list1->SetItemText(8,1,L"ч"); 

		s.Format(L"SELECT "
		L"QFYEAR,TINT,THT,TEXTS,"
		L"KPD,PRICE,COSTS,CURTIME,RESULT,ECONOMY "
		L"FROM 48 WHERE ID_PLAN_INFO=%s",ID_INFO);

		//AfxMessageBox(s);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 48",L"FROM 48F");
		}

		db.fQueryRead(s,crq);
		
		count=crq.m_lRecCount;

		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0]: L"1.00"));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1]: L"2.00"));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2]: L"3.00") );
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3]: L"4.00"));
		m_list1->SetItemText(4,2,(L"8150"));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][4]: L"6.00"));
		m_list1->SetItemText(6,2,((count!=0)?crq.m_RetValues[0][5]: L"7.00"));
		m_list1->SetItemText(7,2,((count!=0)?crq.m_RetValues[0][6]: L"8.00"));
		m_list1->SetItemText(8,2,((count!=0)?crq.m_RetValues[0][7]: L"8.00"));



		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,300);
		m_list2.InsertColumn(1,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(2,L"Значение",LVCFMT_LEFT,80);

		m_list2.InsertItem(0,L"Экономия"	     ,0);
		m_list2.InsertItem(1,L"Экономия (денеж.)",0); 

		m_list2.SetItemText(0,1,L"тыс.Гкал"); m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][8]: L"10.00"));
		m_list2.SetItemText(1,1,L"тыс.руб."); m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][9]: L"11.00"));

		e48();

#pragma endregion 48
		break;
		case 481:
#pragma region 481
			
		//AfxMessageBox(L"48");
		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,430);
		m_list1->InsertColumn(1,L"Е.И."	   ,LVCFMT_LEFT,80);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,80);

		m_list1->InsertItem(0,L"Фактическое годовое потребление тепловой энергии"			  ,0); m_list1->SetItemText(0,1,L"Гкал/год"); 
		m_list1->InsertItem(1,L"Внутренняя расчетная Т воздуха в помещении"					  ,0); m_list1->SetItemText(1,1,L"град.Ц." ); 
		m_list1->InsertItem(2,L"Средняя Т наружного воздуха за отопительный период"			  ,0); m_list1->SetItemText(2,1,L"град.Ц." ); 
		m_list1->InsertItem(3,L"Расчетная температура наружного воздуха для системы отопления",0); m_list1->SetItemText(3,1,L"град.Ц." ); 
		m_list1->InsertItem(4,L"Низшая рабочая теплота сгорания природного газа"			  ,0); m_list1->SetItemText(4,1,L"ккал/м3"  ); 
		m_list1->InsertItem(5,L"КПД теплоагрегатов"											  ,0); m_list1->SetItemText(5,1,L"-"	   ); 
		m_list1->InsertItem(6,L"Стоимость 1 Гкал"											  ,0); m_list1->SetItemText(6,1,L"тыс.руб."); 
		m_list1->InsertItem(7,L"Затраты"													  ,0); m_list1->SetItemText(7,1,L"тыс.руб."); 
		m_list1->InsertItem(8,L"Время работы"												  ,0); m_list1->SetItemText(8,1,L"ч"); 

		s.Format(L"SELECT "
		L"QFYEAR,TINT,THT,TEXTS,"
		L"KPD,PRICE,COSTS,CURTIME,RESULT,ECONOMY,QRN,RESULT_ALT "
		L"FROM 481 WHERE ID_PLAN_INFO=%s",ID_INFO);

		//AfxMessageBox(s);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 481",L"FROM 481F");
		}

		db.fQueryRead(s,crq);
		
		count=crq.m_lRecCount;

		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0]: L"1.00"));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1]: L"2.00"));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2]: L"3.00") );
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3]: L"4.00"));
		m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][10]: L"5.00"));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][4]: L"6.00"));
		m_list1->SetItemText(6,2,((count!=0)?crq.m_RetValues[0][5]: L"7.00"));
		m_list1->SetItemText(7,2,((count!=0)?crq.m_RetValues[0][6]: L"8.00"));
		m_list1->SetItemText(8,2,((count!=0)?crq.m_RetValues[0][7]: L"9.00"));



		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,300);
		m_list2.InsertColumn(1,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(2,L"Значение",LVCFMT_LEFT,80);

		m_list2.InsertItem(0,L"Экономия"	     ,0);
		m_list2.InsertItem(1,L"Экономия"	     ,0);
		m_list2.InsertItem(2,L"Экономия (денеж.)",0); 

		m_list2.SetItemText(0,1,L"Гкал"); m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][8]:	 L"10.00"));
		m_list2.SetItemText(1,1,L"м3"); m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][11]:		 L"11.00"));
		m_list2.SetItemText(2,1,L"тыс.руб."); m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][9]: L"12.00"));

		e481();

#pragma endregion 481
		break;
		case 431:

		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,430);
		m_list1->InsertColumn(1,L"Е.И.",LVCFMT_LEFT,80);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,80);

		m_list1->InsertItem(0,L"Количество тепла выработанное утилизатором"											 ,0); m_list1->SetItemText(0,1,L"Гкал"	 ); 
		m_list1->InsertItem(1,L"Низшая теплота сгорания газа"														 ,0); m_list1->SetItemText(1,1,L"ккал/м3"); 
		m_list1->InsertItem(2,L"Средневзвешенный КПД котлов (результат РНИ)"										 ,0); m_list1->SetItemText(2,1,L"-"		 ); 
		m_list1->InsertItem(3,L"Дополнительная мощность (для компенсации увел.сопротивления выхл.тракта ГПА с i УТ)" ,0); m_list1->SetItemText(3,1,L"кВт"	 ); 
		m_list1->InsertItem(4,L"Номинальный эффективный КПД ГТУ"													 ,0); m_list1->SetItemText(4,1,L"-"		 ); 
		m_list1->InsertItem(5,L"Наработка ГТУ с УТ"																	 ,0); m_list1->SetItemText(5,1,L"ч"		 ); 
		m_list1->InsertItem(6,L"Коэффициент загрузки ГТУ"															 ,0); m_list1->SetItemText(6,1,L"-"		 ); 
		m_list1->InsertItem(7,L"Аэродинамическое сопортивление утилизатора"											 ,0); m_list1->SetItemText(7,1,L"Па"	 ); 
		m_list1->InsertItem(8,L"Массовый расход продуктов сгорания через утилизатор"								 ,0); m_list1->SetItemText(8,1,L"кг/с"	 ); 
		m_list1->InsertItem(9,L"Средняя температура продуктов сгорания через УТ"									 ,0); m_list1->SetItemText(9,1,L"град.Ц."); 
		m_list1->InsertItem(10,L"Стоимость 1 Гкал"																	 ,0); m_list1->SetItemText(10,1,L"тыс.руб"); 
		m_list1->InsertItem(11,L"Затраты на выполненное мероприятия"												 ,0); m_list1->SetItemText(11,1,L"тыс.руб"); 
		m_list1->InsertItem(12,L"Время работы в отчетном периоде"												     ,0); m_list1->SetItemText(12,1,L"ч"      ); 

		s.Format(L"SELECT "
		L"QUT,"     //0
		L"QRN,"		//1
		L"KPDK,"    //2
		L"NADD,"    //3
		L"KPDGTA,"  //4
		L"TGTA,"    //5
		L"KLOAD,"   //6
		L"DPUT,"    //7
		L"G,"       //8
		L"TEMP,"    //9
		L"PRICE,"   //10
		L"CURCOSTS,"//11
		L"CURTIME," //12

		L"Q,"       //13
		L"RASH,"    //14
		L"NADD2,"   //15
		L"QGTAI,"   //16
		L"QGTA,"    //17
		L"QK,"      //18
		L"RESULT,"  //19
		L"ECONOMY, " //20
		L"RESULT_ALT "//21
		L"FROM 431 WHERE ID_PLAN_INFO=%s",ID_INFO);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 431",L"FROM 431F");
		}

		db.fQueryRead(s,crq);
		
		count=crq.m_lRecCount;

		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0] : L"100.00" ));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1] : L"8211.00"));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2] : L"0.91"   ));
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3] : L"50.00"  ));
		m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][4] : L"0.276"  ));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][5] : L"7.00"   ));
		m_list1->SetItemText(6,2,((count!=0)?crq.m_RetValues[0][6] : L"2.00"   ));
		m_list1->SetItemText(7,2,((count!=0)?crq.m_RetValues[0][7] : L"23.00"  ));
		m_list1->SetItemText(8,2,((count!=0)?crq.m_RetValues[0][8] : L"300.000"));
		m_list1->SetItemText(9,2,((count!=0)?crq.m_RetValues[0][9] : L"650.00" ));
		m_list1->SetItemText(10,2,((count!=0)?crq.m_RetValues[0][10]: L"1.23"   ));
		m_list1->SetItemText(11,2,((count!=0)?crq.m_RetValues[0][11]: L"0.00"   ));
		m_list1->SetItemText(12,2,((count!=0)?crq.m_RetValues[0][12]: L"744.00" ));

		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,300);
		m_list2.InsertColumn(1,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(2,L"Значение",LVCFMT_LEFT,80);

		m_list2.InsertItem(0,L"Плотность продуктов сгорания"																					,0); m_list2.SetItemText(0,1,L"кг/м3"   ); 
		m_list2.InsertItem(1,L"Расход продуктов сгорания ГТУ через утилизатор"																	,0); m_list2.SetItemText(1,1,L"м3/с"    ); 
		m_list2.InsertItem(2,L"Дополнительная мощность"																							,0); m_list2.SetItemText(2,1,L"кВт"     ); 
		m_list2.InsertItem(3,L"Доп. расход ТГ, вызванный увеличением сопротивления выхлопного тракта при установке утилизаторов для каждого ГТУ",0); m_list2.SetItemText(3,1,L"тыс.м3"  ); 
		m_list2.InsertItem(4,L"Дополнительный расход топливного газа (общий)"																	,0); m_list2.SetItemText(4,1,L"тыс.м3"  ); 
		m_list2.InsertItem(5,L"Расход газа замещаемой котельной"																				,0); m_list2.SetItemText(5,1,L"тыс.м3"  ); 
		m_list2.InsertItem(6,L"Фактическая экономия газа"																						,0); m_list2.SetItemText(6,1,L"тыс.м3"); 
		m_list2.InsertItem(7,L"Фактическая экономия тепла"																						,0); m_list2.SetItemText(7,1,L"Гкал");
		m_list2.InsertItem(8,L"Экономия в денежном выражении"																					,0); m_list2.SetItemText(8,1,L"тыс.руб."); 

		m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][13]: L"9.00"));
		m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][14]: L"10.00"));
		m_list2.SetItemText(2,2,((count!=0)?crq.m_RetValues[0][15]: L"9.00"));
		m_list2.SetItemText(3,2,((count!=0)?crq.m_RetValues[0][16]: L"10.00"));
		m_list2.SetItemText(4,2,((count!=0)?crq.m_RetValues[0][17]: L"10.00"));
		m_list2.SetItemText(5,2,((count!=0)?crq.m_RetValues[0][18]: L"9.00"));
		m_list2.SetItemText(6,2,((count!=0)?crq.m_RetValues[0][19]: L"10.00"));
		m_list2.SetItemText(7,2,((count!=0)?crq.m_RetValues[0][19]: L"10.00"));
		m_list2.SetItemText(8,2,((count!=0)?crq.m_RetValues[0][20]: L"10.00"));

		e431();

		break;

	case 101:

		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,400);
		m_list1->InsertColumn(1,L"Е.И."	   ,LVCFMT_LEFT,115);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,110);

		s.Format(
			L"SELECT " 
			L"P0, P1, T0, T1,"
			L"Q0,DIN,L,PRICE,COSTS,"
			L"CURTIME,DQ,V,Z0,Z1,RESULT,"
			L"ECONOMY "
			L"FROM 101 "
			L"WHERE ID_PLAN_INFO=%s",
			ID_INFO);

		

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 101",L"FROM 101F");
		}
		//AfxMessageBox(s);
		db.fQueryRead(s,crq);

		//AfxMessageBox(L"!!!");

		count=crq.m_lRecCount;

		

		m_list1->InsertItem(0,L"Р газа в трубопроводе до предвар.снижения в рем.участке"   ,0); 
		m_list1->InsertItem(1,L"P газа в трубопроводе после предвар.снижения в рем.участке",0); 
		m_list1->InsertItem(2,L"Т газа в трубопроводе до предвар.снижения в рем.участке"   ,0);  
		m_list1->InsertItem(3,L"Т газа в трубопроводе после предвар.снижения в рем.участке",0);  
		m_list1->InsertItem(4,L"Плотность прир.газа при стд.условиях"					   ,0);  
		m_list1->InsertItem(5,L"Внутренний диаметр трубопровода"						   ,0);
		m_list1->InsertItem(6,L"Длина участка трубопровода"								   ,0);
		m_list1->InsertItem(7,L"Стоимость 1 тыс.м3 газа"								   ,0);  
		m_list1->InsertItem(8,L"Затраты"												   ,0); 
		m_list1->InsertItem(9,L"Время работы после мероприятия"							   ,0);  

		m_list1->SetItemText(0,1,L"МПа"	    ); 
		m_list1->SetItemText(1,1,L"МПа"	    ); 
		m_list1->SetItemText(2,1,L"К"		); 
		m_list1->SetItemText(3,1,L"К"	    ); 
		m_list1->SetItemText(4,1,L"кг/м3"	); 
		m_list1->SetItemText(5,1,L"м"		); 
		m_list1->SetItemText(6,1,L"м"	    ); 
		m_list1->SetItemText(7,1,L"тыс.руб."); 
		m_list1->SetItemText(8,1,L"тыс.руб.");  
		m_list1->SetItemText(9,1,L"ч"		); 

		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0]:L"5.60"));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1]:L"4.10"));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2]:L"283.00"));
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3]:L"283.00"));
		m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][4]:L"0.684"));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][5]:L"1.40"));
		m_list1->SetItemText(6,2,((count!=0)?crq.m_RetValues[0][6]:L"5000.00"));
		m_list1->SetItemText(7,2,((count!=0)?crq.m_RetValues[0][7]:L"3947"));
		m_list1->SetItemText(8,2,((count!=0)?crq.m_RetValues[0][8]:L"0"));
		m_list1->SetItemText(9,2,((count!=0)?crq.m_RetValues[0][9]:L"744"));

		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,300);
		m_list2.InsertColumn(2,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(3,L"Значение",LVCFMT_LEFT,115);

		m_list2.InsertItem(0,L"Относительная плотность газа по воздуху",0);
		m_list2.InsertItem(1,L"Геометрический участок газопровода"	   ,0);
		m_list2.InsertItem(2,L"Коэффициент сжимаемости газа при Рн"	   ,0);
		m_list2.InsertItem(3,L"Коэффициент сжимаемости газа при Рк"	   ,0);
		m_list2.InsertItem(4,L"Экономия"							   ,0);
		m_list2.InsertItem(5,L"Экономия (денеж.)"					   ,0);
	
		m_list2.SetItemText(0,1,L"-"	  ); m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][10]:L"0,57"));
		m_list2.SetItemText(1,1,L"м3"	  ); m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][11]:L"7693,00"));
		m_list2.SetItemText(2,1,L"-"	  ); m_list2.SetItemText(2,2,((count!=0)?crq.m_RetValues[0][12]:L"0,88"));
		m_list2.SetItemText(3,1,L"-"	  ); m_list2.SetItemText(3,2,((count!=0)?crq.m_RetValues[0][13]:L"0,91"));
		m_list2.SetItemText(4,1,L"тыс.м3" ); m_list2.SetItemText(4,2,((count!=0)?crq.m_RetValues[0][14]:L"146,25"));
		m_list2.SetItemText(5,1,L"тыс.руб"); m_list2.SetItemText(5,2,((count!=0)?crq.m_RetValues[0][15]:L"555,62"));

		e101();

		break;

	case 81:

		m_list1->InsertColumn(0,L"Параметр",LVCFMT_LEFT,400);
		m_list1->InsertColumn(1,L"Е.И."	   ,LVCFMT_LEFT,115);
		m_list1->InsertColumn(2,L"Значение",LVCFMT_LEFT,110);

		s.Format(
			L"SELECT "
			L"T,NNOM,NNOM0,NNOM1,QRN,"
			L"KPD,KEF,PRICE,COSTS,CURTIME,"
			L"KNE,KNE1,"
			L"KNE,KNE1,KKR,KKR,RESULT,"
			L"ECONOMY FROM 81 "
			L"WHERE ID_PLAN_INFO=%s",
			ID_INFO);

		if(db.m_iStatus==FACT)
		{
			s.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
			s.Replace(L"FROM 81",L"FROM 81F");
		}
		//AfxMessageBox(s);
		db.fQueryRead(s,crq);

		//AfxMessageBox(L"!!!");

		count=crq.m_lRecCount;

		

		m_list1->InsertItem(0,L"Планируемая наработка ГПА после ремонта"											  ,0); 
		m_list1->InsertItem(1,L"Номинальная мощность ГПА"															  ,0); 
		m_list1->InsertItem(2,L"Фактическая номинальная мощность ГПА до ремонта"									  ,0);  
		m_list1->InsertItem(3,L"Фактическая номинальная мощность после ремонта"										  ,0);  
		m_list1->InsertItem(4,L"Низшая теплота сгорания"															  ,0);  
		m_list1->InsertItem(5,L"КПД ГТУ"																			  ,0);
		m_list1->InsertItem(6,L"Коэффициент учитывающий загрузку, тех.состояние ГТУ и другие эксплуатационные факторы",0);
		m_list1->InsertItem(7,L"Стоимость 1 тыс.м3 газа"															  ,0);  
		m_list1->InsertItem(8,L"Затраты"																			  ,0); 
		m_list1->InsertItem(9,L"Время работы после мероприятия"														  ,0);  

		m_list1->SetItemText(0,1,L"ч"	    ); 
		m_list1->SetItemText(1,1,L"кВт"	    ); 
		m_list1->SetItemText(2,1,L"кВт"		); 
		m_list1->SetItemText(3,1,L"кВт"	    ); 
		m_list1->SetItemText(4,1,L"ккал/м3"	); 
		m_list1->SetItemText(5,1,L"-"		); 
		m_list1->SetItemText(6,1,L"-"	    ); 
		m_list1->SetItemText(7,1,L"тыс.руб."); 
		m_list1->SetItemText(8,1,L"тыс.руб.");  
		m_list1->SetItemText(9,1,L"ч"		); 

		m_list1->SetItemText(0,2,((count!=0)?crq.m_RetValues[0][0]:L"0"));
		m_list1->SetItemText(1,2,((count!=0)?crq.m_RetValues[0][1]:L"0"));
		m_list1->SetItemText(2,2,((count!=0)?crq.m_RetValues[0][2]:L"0"));
		m_list1->SetItemText(3,2,((count!=0)?crq.m_RetValues[0][3]:L"0"));
		m_list1->SetItemText(4,2,((count!=0)?crq.m_RetValues[0][4]:L"0"));
		m_list1->SetItemText(5,2,((count!=0)?crq.m_RetValues[0][5]:L"0"));
		m_list1->SetItemText(6,2,((count!=0)?crq.m_RetValues[0][6]:L"0"));
		m_list1->SetItemText(7,2,((count!=0)?crq.m_RetValues[0][7]:L"0"));
		m_list1->SetItemText(8,2,((count!=0)?crq.m_RetValues[0][8]:L"0"));
		m_list1->SetItemText(9,2,((count!=0)?crq.m_RetValues[0][9]:L"0"));

		m_list2.InsertColumn(0,L"Параметр",LVCFMT_LEFT,300);
		m_list2.InsertColumn(1,L"Е.И."	  ,LVCFMT_LEFT,100);
		m_list2.InsertColumn(2,L"Значение",LVCFMT_LEFT,115);

		m_list2.InsertItem(0,L"Коэффициент технического состояния ГТУ по мощности до ремонта"	,0);
		m_list2.InsertItem(1,L"Коэффициент технического состояния ГТУ по мощности после ремонта",0);
		m_list2.InsertItem(2,L"Коэффициент эффективности комплексного ремонта ГПА"				,0);
		m_list2.InsertItem(3,L"Планируемая экономия топливного газа ГПА"						,0);
		m_list2.InsertItem(4,L"Экономия (денеж.)"												,0);
	
		m_list2.SetItemText(0,1,L"-"	   ); m_list2.SetItemText(0,2,((count!=0)?crq.m_RetValues[0][10]:L"0"));
		m_list2.SetItemText(1,1,L"-"	   ); m_list2.SetItemText(1,2,((count!=0)?crq.m_RetValues[0][11]:L"0"));
		m_list2.SetItemText(2,1,L"-"	   ); m_list2.SetItemText(2,2,((count!=0)?crq.m_RetValues[0][12]:L"0"));
		m_list2.SetItemText(3,1,L"тыс.м3"  ); m_list2.SetItemText(3,2,((count!=0)?crq.m_RetValues[0][13]:L"0"));
		m_list2.SetItemText(4,1,L"тыс.руб" ); m_list2.SetItemText(4,2,((count!=0)?crq.m_RetValues[0][14]:L"0"));

		e81();

		break;

	default:
		break;
	}

	m_list1->GetItemRect(m_list1->GetItemCount()-1,rect,LVIR_BOUNDS);
	m_list1->MoveWindow(10,100,rect.right,rect.bottom);

	m_list2.GetItemRect(m_list2.GetItemCount()-1,rect2,LVIR_BOUNDS);
	m_list2.MoveWindow(rect.right+20,100,rect2.right+20,rect2.bottom+5);
	
	CRect rw;
	pstext=(CStatic*) this->GetDlgItem(IDC_STATIC_INITDATA);
	pstext->GetWindowRect(rw);

	pstext=(CStatic*) this->GetDlgItem(IDC_STATIC_RESULTCALC);
	pstext->SetWindowTextW(L"Результат расчёта");
	pstext->MoveWindow(rect.right + 20,rw.top-30,100,20);

	ClientToScreen(rect);
	ClientToScreen(rect2);

	CButton* bOK;
	CButton* bCANCEL;

	bOK=(CButton*) this->GetDlgItem(IDOK);
	bCANCEL=(CButton*) this->GetDlgItem(IDCANCEL);

	if((rect.bottom>rect2.bottom)||(rect.bottom==rect2.bottom))
	{
		this->MoveWindow(0,0,rect.right + 50 + rect2.right,rect.bottom+200);
		bCANCEL->MoveWindow(rect.right + rect2.right-50,rect.bottom+100,70,40);
		bOK->MoveWindow(rect.right + rect2.right-150,rect.bottom+100,70,40);
	}
	else
	{
		this->MoveWindow(0,0,rect.right + 50 + rect2.right,rect2.bottom+200);
		bCANCEL->MoveWindow(rect.right + rect2.right-50,rect2.bottom+100,70,40);
		bOK->MoveWindow(rect.right + rect2.right-150,rect2.bottom+100,70,40);
	}

	bOK=NULL;
	bCANCEL=NULL;
	delete bOK;
	delete bCANCEL;

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	pstext=NULL;
	delete pstext;

	return false;
}

int CDlgCalcRes::EventMatchFunction(void)
{
#ifdef CONVERTING_ALIAS
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Оптимизация режимов работы компрессорных станций и газопроводов"														   )==0) return 1;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Выработка газа потребителям перед выполнением ремонтных работ"									   )==0) return 2;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Выработка (оттягивание) газа ГПА КС из ремонтируемых участков газопроводов"											   )==0) return 4;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Очистка (промывка) проточной части осевых компрессоров"																   )==0) return 7;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Ремонт газоперекачивающих агрегатов"																	   )==0) return 8;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Применение безрасходной системы продувки пылеуловителей ГРС"															   )==0) return 12;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Оптимизация количества и времени продувок пылеуловителей"																   )==0) return 15;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Режимно-наладочные испытания котельных"																					       )==0) return 17;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Перевод освещения промышленных и бытовых помещений на осветительное оборудование с использованием светодиодов"   		   )==0) return 20;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Оптимизация схемы электроснабжения путем вывода в резерв ТП и отключения силовых трансформаторов, работающих на холостом ходу")==0) return 23;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Оптимизация режимов работы вентиляторов АВО газа и АВО масла"															   )==0) return 25;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Очистка поверхностей теплообмена трубных пучков АВО газа и АВО масла"   				   )==0) return 26;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Использование устройств плавного пуска и частотно регулируемого электропривода АВО масла, АВО газа, насосов водоснабжения")==0) return 27;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Установка конденсаторных батарей с автоматическим управлением"															   )==0) return 38;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Проведение тепловизионного обследования зданий и сооружений на выявление теплопотерь с их последующим устранением"		   )==0) return 42;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Использование утилизаторов тепла работающих газоперекачивающих агрегатов"												   )==0) return 43;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Регулирование температуры в тепловой сети отопления в зависимости от наружной температуры"								   )==0) return 48;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Химическая промывка трубных пучков утилизационных теплообменников ГПА"														   )==0) return 100;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Регулирование температуры в тепловой сети отопления в зависимости от наружной температуры1"								   )==0) return 481;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Использование утилизаторов тепла работающих газоперекачивающих агрегатов1"												   )==0) return 431;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Выработка (оттягивание) газа мобильными компрессорными установками из ремонтируемых участков газопроводов"				   )==0) return 101;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Ремонт газоперекачивающих агрегатов1"																	   )==0) return 81;
#else
	if(StrCmp(db.m_szEventName,L"Оптимизация режимов работы компрессорных станций и газопроводов"														   )==0) return 1;
	if(StrCmp(db.m_szEventName,L"Выработка газа потребителям перед выполнением ремонтных работ"									   )==0) return 2;
	if(StrCmp(db.m_szEventName,L"Выработка (оттягивание) газа ГПА КС из ремонтируемых участков газопроводов"											   )==0) return 4;
	if(StrCmp(db.m_szEventName,L"Очистка (промывка) проточной части осевых компрессоров"																   )==0) return 7;
	if(StrCmp(db.m_szEventName,L"Ремонт газоперекачивающих агрегатов"																	   )==0) return 8;
	if(StrCmp(db.m_szEventName,L"Применение безрасходной системы продувки пылеуловителей ГРС"															   )==0) return 12;
	if(StrCmp(db.m_szEventName,L"Оптимизация количества и времени продувок пылеуловителей"																   )==0) return 15;
	if(StrCmp(db.m_szEventName,L"Режимно-наладочные испытания котельных"																					       )==0) return 17;
	if(StrCmp(db.m_szEventName,L"Перевод освещения промышленных и бытовых помещений на осветительное оборудование с использованием светодиодов"   		   )==0) return 20;
	if(StrCmp(db.m_szEventName,L"Оптимизация схемы электроснабжения путем вывода в резерв ТП и отключения силовых трансформаторов, работающих на холостом ходу")==0) return 23;
	if(StrCmp(db.m_szEventName,L"Оптимизация режимов работы вентиляторов АВО газа и АВО масла"															   )==0) return 25;
	if(StrCmp(db.m_szEventName,L"Очистка поверхностей теплообмена трубных пучков АВО газа и АВО масла"   				   )==0) return 26;
	if(StrCmp(db.m_szEventName,L"Использование устройств плавного пуска и частотно регулируемого электропривода АВО масла, АВО газа, насосов водоснабжения")==0) return 27;
	if(StrCmp(db.m_szEventName,L"Установка конденсаторных батарей с автоматическим управлением"															   )==0) return 38;
	if(StrCmp(db.m_szEventName,L"Проведение тепловизионного обследования зданий и сооружений на выявление теплопотерь с их последующим устранением"		   )==0) return 42;
	if(StrCmp(db.m_szEventName,L"Использование утилизаторов тепла работающих газоперекачивающих агрегатов"												   )==0) return 43;
	if(StrCmp(db.m_szEventName,L"Регулирование температуры в тепловой сети отопления в зависимости от наружной температуры"								   )==0) return 48;
	if(StrCmp(db.m_szEventName,L"Химическая промывка трубных пучков утилизационных теплообменников ГПА"														   )==0) return 100;
	if(StrCmp(db.m_szEventName,L"Регулирование температуры в тепловой сети отопления в зависимости от наружной температуры1"								   )==0) return 481;
	if(StrCmp(db.m_szEventName,L"Использование утилизаторов тепла работающих газоперекачивающих агрегатов1"												   )==0) return 431;
	if(StrCmp(db.m_szEventName,L"Выработка (оттягивание) газа мобильными компрессорными установками из ремонтируемых участков газопроводов"				   )==0) return 101;
	if(StrCmp(db.m_szEventName,L"Ремонт газоперекачивающих агрегатов1"																	   )==0) return 81;
#endif
	return 0;
}

void CDlgCalcRes::CalcResult(void)
{
	switch(m_inumEvent)
	{
	case 100:
		e100();
		break;
	case 1:
		e1();
		break;
	case 2:
		e2();
		break;
	case 4:
		e4();
		break;
	case 7:
		e7();
		break;
	case 8:
		e8();
		break;
	case 12:
		e12();
		break;
	case 15:
		e15();
		break;
	case 17:
		e17();
		break;
	case 20:
		e20();
		break;
	case 23:
		e23();
		break;
	case 25:
		e25();
		break;
	case 26:
		e26();
		break;
	case 27:
		e27();
		break;
	case 38:
		e38();
		break;
	case 42:
		e42();
		break;
	case 43:
		e43();
		break;
	case 48:
		e48();
		break;
	case 481:
		e481();
		break;
	case 431:
		e431();
		break;
	case 101:
		e101();
		break;
	case 81:
		e81();
		break;
	default:
		break;
	}
}

afx_msg LRESULT CDlgCalcRes::OnUpdateresult(WPARAM wParam, LPARAM lParam)
{
	CalcResult();
	return 0;
}

void CDlgCalcRes::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	switch (m_inumEvent)
	{
	case 100:
		e100Rec();
		break;
	case 1:
		e1Rec();
		break;
	case 2:
		e2Rec();
		break;
	case 4:
		e4Rec();
		break;
	case 7:
		e7Rec();
		break;
	case 8:
		e8Rec();
		break;
	case 12:
		e12Rec();
		break;
	case 15:
		e15Rec();
		break;
	case 17:
		e17Rec();
		break;
	case 20:
		e20Rec();
		break;
	case 23:
		e23Rec();
		break;
	case 25:
		e25Rec();
		break;
	case 26:
		e26Rec();
		break;
	case 27:
		e27Rec();
		break;
	case 38:
		e38Rec();
		break;
	case 42:
		e42Rec();
		break;
	case 43:
		e43Rec();
		break;
	case 48:
		e48Rec();
		break;
	case 481:
		e481Rec();
		break;
	case 431:
		e431Rec();
		break;
	case 101:
		e101Rec();
		break;
	case 81:
		e81Rec();
		break;
	default:
		break;
	}
	CDialog::OnOK();
}

void CDlgCalcRes::e1(void)
{
	double* bRtg=new double[8];
	double* bT=new double[8];
	double* aRtg=new double[8];
	double* aT=new double[8];

	double bRes=0;
	double aRes=0;

	double price;
//	double cost;
//	double twork;

	double Result;
	double Economy;
		
	CString szBuffer;
///	CRetQuery crq;

	/*
	szBuffer.Format(
		L"SELECT FROM 1 "
		L"Q1b,Q2b,Q3b,Q4b,Q5b,Q6b,Q7b,Q8b,"
		L"Q1a,Q2a,Q3a,Q4a,Q5a,Q6a,Q7a,Q8a,"
		L"T1b,T2b,T3b,T4b,T5b,T6b,T7b,T8b,"
		L"T1a,T2a,T3a,T4a,T5a,T6a,T7a,T8a,"
		L"PRICE,COSTS,dQ0,dQ1,RESULT,ECONOMY,CURTIME WHERE ID_PLAN_INFO IN "
		L"(SELECT ID_PLAN_INFO FROM PLAN_INFO WHERE NYEAR=%s AND NQUART=%s AND EVENT=%s)",
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
	db.fQueryRead(szBuffer,crq);
	*/
	


	for(int i=1;i<9;i++)
	{
		bRtg[i-1]=_wtof(m_list1->GetItemText(i,2));
		bT[i-1]=_wtof(m_list1->GetItemText(i,3));
	}

	for(int i=10;i<18;i++)
	{
		aRtg[i-10]=_wtof(m_list1->GetItemText(i,2));
		aT[i-10]=_wtof(m_list1->GetItemText(i,3));
	}

	for(int i=0;i<8;i++)
	{
		bRes+=(bRtg[i]*bT[i]);
		aRes+=(aRtg[i]*aT[i]);
	}

	price=_wtof(m_list1->GetItemText(18,3));

	bRes=bRes/1000;

	aRes/=1000;

	Result=bRes-aRes;

	Economy=Result*price;

	szBuffer.Format(L"%.2f",bRes   ); m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.2f",aRes   ); m_list2.SetItemText(1,2,szBuffer);
	szBuffer.Format(L"%.3f",Result ); m_list2.SetItemText(2,2,szBuffer);
	szBuffer.Format(L"%.3f",Economy); m_list2.SetItemText(3,2,szBuffer);

	delete [] bRtg;
	delete [] bT;
	delete [] aRtg;
	delete [] aT;

	
}

bool CDlgCalcRes::e1Rec(void)
{		
	CString szBuffer;

	CRetQuery crq;
	CString ID_INFO;
	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
		db.fQueryRead(szBuffer,crq);
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
		db.fQueryRead(szBuffer,crq);
#endif
	}
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
		db.fQueryRead(szBuffer,crq);
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
		db.fQueryRead(szBuffer,crq);
#endif


	}
		ID_INFO=crq.m_RetValues[0][0];
	
	szBuffer.Format(
		L"UPDATE 1 SET "
		L"Q1b=%s, Q2b=%s, Q3b=%s, Q4b=%s, Q5b=%s, Q6b=%s, Q7b=%s, Q8b=%s, "
		L"Q1a=%s, Q2a=%s, Q3a=%s, Q4a=%s, Q5a=%s, Q6a=%s, Q7a=%s, Q8a=%s, "
		L"T1b=%s, T2b=%s, T3b=%s, T4b=%s, T5b=%s, T6b=%s, T7b=%s, T8b=%s, "
		L"T1a=%s, T2a=%s, T3a=%s, T4a=%s, T5a=%s, T6a=%s, T7a=%s, T8a=%s, "
		L"PRICE=%s, COSTS=%s, dQ0=%s, dQ1=%s, RESULT=%s, ECONOMY=%s, CURTIME=%s "
		L"WHERE ID_PLAN_INFO=%s",
		m_list1->GetItemText(1,2),
		m_list1->GetItemText(2,2),
		m_list1->GetItemText(3,2),
		m_list1->GetItemText(4,2),
		m_list1->GetItemText(5,2),
		m_list1->GetItemText(6,2),
		m_list1->GetItemText(7,2),
		m_list1->GetItemText(8,2),
		m_list1->GetItemText(10,2),
		m_list1->GetItemText(11,2),
		m_list1->GetItemText(12,2),
		m_list1->GetItemText(13,2),
		m_list1->GetItemText(14,2),
		m_list1->GetItemText(15,2),
		m_list1->GetItemText(16,2),
		m_list1->GetItemText(17,2),
		m_list1->GetItemText(1,3),
		m_list1->GetItemText(2,3),
		m_list1->GetItemText(3,3),
		m_list1->GetItemText(4,3),
		m_list1->GetItemText(5,3),
		m_list1->GetItemText(6,3),
		m_list1->GetItemText(7,3),
		m_list1->GetItemText(8,3),
		m_list1->GetItemText(10,3),
		m_list1->GetItemText(11,3),
		m_list1->GetItemText(12,3),
		m_list1->GetItemText(13,3),
		m_list1->GetItemText(14,3),
		m_list1->GetItemText(15,3),
		m_list1->GetItemText(16,3),
		m_list1->GetItemText(17,3),
		m_list1->GetItemText(18,3),
		m_list1->GetItemText(19,3),
		m_list2.GetItemText(0,2),
		m_list2.GetItemText(1,2),
		m_list2.GetItemText(2,2),
		m_list2.GetItemText(3,2),
		m_list1->GetItemText(20,3),
		ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
		szBuffer.Replace(L"UPDATE 1",L"UPDATE 1F");
	}

	db.ExeSql(szBuffer);

	szBuffer.Format(
		L"UPDATE PLAN_INFO SET "
		L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
		L"WHERE ID_PLAN_INFO=%s",
		m_list2.GetItemText(2,2),
		m_list2.GetItemText(3,2),
		m_list1->GetItemText(19,3),
		m_list1->GetItemText(20,3),
		ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return 0;
}

bool CDlgCalcRes::e7Rec(void)
{		
	CString szBuffer;

	CRetQuery crq;

	CString ID_INFO;
	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
		//db.fQueryRead(szBuffer,crq);
	}
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName,ALIAS_TO_NAMES);
#endif
	}
		db.fQueryRead(szBuffer,crq);

		ID_INFO=crq.m_RetValues[0][0];

	szBuffer.Format(
		L"UPDATE 7 SET "
		L"NMONTH=%s,NPROM=%s,KLOAD=%s,N=%s,Kdepression=%s,Q=%s,COUNT=%s,PRICE=%s,COSTS=%s,CURTIME=%s,KECTG=%s,L=%s,RESULT=%s,ECONOMY=%s "
		L"WHERE ID_PLAN_INFO=%s",
		m_list1->GetItemText(0,2),
		m_list1->GetItemText(1,2),
		m_list1->GetItemText(2,2),
		m_list1->GetItemText(3,2),
		m_list1->GetItemText(4,2),
		m_list1->GetItemText(5,2),
		m_list1->GetItemText(6,2),
		m_list1->GetItemText(7,2),
		m_list1->GetItemText(8,2),
		m_list1->GetItemText(9,2),
		m_list2.GetItemText(0,2),
		m_list2.GetItemText(1,2),
		m_list2.GetItemText(2,2),
		m_list2.GetItemText(3,2),
		ID_INFO);

	//AfxMessageBox(szBuffer);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
		szBuffer.Replace(L"UPDATE 7",L"UPDATE 7F");
	}

	db.ExeSql(szBuffer);

	szBuffer.Format(
		L"UPDATE PLAN_INFO SET "
		L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
		L"WHERE ID_PLAN_INFO=%s",
		m_list2.GetItemText(2,2),
		m_list2.GetItemText(3,2),
		m_list1->GetItemText(8,2),
		m_list1->GetItemText(9,2),
		ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return 0;
}

void CDlgCalcRes::e7(void)
{
	CString szBuffer;

	double NMONTH=		_wtof(m_list1->GetItemText(0,2));
	double NPROM=		_wtof(m_list1->GetItemText(1,2));
	double KLOAD=		_wtof(m_list1->GetItemText(2,2));
	double N=			_wtof(m_list1->GetItemText(3,2));
	double Kdepression= _wtof(m_list1->GetItemText(4,2));
	double Q=			_wtof(m_list1->GetItemText(5,2));
	double COUNT=		_wtof(m_list1->GetItemText(6,2));
	double PRICE=		_wtof(m_list1->GetItemText(7,2));
	double COSTS=		_wtof(m_list1->GetItemText(8,2));
	double CURTIME=		_wtof(m_list1->GetItemText(9,2));

	double KECTG = 2.08725E-20*pow(NPROM,6)-2.148376E-16*pow(NPROM,5)+0.000000000000890613*pow(NPROM,4)-0.000000001927915*pow(NPROM,3)+0.0000023698*pow(NPROM,2)-0.00169805*NPROM+0.999345;	
	double LOAD = KLOAD*N;
	double RESULT = KECTG*Kdepression*NMONTH*LOAD/(Q*1000)*COUNT;
	double ECONOMY = (RESULT*PRICE);

	szBuffer.Format(L"%.2f",KECTG  ); m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.2f",LOAD   ); m_list2.SetItemText(1,2,szBuffer);
	szBuffer.Format(L"%.3f",RESULT ); m_list2.SetItemText(2,2,szBuffer);
	szBuffer.Format(L"%.3f",ECONOMY); m_list2.SetItemText(3,2,szBuffer);
}

bool CDlgCalcRes::e8(void)
{
	CString szBuffer;

	double NMONTH=		_wtof(m_list1->GetItemText(0,2));
	double NREPAIR=		_wtof(m_list1->GetItemText(1,2));
	double KLOAD=		_wtof(m_list1->GetItemText(2,2));
	double N=			_wtof(m_list1->GetItemText(3,2));
	double Kdepression= _wtof(m_list1->GetItemText(4,2));
	double Q=			_wtof(m_list1->GetItemText(5,2));
	double PRICE=		_wtof(m_list1->GetItemText(6,2));
	double COSTS=		_wtof(m_list1->GetItemText(6,2));
	double CURTIME=		_wtof(m_list1->GetItemText(7,2));

	double KECTG = 2.08725E-20*pow(NREPAIR,6) - 2.148376E-16*pow(NREPAIR,5) + 0.000000000000890613*pow(NREPAIR,4) - 0.000000001927915*pow(NREPAIR,3) + 0.0000023698*pow(NREPAIR,2) - 0.00169805*NREPAIR + 0.999345;
	double LOAD = KLOAD*N;
	double RESULT = KECTG*Kdepression*NMONTH*LOAD/(Q*1000);
	double ECONOMY  = (RESULT*PRICE);

	szBuffer.Format(L"%.2f",KECTG  ); m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.2f",LOAD   ); m_list2.SetItemText(1,2,szBuffer);
	szBuffer.Format(L"%.3f",RESULT ); m_list2.SetItemText(2,2,szBuffer);
	szBuffer.Format(L"%.3f",ECONOMY); m_list2.SetItemText(3,2,szBuffer);

	return false;
}

bool CDlgCalcRes::e8Rec(void)
{
	CString szBuffer;

	CRetQuery crq;

	CString ID_INFO;

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}
	
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
#endif
	}

	db.fQueryRead(szBuffer,crq);
	
	ID_INFO=crq.m_RetValues[0][0];

	szBuffer.Format(
		L"UPDATE 8 SET "
		L"NMONTH=%s, NREPAIR=%s, KLOAD=%s, NOM=%s, Kdepression=%s, QP=%s, PRICE=%s, COSTS=%s, CURTIME=%s, KECTG=%s, LOAD=%s, RESULT=%s, ECONOMY=%s "
		L"WHERE ID_PLAN_INFO=%s",
		m_list1->GetItemText(0,2),
		m_list1->GetItemText(1,2),
		m_list1->GetItemText(2,2),
		m_list1->GetItemText(3,2),
		m_list1->GetItemText(4,2),
		m_list1->GetItemText(5,2),
		m_list1->GetItemText(6,2),
		m_list1->GetItemText(7,2),
		m_list1->GetItemText(8,2),
		m_list2.GetItemText(0,2),
		m_list2.GetItemText(1,2),
		m_list2.GetItemText(2,2),
		m_list2.GetItemText(3,2),
		ID_INFO);
	//AfxMessageBox(szBuffer);
	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 8",L"UPDATE 8F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	db.ExeSql(szBuffer);

	

	szBuffer.Format(
		L"UPDATE PLAN_INFO SET "
		L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
		L"WHERE ID_PLAN_INFO=%s",
		m_list2.GetItemText(2,2),
		m_list2.GetItemText(3,2),
		m_list1->GetItemText(7,2),
		m_list1->GetItemText(8,2),
		ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}
	//AfxMessageBox(szBuffer);
	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}

bool CDlgCalcRes::e12(void)
{
	CString szBuffer;
	double T	   =_wtof(m_list1->GetItemText(0,2));
	double P	   =_wtof(m_list1->GetItemText(1,2));
	double P0	   =_wtof(m_list1->GetItemText(2,2));
	double P1	   =_wtof(m_list1->GetItemText(3,2)); 
	double T0	   =_wtof(m_list1->GetItemText(4,2)); 
	double T1	   =_wtof(m_list1->GetItemText(5,2)); 
	double QP	   =_wtof(m_list1->GetItemText(6,2)); 
	double V1	   =_wtof(m_list1->GetItemText(7,2));
	double V2	   =_wtof(m_list1->GetItemText(8,2));
	double V3	   =_wtof(m_list1->GetItemText(9,2)); 
	double PRICE   =_wtof(m_list1->GetItemText(11,2)); 
	double COSTS   =_wtof(m_list1->GetItemText(12,2)); 
	double CURTIME =_wtof(m_list1->GetItemText(13,2));
	double n	   =_wtof(m_list1->GetItemText(10,2));
	
	double DQP	   =QP/1.206;
	double TSR0	   =T0+273.15;
	double TSR1	   =T1+273.15;
	double PSR0	   =P0*0.0981;
	double PSR1	   =P1*0.0981;
	double Z0	   =1-(((10.2*PSR0-6)*(0.345*0.01*DQP-0.446*0.001)+0.015)*(1.3-0.0144*(TSR0-283.2)));
	double Z1	   =1-(((10.2*PSR1-6)*(0.345*0.01*DQP-0.446*0.001)+0.015)*(1.3-0.0144*(TSR1-283.2)));
	double Q0	   =((V1+V2+V3)*PSR0*(T+273.15))/(Z0*TSR0*P*1000);
	double Q1	   =((V1+V2+V3)*PSR1*(T+273.15))/(Z1*TSR1*P*1000);
	double Q01	   =Q0*n;
	double Q11	   =Q1*n;
	double RESULT  =Q01-Q11;
	double ECONOMY =RESULT*PRICE;

	

	szBuffer.Format(L"%.2f",DQP);
	m_list2.SetItemText(0,2,szBuffer);

	szBuffer.Format(L"%.2f",TSR0);
	m_list2.SetItemText(1,2,szBuffer);
	
	szBuffer.Format(L"%.2f",TSR1);
	m_list2.SetItemText(2,2,szBuffer);

	szBuffer.Format(L"%.2f",PSR0);
	m_list2.SetItemText(3,2,szBuffer);
	
	szBuffer.Format(L"%.2f",PSR1);
	m_list2.SetItemText(4,2,szBuffer);

	szBuffer.Format(L"%.2f",Z0);
	m_list2.SetItemText(5,2,szBuffer);
	
	szBuffer.Format(L"%.2f",Z1);
	m_list2.SetItemText(6,2,szBuffer);

	szBuffer.Format(L"%.2f",Q0);
	m_list2.SetItemText(7,2,szBuffer);

	szBuffer.Format(L"%.2f",Q1);
	m_list2.SetItemText(8,2,szBuffer);

	szBuffer.Format(L"%.2f",Q01);
	m_list2.SetItemText(9,2,szBuffer);

	szBuffer.Format(L"%.2f",Q11);
	m_list2.SetItemText(10,2,szBuffer);

	szBuffer.Format(L"%.3f",RESULT);
	m_list2.SetItemText(11,2,szBuffer);

	szBuffer.Format(L"%.3f",ECONOMY);
	m_list2.SetItemText(12,2,szBuffer);


	return false;
}

bool CDlgCalcRes::e12Rec(void)
{
	CString szBuffer;
	CString ID_INFO;
	CRetQuery crq;

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
#endif
	}
	//AfxMessageBox(szBuffer);

	db.fQueryRead(szBuffer,crq);
	
	ID_INFO=crq.m_RetValues[0][0];
	
	szBuffer.Format(
	L"UPDATE 12 SET "
	L"T=%s, P=%s, P0=%s, P1=%s, T0=%s, T1=%s, QP=%s, V1=%s, V2=%s, V3=%s, DQP=%s, TSR0=%s, TSR1=%s, PSR0=%s, PSR1=%s,"
	L"Z0=%s, Z1=%s, Q0=%s, Q1=%s, n=%s, Q01=%s, Q11=%s, RESULT=%s, PRICE=%s, COSTS=%s, ECONOMY=%s, CURTIME=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list1->GetItemText(0,2),//T
	m_list1->GetItemText(1,2),//P
	m_list1->GetItemText(2,2),//P0
	m_list1->GetItemText(3,2),//P1
	m_list1->GetItemText(4,2),//T0
	m_list1->GetItemText(5,2),//T1
	m_list1->GetItemText(6,2),//QP
	m_list1->GetItemText(7,2),//V1
	m_list1->GetItemText(8,2),//V2
	m_list1->GetItemText(9,2),//V3
	m_list2.GetItemText(0,2),//DQP
	m_list2.GetItemText(1,2),//TSR0
	m_list2.GetItemText(2,2),//TSR1
	m_list2.GetItemText(3,2),//PSR0
	m_list2.GetItemText(4,2),//PSR1
	m_list2.GetItemText(5,2),//Z0
	m_list2.GetItemText(6,2),//Z1
	m_list2.GetItemText(7,2),//Q0
	m_list2.GetItemText(8,2),//Q1
	m_list1->GetItemText(10,2),//n
	m_list2.GetItemText(9,2),//Q01
	m_list2.GetItemText(10,2),//Q11
	m_list2.GetItemText(11,2),//RESULT
	m_list1->GetItemText(11,2),//PRICE
	m_list1->GetItemText(12,2),//COSTS
	m_list2.GetItemText(12,2),//ECONOMY
	m_list1->GetItemText(13,2),//CURTIME
	ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 12",L"UPDATE 12F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}
	//AfxMessageBox(szBuffer);
	db.ExeSql(szBuffer);

	szBuffer.Format(
		L"UPDATE PLAN_INFO SET "
		L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
		L"WHERE ID_PLAN_INFO=%s",
		m_list2.GetItemText(11,2),
		m_list2.GetItemText(12,2),
		m_list1->GetItemText(12,2),
		m_list1->GetItemText(13,2),
		ID_INFO);

	//AfxMessageBox(szBuffer);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	db.ExeSql(szBuffer);

	//AfxMessageBox(L"sds");

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}

bool CDlgCalcRes::e15(void)
{
	CString szBuffer;
	double P0	     =_wtof(m_list1->GetItemText(0,2)); 
	double D0	     =_wtof(m_list1->GetItemText(1,2)); 
	double L0	     =_wtof(m_list1->GetItemText(2,2));
	double T0	     =_wtof(m_list1->GetItemText(3,2));
	double G	     =_wtof(m_list1->GetItemText(4,2));
	double KRESIST   =_wtof(m_list1->GetItemText(5,2)); 
	double SHEROH    =_wtof(m_list1->GetItemText(6,2)); 
	double PATM	     =_wtof(m_list1->GetItemText(7,2)); 
	double PPG	     =_wtof(m_list1->GetItemText(8,2)); 
	double TPROD     =_wtof(m_list1->GetItemText(9,2));
	double NEMK		 =_wtof(m_list1->GetItemText(10,2)); 
	double NOMNPROD  =_wtof(m_list1->GetItemText(11,2)); 
	double FACTNPROD =_wtof(m_list1->GetItemText(12,2)); 
	double PRICE	 =_wtof(m_list1->GetItemText(13,2));

	double POPSECH;
	double KGIDR;
	double QSTD;
	double K_DP1P0;
	double K_DT1P0;
	double K_DL1P0;
	double KDD1D0;

	double RESULT;
	double ECONOMY;

	POPSECH=3.14*D0*D0/4;
	KGIDR=0.11*pow((SHEROH/D0),0.25);
	QSTD=(pow(10,6)*POPSECH/PPG)*pow((pow(KRESIST,2)*pow(P0,2)-pow(PATM,2))*(D0/(KGIDR*L0*G*T0)),0.5);
	K_DP1P0=P0/5.5;
	K_DT1P0=pow((293/T0),0.5);
	K_DL1P0=pow(100/L0,0.5);
	KDD1D0=pow(D0,2)/0.0025;
	RESULT=NEMK*(NOMNPROD-FACTNPROD)*QSTD*K_DL1P0*K_DP1P0*K_DT1P0*KDD1D0*TPROD/1000;
	ECONOMY=RESULT*PRICE;

	szBuffer.Format(L"%.3f",POPSECH);
	m_list2.SetItemText(0,2,szBuffer);

	szBuffer.Format(L"%.3f",KGIDR);
	m_list2.SetItemText(1,2,szBuffer);

	szBuffer.Format(L"%.3f",QSTD);
	m_list2.SetItemText(2,2,szBuffer);

	szBuffer.Format(L"%.3f",K_DP1P0);
	m_list2.SetItemText(3,2,szBuffer);

	szBuffer.Format(L"%.3f",K_DT1P0);
	m_list2.SetItemText(4,2,szBuffer);

	szBuffer.Format(L"%.3f",K_DL1P0);
	m_list2.SetItemText(5,2,szBuffer);

	szBuffer.Format(L"%.3f",KDD1D0);
	m_list2.SetItemText(6,2,szBuffer);

	szBuffer.Format(L"%.3f",RESULT);
	m_list2.SetItemText(7,2,szBuffer);

	szBuffer.Format(L"%.3f",ECONOMY);
	m_list2.SetItemText(8,2,szBuffer);



	return false;
}

bool CDlgCalcRes::e15Rec(void)
{
	CString szBuffer;
	CString ID_INFO;
	CRetQuery crq;

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
#endif
	}
	db.fQueryRead(szBuffer,crq);

	ID_INFO=crq.m_RetValues[0][0];
	
	szBuffer.Format(L"UPDATE 15 SET "
	L"P0=%s, D0=%s, L0=%s, T0=%s, G=%s, KRESIST=%s, SHEROH=%s, PATM=%s, PPG=%s, TPROD=%s,"
	L"NEMK=%s, NOMNPROD=%s, FACTNPROD=%s, PRICE=%s, COSTS=%s, CURTIME=%s " 
	L"WHERE ID_PLAN_INFO=%s",
	m_list1->GetItemText(0,2),
	m_list1->GetItemText(1,2),
	m_list1->GetItemText(2,2),
	m_list1->GetItemText(3,2),
	m_list1->GetItemText(4,2),
	m_list1->GetItemText(5,2),
	m_list1->GetItemText(6,2),
	m_list1->GetItemText(7,2),
	m_list1->GetItemText(8,2),
	m_list1->GetItemText(9,2),
	m_list1->GetItemText(10,2),
	m_list1->GetItemText(11,2),
	m_list1->GetItemText(12,2),
	m_list1->GetItemText(13,2),
	m_list1->GetItemText(14,2),
	m_list1->GetItemText(15,2),
	ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 15",L"UPDATE 15F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	//AfxMessageBox(szBuffer);

	db.ExeSql(szBuffer);

	szBuffer.Format(
		L"UPDATE PLAN_INFO SET "
		L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
		L"WHERE ID_PLAN_INFO=%s",
		m_list2.GetItemText(7,2),
		m_list2.GetItemText(8,2),
		m_list1->GetItemText(14,2),
		m_list1->GetItemText(15,2),
		ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}

bool CDlgCalcRes::e25(void)
{
	CString szBuffer;

	double N0;
	double NSD; 
	double KPD; 
	double PRICE; 
	double COSTS; 
	double CURTIME;
	
	double DW1; 
	double RESULT; 
	double ECONOMY;
	
	N0 =	 _wtof(m_list1->GetItemText(0,2)); 
	NSD	=	 _wtof(m_list1->GetItemText(1,2));
	KPD =	 _wtof(m_list1->GetItemText(2,2));
	PRICE =  _wtof(m_list1->GetItemText(3,2));
	COSTS =	 _wtof(m_list1->GetItemText(4,2));
	CURTIME =_wtof(m_list1->GetItemText(5,2));

	RESULT=DW1=(NSD*N0*KPD/100)/1000;
	ECONOMY=RESULT*PRICE;

	szBuffer.Format(L"%.3f",N0);      m_list1->SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.3f",NSD);     m_list1->SetItemText(1,2,szBuffer);
	szBuffer.Format(L"%.3f",KPD);     m_list1->SetItemText(2,2,szBuffer);
	szBuffer.Format(L"%.3f",PRICE);   m_list1->SetItemText(4,2,szBuffer);
	szBuffer.Format(L"%.3f",COSTS);	  m_list1->SetItemText(4,2,szBuffer);
	szBuffer.Format(L"%.3f",CURTIME); m_list1->SetItemText(5,2,szBuffer);
	szBuffer.Format(L"%.3f",DW1);	  m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.3f",RESULT);  m_list2.SetItemText(1,2,szBuffer); 
	szBuffer.Format(L"%.3f",ECONOMY); m_list2.SetItemText(2,2,szBuffer);

	return false;
}

bool CDlgCalcRes::e25Rec(void)
{
	CString szBuffer;
	CString ID_INFO;
	CRetQuery crq;

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName,ALIAS_TO_NAMES);
#endif
	}
	db.fQueryRead(szBuffer,crq);

	ID_INFO=crq.m_RetValues[0][0];
	
	//AfxMessageBox(L"Begin");

	szBuffer.Format(L"UPDATE 25 SET "
	L"N0=%s, NSD=%s, KPD=%s, PRICE=%s, COSTS=%s, CURTIME=%s, "
	L"DW1=%s, RESULT=%s, ECONOMY=%s " 
	L"WHERE ID_PLAN_INFO=%s",
	m_list1->GetItemText(0,2),
	m_list1->GetItemText(1,2),
	m_list1->GetItemText(2,2),
	m_list1->GetItemText(3,2),
	m_list1->GetItemText(4,2),
	m_list1->GetItemText(5,2),
	m_list2.GetItemText(0,2),
	m_list2.GetItemText(1,2),
	m_list2.GetItemText(2,2),
	ID_INFO);

	//AfxMessageBox(szBuffer);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 25",L"UPDATE 25F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	db.ExeSql(szBuffer);

	szBuffer.Format(
		L"UPDATE PLAN_INFO SET "
		L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
		L"WHERE ID_PLAN_INFO=%s",
		m_list2.GetItemText(1,2),
		m_list2.GetItemText(2,2),
		m_list1->GetItemText(4,2),
		m_list1->GetItemText(5,2),
		ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}

bool CDlgCalcRes::e27(void)
{
	CString szBuffer;

	double NAVO		=_wtof(m_list1->GetItemText(0,2)); 
	double NAVOMAX	=_wtof(m_list1->GetItemText(1,2)); 
	double DTINOUT	=_wtof(m_list1->GetItemText(2,2)); 
	double DTMAX	=_wtof(m_list1->GetItemText(3,2)); 
	double b		=_wtof(m_list1->GetItemText(4,2));
	double N		=_wtof(m_list1->GetItemText(5,2)); 
	double T		=_wtof(m_list1->GetItemText(6,2)); 
	double PRICE	=_wtof(m_list1->GetItemText(7,2)); 
	double COSTS	=_wtof(m_list1->GetItemText(8,2)); 
	double CURTIME	=_wtof(m_list1->GetItemText(9,2));
	
	double KLOADNH	=pow(DTINOUT/DTMAX,3/b); 
	double KLOADND	=pow(KLOADNH,b/3); 
	
	double DRES		=N*(KLOADND-KLOADNH); 
	double RESULT	=T*DRES/1000; 
	double ECONOMY	=RESULT*PRICE;

	szBuffer.Format(L"%.3f",KLOADNH); m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.3f",KLOADND); m_list2.SetItemText(1,2,szBuffer);
	szBuffer.Format(L"%.3f",DRES   ); m_list2.SetItemText(2,2,szBuffer);
	szBuffer.Format(L"%.3f",RESULT ); m_list2.SetItemText(3,2,szBuffer);
	szBuffer.Format(L"%.3f",ECONOMY); m_list2.SetItemText(4,2,szBuffer);

	return false;
}

bool CDlgCalcRes::e27Rec(void)
{
	CString szBuffer;
	CString ID_INFO;
	CRetQuery crq;

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
#endif
	}
	db.fQueryRead(szBuffer,crq);

	ID_INFO=crq.m_RetValues[0][0];
	
	szBuffer.Format(L"UPDATE 27 SET "
	L"NAVO=%s,NAVOMAX=%s,DTINOUT=%s,DTMAX=%s,b=%s,"
	L"N=%s,T=%s,PRICE=%s,COSTS=%s,CURTIME=%s,"
	L"KLOADNH=%s,KLOADND=%s,DRES=%s,RESULT=%s,ECONOMY=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list1->GetItemText(0,2),
	m_list1->GetItemText(1,2),
	m_list1->GetItemText(2,2),
	m_list1->GetItemText(3,2),
	m_list1->GetItemText(4,2),
	m_list1->GetItemText(5,2),
	m_list1->GetItemText(6,2),
	m_list1->GetItemText(7,2),
	m_list1->GetItemText(8,2),
	m_list1->GetItemText(9,2),
	m_list2.GetItemText(0,2),
	m_list2.GetItemText(1,2),
	m_list2.GetItemText(2,2),
	m_list2.GetItemText(3,2),
	m_list2.GetItemText(4,2),
	ID_INFO);

	//AfxMessageBox(szBuffer);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 27",L"UPDATE 27F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}
	db.ExeSql(szBuffer);

	szBuffer.Format(
	L"UPDATE PLAN_INFO SET "
	L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list2.GetItemText(3,2),
	m_list2.GetItemText(4,2),
	m_list1->GetItemText(8,2),
	m_list1->GetItemText(9,2),
	ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}

bool CDlgCalcRes::e38(void)
{
	CString szBuffer;

	double EKRN		=_wtof(m_list1->GetItemText(0,2));
	double WKC		=_wtof(m_list1->GetItemText(1,2)); 
	double TGANGLEF1=_wtof(m_list1->GetItemText(2,2)); 
	double TGANGLEF2=_wtof(m_list1->GetItemText(3,2)); 
	double PRICE	=_wtof(m_list1->GetItemText(4,2));
	double COSTS	=_wtof(m_list1->GetItemText(5,2)); 
	double CURTIME	=_wtof(m_list1->GetItemText(6,2)); 

	double RESULT=0.001*EKRN*WKC*(TGANGLEF1-TGANGLEF2);
	double ECONOMY=RESULT*PRICE;

	szBuffer.Format(L"%.3f",RESULT); m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.3f",ECONOMY); m_list2.SetItemText(1,2,szBuffer);

	return false;
}

bool CDlgCalcRes::e38Rec(void)
{
	CString szBuffer;
	CString ID_INFO;
	CRetQuery crq;

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}

	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
#endif
	}
	db.fQueryRead(szBuffer,crq);

	ID_INFO=crq.m_RetValues[0][0];
	
	szBuffer.Format(L"UPDATE 38 SET "
	L"EKRN=%s,WKC=%s,TGANGLEF1=%s,TGANGLEF2=%s,PRICE=%s,"
	L"COSTS=%s,CURTIME=%s,RESULT=%s,ECONOMY=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list1->GetItemText(0,2),
	m_list1->GetItemText(1,2),
	m_list1->GetItemText(2,2),
	m_list1->GetItemText(3,2),
	m_list1->GetItemText(4,2),
	m_list1->GetItemText(5,2),
	m_list1->GetItemText(6,2),
	m_list2.GetItemText(0,2),
	m_list2.GetItemText(1,2),
	ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 38",L"UPDATE 38F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	db.ExeSql(szBuffer);

	szBuffer.Format(
	L"UPDATE PLAN_INFO SET "
	L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list2.GetItemText(0,2),
	m_list2.GetItemText(1,2),
	m_list1->GetItemText(5,2),
	m_list1->GetItemText(6,2),
	ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}

bool CDlgCalcRes::e42(void)
{
	CString szBuffer;

	double KT	  =_wtof(m_list1->GetItemText(0,2)); 
	double S	  =_wtof(m_list1->GetItemText(1,2));
	double TAIR	  =_wtof(m_list1->GetItemText(2,2)); 
	double TMAT	  =_wtof(m_list1->GetItemText(3,2)); 
	double NTEP	  =_wtof(m_list1->GetItemText(4,2));
	double PRICE  =_wtof(m_list1->GetItemText(5,2)); 
	double COSTS  =_wtof(m_list1->GetItemText(6,2)); 
	double CURTIME=_wtof(m_list1->GetItemText(7,2)); 
	
	double RESULT=(0.0864/1000)*S*KT*abs(TAIR-TMAT)*NTEP; 
	double ECONOMY=RESULT*PRICE;

	szBuffer.Format(L"%.3f",RESULT);  m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.3f",ECONOMY); m_list2.SetItemText(1,2,szBuffer);



	return false;
}

bool CDlgCalcRes::e42Rec(void)
{
	CString szBuffer;
	CString ID_INFO;
	CRetQuery crq;

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%s AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%s AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
#endif
	}
	db.fQueryRead(szBuffer,crq);

	ID_INFO=crq.m_RetValues[0][0];
	
	szBuffer.Format(L"UPDATE 42 SET "
	L"KT=%s,S=%s,TAIR=%s,TMAT=%s,NTEP=%s,"
	L"PRICE=%s,COSTS=%s,CURTIME=%s,RESULT=%s,ECONOMY=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list1->GetItemText(0,2),
	m_list1->GetItemText(1,2),
	m_list1->GetItemText(2,2),
	m_list1->GetItemText(3,2),
	m_list1->GetItemText(4,2),
	m_list1->GetItemText(5,2),
	m_list1->GetItemText(6,2),
	m_list1->GetItemText(7,2),
	m_list2.GetItemText(0,2),
	m_list2.GetItemText(1,2),
	ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 42",L"UPDATE 42F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	db.ExeSql(szBuffer);

	szBuffer.Format(
	L"UPDATE PLAN_INFO SET "
	L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list2.GetItemText(0,2),
	m_list2.GetItemText(1,2),
	m_list1->GetItemText(6,2),
	m_list1->GetItemText(7,2),
	ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}

bool CDlgCalcRes::e48(void)
{
	CString szBuffer;

	double QFYEAR  =_wtof(m_list1->GetItemText(0,2)); 
	double TINT	   =_wtof(m_list1->GetItemText(1,2));  
	double THT     =_wtof(m_list1->GetItemText(2,2));  
	double TEXTS   =_wtof(m_list1->GetItemText(3,2));  
	//double QRN     =_wtof(m_list1->GetItemText(4,2)); 
	double KPD     =_wtof(m_list1->GetItemText(5,2));  
	double PRICE   =_wtof(m_list1->GetItemText(6,2));  
	double COSTS   =_wtof(m_list1->GetItemText(7,2));  
	double CURTIME =_wtof(m_list1->GetItemText(8,2)); 

	double RESULT=QFYEAR*(1-(TINT-THT)/(TINT-TEXTS))*0.1*0.24;
	double ECONOMY=RESULT*PRICE;
	
	szBuffer.Format(L"%.3f",RESULT);   m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.3f",ECONOMY);  m_list2.SetItemText(1,2,szBuffer);

	return false;
}

bool CDlgCalcRes::e48Rec(void)
{
	CString szBuffer;
	CString ID_INFO;
	CRetQuery crq;

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}

	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
#endif
	}
	//AfxMessageBox();

	db.fQueryRead(szBuffer,crq);

	ID_INFO=crq.m_RetValues[0][0];
	
	szBuffer.Format(L"UPDATE 48 SET "
	L"QFYEAR=%s,TINT=%s,THT=%s,TEXTS=%s,QRN=%s,"
	L"KPD=%s,PRICE=%s,COSTS=%s,CURTIME=%s,RESULT=%s,ECONOMY=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list1->GetItemText(0,2),
	m_list1->GetItemText(1,2),
	m_list1->GetItemText(2,2),
	m_list1->GetItemText(3,2),
	m_list1->GetItemText(4,2),
	m_list1->GetItemText(5,2),
	m_list1->GetItemText(6,2),
	m_list1->GetItemText(7,2),
	m_list1->GetItemText(8,2),
	m_list2.GetItemText(0,2),
	m_list2.GetItemText(1,2),
	ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 48",L"UPDATE 48F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	db.ExeSql(szBuffer);

	szBuffer.Format(
	L"UPDATE PLAN_INFO SET "
	L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list2.GetItemText(0,2),
	m_list2.GetItemText(1,2),
	m_list1->GetItemText(7,2),
	m_list1->GetItemText(8,2),
	ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}
	
	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}

bool CDlgCalcRes::e481(void)
{
	CString szBuffer;

	double QFYEAR  =_wtof(m_list1->GetItemText(0,2)); 
	double TINT	   =_wtof(m_list1->GetItemText(1,2));  
	double THT     =_wtof(m_list1->GetItemText(2,2));  
	double TEXTS   =_wtof(m_list1->GetItemText(3,2));  
	double QRN     =_wtof(m_list1->GetItemText(4,2)); 
	double KPD     =_wtof(m_list1->GetItemText(5,2));  
	double PRICE   =_wtof(m_list1->GetItemText(6,2));  
	double COSTS   =_wtof(m_list1->GetItemText(7,2));  
	double CURTIME =_wtof(m_list1->GetItemText(8,2)); 

	double RESULT=QFYEAR*(1-(TINT-THT)/(TINT-TEXTS))*0.1;
	double RESULT_ALT=RESULT*1000000/(QRN*KPD);
	double ECONOMY=RESULT_ALT*PRICE/1000;
	
	szBuffer.Format(L"%.3f",RESULT);     m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.3f",RESULT_ALT); m_list2.SetItemText(1,2,szBuffer);
	szBuffer.Format(L"%.3f",ECONOMY);	 m_list2.SetItemText(2,2,szBuffer);

	return false;
}

bool CDlgCalcRes::e481Rec(void)
{
	CString szBuffer;
	CString ID_INFO;
	CRetQuery crq;

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
#endif
	}
	//AfxMessageBox();

	db.fQueryRead(szBuffer,crq);

	ID_INFO=crq.m_RetValues[0][0];
	
	szBuffer.Format(L"UPDATE 481 SET "
	L"QFYEAR=%s,TINT=%s,THT=%s,TEXTS=%s,QRN=%s,"
	L"KPD=%s,PRICE=%s,COSTS=%s,CURTIME=%s,RESULT=%s,ECONOMY=%s, RESULT_ALT=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list1->GetItemText(0,2),
	m_list1->GetItemText(1,2),
	m_list1->GetItemText(2,2),
	m_list1->GetItemText(3,2),
	m_list1->GetItemText(4,2),
	m_list1->GetItemText(5,2),
	m_list1->GetItemText(6,2),
	m_list1->GetItemText(7,2),
	m_list1->GetItemText(8,2),
	m_list2.GetItemText(0,2),
	m_list2.GetItemText(2,2),
	m_list2.GetItemText(1,2),
	ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 481",L"UPDATE 481F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	db.ExeSql(szBuffer);

	szBuffer.Format(
	L"UPDATE PLAN_INFO SET "
	L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list2.GetItemText(0,2),
	m_list2.GetItemText(2,2),
	m_list1->GetItemText(7,2),
	m_list1->GetItemText(8,2),
	ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}
	
	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}


bool CDlgCalcRes::GetOldParamForTotalValues(void)
{
	switch (m_inumEvent)
	{
		case 1:
			
			break;
		case 7:
			
			break;
		case 8:
			
			break;
		case 12:
			
			break;
		case 15:
			
			break;
		case 25:
			
			break;
		case 27:
			
			break;
		case 38:
			
			break;
		case 42:
			
			break;
		case 48:
			
			break;
		default:
			break;
	}
	return true;
}


bool CDlgCalcRes::e2(void)
{
	CString szBuffer;

	double P0		=_wtof(m_list1->GetItemText(0,2));
	double P1		=_wtof(m_list1->GetItemText(1,2));
	double T0		=_wtof(m_list1->GetItemText(2,2));
	double T1		=_wtof(m_list1->GetItemText(3,2));
	double q		=_wtof(m_list1->GetItemText(4,2));
	double Din		=_wtof(m_list1->GetItemText(5,2));
	double L		=_wtof(m_list1->GetItemText(6,2));
	double PRICE	=_wtof(m_list1->GetItemText(7,2));
	double COSTS	=_wtof(m_list1->GetItemText(8,2));
	double TIME		=_wtof(m_list1->GetItemText(9,2));

	double dq=q/1.2044;
	double V=3.14*Din*Din*L/4;
	double Z0=1-((10.2*P0-6)*(0.345*dq/100-0.446/1000)+0.015)*(1.3-0.0144*(T0-283.2));
	double Z1=1-((10.2*P1-6)*(0.345*dq/100-0.446/1000)+0.015)*(1.3-0.0144*(T1-283.2));;
	double RESULT=0.001*2893*V*(P0/(Z0*T0)-P1/(Z1*T1));
	double ECONOMY=RESULT*PRICE;

	szBuffer.Format(L"%.3f", dq		); m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.3f", V		); m_list2.SetItemText(1,2,szBuffer);
	szBuffer.Format(L"%.3f", Z0		); m_list2.SetItemText(2,2,szBuffer);
	szBuffer.Format(L"%.3f", Z1		); m_list2.SetItemText(3,2,szBuffer);
	szBuffer.Format(L"%.3f", RESULT	); m_list2.SetItemText(4,2,szBuffer);
	szBuffer.Format(L"%.3f", ECONOMY); m_list2.SetItemText(5,2,szBuffer);

	return false;
}


bool CDlgCalcRes::e2Rec(void)
{
	CString szBuffer;
	CString ID_INFO;
	CRetQuery crq;

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%s AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%s AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
#endif
	}
	db.fQueryRead(szBuffer,crq);

	ID_INFO=crq.m_RetValues[0][0];
	
	szBuffer.Format(L"UPDATE 2 SET "
	L"PINIT=%s, PLIM=%s, TINIT=%s, TLIM=%s, P0=%s, "
	L"DIN=%s, L=%s, PRICE=%s, COSTS=%s, CURTIME=%s,"
	L"RESULT=%s, ECONOMY=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list1->GetItemText(0,2),
	m_list1->GetItemText(1,2),
	m_list1->GetItemText(2,2),
	m_list1->GetItemText(3,2),
	m_list1->GetItemText(4,2),
	m_list1->GetItemText(5,2),
	m_list1->GetItemText(6,2),
	m_list1->GetItemText(7,2),
	m_list1->GetItemText(8,2),
	m_list1->GetItemText(9,2),
	m_list2.GetItemText(4,2),
	m_list2.GetItemText(5,2),
	ID_INFO);

	//AfxMessageBox(szBuffer);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 2",L"UPDATE 2F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	db.ExeSql(szBuffer);

	szBuffer.Format(
	L"UPDATE PLAN_INFO SET "
	L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list2.GetItemText(4,2),
	m_list2.GetItemText(5,2),
	m_list1->GetItemText(8,2),
	m_list1->GetItemText(9,2),
	ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}
	
	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}


bool CDlgCalcRes::e4(void)
{
	CString szBuffer;

	double P0		=_wtof(m_list1->GetItemText(0,2));
	double P1		=_wtof(m_list1->GetItemText(1,2));
	double T0		=_wtof(m_list1->GetItemText(2,2));
	double T1		=_wtof(m_list1->GetItemText(3,2));
	double q0		=_wtof(m_list1->GetItemText(4,2));
	double Din		=_wtof(m_list1->GetItemText(5,2));
	double L		=_wtof(m_list1->GetItemText(6,2));
	double PRICE	=_wtof(m_list1->GetItemText(7,2));
	double COSTS	=_wtof(m_list1->GetItemText(8,2));
	
	double DQ=q0/1.2044;
	double V=3.14*Din*Din*L/4;
	double Z0=1-((10.2*P0-6)*(0.345*0.01*DQ-0.446*0.001)+0.015)*(1.3-0.0144*(T0-283.2));
	double Z1=1-((10.2*P1-6)*(0.345*0.01*DQ-0.446*0.001)+0.015)*(1.3-0.0144*(T1-283.2));
	double RESULT=0.001*2893*V*(P0/(Z0*T0)-P1/(Z1*T1));
	double ECONOMY=RESULT*PRICE;

	szBuffer.Format(L"%.3f",DQ);	  m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.3f",V);		  m_list2.SetItemText(1,2,szBuffer);
	szBuffer.Format(L"%.3f",Z0);	  m_list2.SetItemText(2,2,szBuffer);
	szBuffer.Format(L"%.3f",Z1);	  m_list2.SetItemText(3,2,szBuffer);
	szBuffer.Format(L"%.3f",RESULT);  m_list2.SetItemText(4,2,szBuffer);
	szBuffer.Format(L"%.3f",ECONOMY); m_list2.SetItemText(5,2,szBuffer);
	
	return false;
}


bool CDlgCalcRes::e4Rec(void)
{
	CString szBuffer;
	CString ID_INFO;
	CRetQuery crq;

	//AfxMessageBox(L"if(db.m_iStatus==PLAN)");

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
#endif
	}
	//AfxMessageBox(szBuffer);

	db.fQueryRead(szBuffer,crq);

	ID_INFO=crq.m_RetValues[0][0];
	
	szBuffer.Format(L"UPDATE 4 SET "
		L"P0=%s, P1=%s, T0=%s, T1=%s, Q0=%s,"
		L"DIN=%s, L=%s, PRICE=%s, COSTS=%s, CURTIME=%s,"
		L"RESULT=%s, ECONOMY=%s "
		L"WHERE ID_PLAN_INFO=%s",
		m_list1->GetItemText(0,2),
		m_list1->GetItemText(1,2),
		m_list1->GetItemText(2,2),
		m_list1->GetItemText(3,2),
		m_list1->GetItemText(4,2),
		m_list1->GetItemText(5,2),
		m_list1->GetItemText(6,2),
		m_list1->GetItemText(7,2),
		m_list1->GetItemText(8,2),
		m_list1->GetItemText(9,2),
		m_list2.GetItemText(4,2),
		m_list2.GetItemText(5,2),
		ID_INFO);

	
	//AfxMessageBox(szBuffer);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 4",L"UPDATE 4F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	//AfxMessageBox(szBuffer);

	db.ExeSql(szBuffer);
	//AfxMessageBox(szBuffer);
	szBuffer.Format(
	L"UPDATE PLAN_INFO SET "
	L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list2.GetItemText(4,2),
	m_list2.GetItemText(5,2),
	m_list1->GetItemText(8,2),
	m_list1->GetItemText(9,2),
	ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}
	
	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}


bool CDlgCalcRes::e17(void)
{
	CString szBuffer;

	double BTMG=	_wtof(m_list1->GetItemText(0,2));
	double QNMG=	_wtof(m_list1->GetItemText(1,2));
	double NFMG=	_wtof(m_list1->GetItemText(2,2));
	double NMG=		_wtof(m_list1->GetItemText(3,2));
	double TMG=		_wtof(m_list1->GetItemText(4,2));
	
	double BTBG=	_wtof(m_list1->GetItemText(5,2));
	double QNBG=	_wtof(m_list1->GetItemText(6,2));
	double NFBG=	_wtof(m_list1->GetItemText(7,2));
	double NBG=		_wtof(m_list1->GetItemText(8,2));
	double TBG=		_wtof(m_list1->GetItemText(9,2));
	
	double PRICE=	_wtof(m_list1->GetItemText(10,2));
	double COSTS=	_wtof(m_list1->GetItemText(11,2));

	double VN0=(BTMG*QNMG/7000*(1-NFMG/NMG)*TMG)*0.001;
	double VN1=(BTBG*QNBG/7000*(1-NFBG/NBG)*TBG)*0.001;

	//szBuffer.Format(L"%.3f",VN0); AfxMessageBox(szBuffer);

	double RESULT=VN0+VN1;
	double ECONOMY=RESULT*PRICE;

	szBuffer.Format(L"%.3f",VN0);		  m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.3f",VN1);		  m_list2.SetItemText(1,2,szBuffer);
	szBuffer.Format(L"%.3f",RESULT);	  m_list2.SetItemText(2,2,szBuffer);
	szBuffer.Format(L"%.3f",ECONOMY);	  m_list2.SetItemText(3,2,szBuffer);

	return false;
}


bool CDlgCalcRes::e17Rec(void)
{
	CString szBuffer;
	CString ID_INFO;
	CRetQuery crq;

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
#endif
	}
	db.fQueryRead(szBuffer,crq);

	//AfxMessageBox(szBuffer);

	ID_INFO=crq.m_RetValues[0][0];
	
	szBuffer.Format(L"UPDATE 17 SET "
		L"BTMG=%s,"   //0
		L"QNMG=%s,"   //1
		L"NFMG=%s,"   //2
		L"NMG=%s,"    //3
		L"TMG=%s,"    //4
		L"BTBG=%s,"   //5
		L"QNBG=%s,"   //6
		L"NFBG=%s,"   //7
		L"NBG=%s,"	  //8
		L"TBG=%s,"    //9
		L"PRICE=%s,"  //10
		L"COSTS=%s," //11 
		L"CURTIME=%s,"//12 
		L"VN0=%s,"	  //0
		L"VN1=%s,"	  //1
		L"VE=%s,"	  //2
		L"ECONOMY=%s "//3
		L"WHERE ID_PLAN_INFO=%s",
		m_list1->GetItemText(0,2),
		m_list1->GetItemText(1,2),
		m_list1->GetItemText(2,2),
		m_list1->GetItemText(3,2),
		m_list1->GetItemText(4,2),
		m_list1->GetItemText(5,2),
		m_list1->GetItemText(6,2),
		m_list1->GetItemText(7,2),
		m_list1->GetItemText(8,2),
		m_list1->GetItemText(9,2),
		m_list1->GetItemText(10,2),
		m_list1->GetItemText(11,2),
		m_list1->GetItemText(12,2),
		m_list2.GetItemText(0,2),
		m_list2.GetItemText(1,2),
		m_list2.GetItemText(2,2),
		m_list2.GetItemText(3,2),
		ID_INFO);

	
	//AfxMessageBox(szBuffer);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 17",L"UPDATE 17F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	//AfxMessageBox(szBuffer);
	//AfxMessageBox(szBuffer);

	db.ExeSql(szBuffer);
	//AfxMessageBox(szBuffer);
	szBuffer.Format(
	L"UPDATE PLAN_INFO SET "
	L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list2.GetItemText(2,2),
	m_list2.GetItemText(3,2),
	m_list1->GetItemText(11,2),
	m_list1->GetItemText(12,2),
	ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}
	
	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}


bool CDlgCalcRes::e26(void)
{
	CString szBuffer;

	double N0=	   _wtof(m_list1->GetItemText(0,2));
	double T=	   _wtof(m_list1->GetItemText(1,2));
	double KPD=    _wtof(m_list1->GetItemText(2,2));
	double K=	   _wtof(m_list1->GetItemText(3,2));
	double PRICE=  _wtof(m_list1->GetItemText(4,2));
	double COSTS=  _wtof(m_list1->GetItemText(5,2));
	double CURTIME=_wtof(m_list1->GetItemText(6,2));

	double RESULT=0.001*K*(N0/KPD)*T;
	double ECONOMY=RESULT*PRICE;

	szBuffer.Format(L"%.3f",RESULT);  m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.3f",ECONOMY); m_list2.SetItemText(1,2,szBuffer);

	return false;
}


bool CDlgCalcRes::e26Rec(void)
{
	CString szBuffer;
	CString ID_INFO;
	CRetQuery crq;

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
#endif
	}
	db.fQueryRead(szBuffer,crq);

	//AfxMessageBox(szBuffer);

	ID_INFO=crq.m_RetValues[0][0];
	
	szBuffer.Format(L"UPDATE 26 SET "
		L"N0=%s,"   //0
		L"T=%s,"   //1
		L"KPD=%s,"   //2
		L"K=%s,"    //3
		L"PRICE=%s,"    //4
		L"COSTS=%s,"   //5
		L"CURTIME=%s,"   //6
		L"RESULT=%s,"   //7
		L"ECONOMY=%s "	  //8
		L"WHERE ID_PLAN_INFO=%s",
		m_list1->GetItemText(0,2),
		m_list1->GetItemText(1,2),
		m_list1->GetItemText(2,2),
		m_list1->GetItemText(3,2),
		m_list1->GetItemText(4,2),
		m_list1->GetItemText(5,2),
		m_list1->GetItemText(6,2),
		m_list2.GetItemText(0,2),
		m_list2.GetItemText(1,2),
		ID_INFO);

	
	//AfxMessageBox(szBuffer);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 26",L"UPDATE 26F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	//AfxMessageBox(szBuffer);
	//AfxMessageBox(szBuffer);

	db.ExeSql(szBuffer);
	//AfxMessageBox(szBuffer);
	szBuffer.Format(
	L"UPDATE PLAN_INFO SET "
	L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list2.GetItemText(0,2),
	m_list2.GetItemText(1,2),
	m_list1->GetItemText(5,2),
	m_list1->GetItemText(6,2),
	ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}
	
	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}


bool CDlgCalcRes::e20(void)
{
	CString szBuffer;

	double NZAM =	 _wtof(m_list1->GetItemText(0,2));
	double NUST =	 _wtof(m_list1->GetItemText(1,2));
	double T =		 _wtof(m_list1->GetItemText(2,2));
	double N =		 _wtof(m_list1->GetItemText(3,2));
	double PRICE =	 _wtof(m_list1->GetItemText(4,2));
	double COSTS =	 _wtof(m_list1->GetItemText(5,2));
	double CURTIME = _wtof(m_list1->GetItemText(6,2));

	double DW0 = 0.001*(NZAM-NUST)*T;
	double RESULT=DW0*N;
	double ECONOMY=RESULT*PRICE;

	szBuffer.Format(L"%.3f",DW0);	  m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.3f",RESULT);  m_list2.SetItemText(1,2,szBuffer);
	szBuffer.Format(L"%.3f",ECONOMY); m_list2.SetItemText(2,2,szBuffer);

	return false;
}


bool CDlgCalcRes::e20Rec(void)
{
	CString szBuffer;
	CString ID_INFO;
	CRetQuery crq;

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
#endif
	}
	db.fQueryRead(szBuffer,crq);

	//AfxMessageBox(szBuffer);

	ID_INFO=crq.m_RetValues[0][0];
	
	szBuffer.Format(L"UPDATE 20 SET "
		L"NZAM=%s,"    //0
		L"NUST=%s,"    //1
		L"T=%s,"	   //2
		L"N=%s,"       //3
		L"PRICE=%s,"   //4
		L"COSTS=%s,"   //5
		L"CURTIME=%s," //6
		L"DW0=%s,"     //7
		L"RESULT=%s,"  //8
		L"ECONOMY=%s " //9
		L"WHERE ID_PLAN_INFO=%s",
		m_list1->GetItemText(0,2),
		m_list1->GetItemText(1,2),
		m_list1->GetItemText(2,2),
		m_list1->GetItemText(3,2),
		m_list1->GetItemText(4,2),
		m_list1->GetItemText(5,2),
		m_list1->GetItemText(6,2),
		m_list2.GetItemText(0,2),
		m_list2.GetItemText(1,2),
		m_list2.GetItemText(2,2),
		ID_INFO);

	
	

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 20",L"UPDATE 20F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	//AfxMessageBox(szBuffer);
	//AfxMessageBox(szBuffer);

	db.ExeSql(szBuffer);

	
	//AfxMessageBox(szBuffer);
	szBuffer.Format(
	L"UPDATE PLAN_INFO SET "
	L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list2.GetItemText(1,2),
	m_list2.GetItemText(2,2),
	m_list1->GetItemText(5,2),
	m_list1->GetItemText(6,2),
	ID_INFO);

	//AfxMessageBox(szBuffer);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}
	
	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}


bool CDlgCalcRes::e23(void)
{
	CString szBuffer;

	double DPIDLE=_wtof(m_list1->GetItemText(0,2));
	double T=	  _wtof(m_list1->GetItemText(1,2));
	double U=	  _wtof(m_list1->GetItemText(2,2));
	double UNOM=  _wtof(m_list1->GetItemText(3,2));
	double N=	  _wtof(m_list1->GetItemText(4,2));
	double PRICE= _wtof(m_list1->GetItemText(5,2));
	
	double RESULT=0.001*(DPIDLE*T*(U/UNOM)*(U/UNOM))*N;
	double ECONOMY=RESULT*PRICE;

	szBuffer.Format(L"%.3f",RESULT); m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.3f",ECONOMY); m_list2.SetItemText(1,2,szBuffer);

	return false;
}


bool CDlgCalcRes::e23Rec(void)
{
	CString szBuffer;
	CString ID_INFO;
	CRetQuery crq;

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
#endif
	}
	db.fQueryRead(szBuffer,crq);

	//AfxMessageBox(szBuffer);

	ID_INFO=crq.m_RetValues[0][0];
	
	szBuffer.Format(L"UPDATE 23 SET "
		L"DPIDLE=%s,"    //0
		L"T=%s,"		 //1
		L"U=%s,"	     //2
		L"UNOM=%s,"      //3
		L"N=%s,"		 //4
		L"PRICE=%s,"	 //5
		L"CURCOSTS=%s,"  //6
		L"CURTIME=%s,"   //7
		L"RESULT=%s,"	 //8
		L"ECONOMY=%s "   //9
		L"WHERE ID_PLAN_INFO=%s",
		m_list1->GetItemText(0,2),
		m_list1->GetItemText(1,2),
		m_list1->GetItemText(2,2),
		m_list1->GetItemText(3,2),
		m_list1->GetItemText(4,2),
		m_list1->GetItemText(5,2),
		m_list1->GetItemText(6,2),
		m_list1->GetItemText(7,2),
		m_list2.GetItemText(0,2),
		m_list2.GetItemText(1,2),
		ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 23",L"UPDATE 23F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	//AfxMessageBox(szBuffer);
	//AfxMessageBox(szBuffer);

	db.ExeSql(szBuffer);

	
	//AfxMessageBox(szBuffer);
	szBuffer.Format(
	L"UPDATE PLAN_INFO SET "
	L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list2.GetItemText(0,2),
	m_list2.GetItemText(1,2),
	m_list1->GetItemText(6,2),
	m_list1->GetItemText(7,2),
	ID_INFO);

	//AfxMessageBox(szBuffer);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}
	
	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}


bool CDlgCalcRes::e43(void)
{
	CString szBuffer;

	double QUT=		_wtof(m_list1->GetItemText(0,2));
	double QRN=		_wtof(m_list1->GetItemText(1,2));
	double KPD=		_wtof(m_list1->GetItemText(2,2));
	double NADD=	_wtof(m_list1->GetItemText(3,2));
	double KPDGTA=	_wtof(m_list1->GetItemText(4,2));
	double T=		_wtof(m_list1->GetItemText(5,2));
	double KLOAD=	_wtof(m_list1->GetItemText(6,2));
	double DPUT=	_wtof(m_list1->GetItemText(7,2));
	double G=		_wtof(m_list1->GetItemText(8,2));
	double TEMP=	_wtof(m_list1->GetItemText(9,2));
	double PRICE=	_wtof(m_list1->GetItemText(10,2));
	double CURCOSTS=_wtof(m_list1->GetItemText(11,2));
	double CURTIME=	_wtof(m_list1->GetItemText(12,2));
	
	double Q=1.293*273.15/(273.15+TEMP);
	double RASH=G/Q;
	double NADD_R=RASH*DPUT;
	double RASHADDI=(0.001*860/QRN)*(NADD*T*KLOAD/KPDGTA);

	double RASHKOT=1000*QUT/(QRN*KPD);
	//03032018 Добавлено: преобразование из тыс.м3 в Гкал
	//double RESULT=RASHKOT-RASHADDI;
	double RESULT=((RASHKOT-RASHADDI)*1.14)/154.8*1000;
	double ECONOMY=RESULT*PRICE;
	
	szBuffer.Format(L"%.3f",Q);		   m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.3f",RASH);     m_list2.SetItemText(1,2,szBuffer);
	szBuffer.Format(L"%.3f",NADD_R);   m_list2.SetItemText(2,2,szBuffer);
	szBuffer.Format(L"%.3f",RASHADDI); m_list2.SetItemText(3,2,szBuffer);
	szBuffer.Format(L"%.3f",RASHADDI); m_list2.SetItemText(4,2,szBuffer);
	szBuffer.Format(L"%.3f",RASHKOT);  m_list2.SetItemText(5,2,szBuffer);
	szBuffer.Format(L"%.3f",RESULT);   m_list2.SetItemText(6,2,szBuffer);
	szBuffer.Format(L"%.3f",ECONOMY);  m_list2.SetItemText(7,2,szBuffer);

	return false;
}


bool CDlgCalcRes::e43Rec(void)
{
	CString szBuffer;
	CString ID_INFO;
	CRetQuery crq;

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
#endif
	}
	db.fQueryRead(szBuffer,crq);

	//AfxMessageBox(szBuffer);

	ID_INFO=crq.m_RetValues[0][0];
	
	szBuffer.Format(L"UPDATE 43 SET "
		L"QUT=%s,"     //0
		L"QRN=%s,"		//1
		L"KPDK=%s,"    //2
		L"NADD=%s,"    //3
		L"KPDGTA=%s,"  //4
		L"TGTA=%s,"    //5
		L"KLOAD=%s,"   //6
		L"DPUT=%s,"    //7
		L"G=%s,"       //8
		L"TEMP=%s,"    //9
		L"PRICE=%s,"   //10
		L"CURCOSTS=%s,"//11
		L"CURTIME=%s," //12
		L"Q=%s,"       //13
		L"RASH=%s,"    //14
		L"NADD2=%s,"   //15
		L"QGTAI=%s,"   //16
		L"QGTA=%s,"    //17
		L"QK=%s,"      //18
		L"RESULT=%s,"  //19
		L"ECONOMY=%s " //20
		L"WHERE ID_PLAN_INFO=%s",
		m_list1->GetItemText(0,2),
		m_list1->GetItemText(1,2),
		m_list1->GetItemText(2,2),
		m_list1->GetItemText(3,2),
		m_list1->GetItemText(4,2),
		m_list1->GetItemText(5,2),
		m_list1->GetItemText(6,2),
		m_list1->GetItemText(7,2),
		m_list1->GetItemText(8,2),
		m_list1->GetItemText(9,2),
		m_list1->GetItemText(10,2),
		m_list1->GetItemText(11,2),
		m_list1->GetItemText(12,2),
		m_list2.GetItemText(0,2),
		m_list2.GetItemText(1,2),
		m_list2.GetItemText(2,2),
		m_list2.GetItemText(3,2),
		m_list2.GetItemText(4,2),
		m_list2.GetItemText(5,2),
		m_list2.GetItemText(6,2),
		m_list2.GetItemText(7,2),
		ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 43",L"UPDATE 43F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	//AfxMessageBox(szBuffer);
	//AfxMessageBox(szBuffer);

	db.ExeSql(szBuffer);

	
	//AfxMessageBox(szBuffer);
	szBuffer.Format(
	L"UPDATE PLAN_INFO SET "
	L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list2.GetItemText(6,2),
	m_list2.GetItemText(7,2),
	m_list1->GetItemText(11,2),
	m_list1->GetItemText(12,2),
	ID_INFO);

	//AfxMessageBox(szBuffer);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}
	
	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}

bool CDlgCalcRes::e431(void)
{
	CString szBuffer;

	double QUT=		_wtof(m_list1->GetItemText(0,2));
	double QRN=		_wtof(m_list1->GetItemText(1,2));
	double KPD=		_wtof(m_list1->GetItemText(2,2));
	double NADD=	_wtof(m_list1->GetItemText(3,2));
	double KPDGTA=	_wtof(m_list1->GetItemText(4,2));
	double T=		_wtof(m_list1->GetItemText(5,2));
	double KLOAD=	_wtof(m_list1->GetItemText(6,2));
	double DPUT=	_wtof(m_list1->GetItemText(7,2));
	double G=		_wtof(m_list1->GetItemText(8,2));
	double TEMP=	_wtof(m_list1->GetItemText(9,2));
	double PRICE=	_wtof(m_list1->GetItemText(10,2));
	double CURCOSTS=_wtof(m_list1->GetItemText(11,2));
	double CURTIME=	_wtof(m_list1->GetItemText(12,2));
	
	double Q=1.293*273.15/(273.15+TEMP);
	double RASH=G/Q;
	double NADD_R=RASH*DPUT;
	double RASHADDI=(0.001*860/QRN)*(NADD*T*KLOAD/KPDGTA);

	double RASHKOT=1000*QUT/(QRN*KPD);
	//03032018 Добавлено: преобразование из тыс.м3 в Гкал
	//double RESULT=RASHKOT-RASHADDI;
	double RESULT=(sqrt((RASHKOT-RASHADDI)*(RASHKOT-RASHADDI))*1.15)/160*1000;
	double ECONOMY=RESULT*PRICE;
	double RESULT_ALT=sqrt((RASHKOT-RASHADDI)*(RASHKOT-RASHADDI));

	szBuffer.Format(L"%.3f",Q);		     m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.3f",RASH);       m_list2.SetItemText(1,2,szBuffer);
	szBuffer.Format(L"%.3f",NADD_R);     m_list2.SetItemText(2,2,szBuffer);
	szBuffer.Format(L"%.3f",RASHADDI);   m_list2.SetItemText(3,2,szBuffer);
	szBuffer.Format(L"%.3f",RASHADDI);   m_list2.SetItemText(4,2,szBuffer);
	szBuffer.Format(L"%.3f",RASHKOT);    m_list2.SetItemText(5,2,szBuffer);
	szBuffer.Format(L"%.3f",RESULT_ALT); m_list2.SetItemText(6,2,szBuffer);
	szBuffer.Format(L"%.3f",RESULT);     m_list2.SetItemText(7,2,szBuffer);
	szBuffer.Format(L"%.3f",ECONOMY);    m_list2.SetItemText(8,2,szBuffer);

	return false;
}


bool CDlgCalcRes::e431Rec(void)
{
	CString szBuffer;
	CString ID_INFO;
	CRetQuery crq;

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
#endif
	}
	db.fQueryRead(szBuffer,crq);

	//AfxMessageBox(szBuffer);

	ID_INFO=crq.m_RetValues[0][0];
	
	szBuffer.Format(L"UPDATE 431 SET "
		L"QUT=%s,"     //0
		L"QRN=%s,"		//1
		L"KPDK=%s,"    //2
		L"NADD=%s,"    //3
		L"KPDGTA=%s,"  //4
		L"TGTA=%s,"    //5
		L"KLOAD=%s,"   //6
		L"DPUT=%s,"    //7
		L"G=%s,"       //8
		L"TEMP=%s,"    //9
		L"PRICE=%s,"   //10
		L"CURCOSTS=%s,"//11
		L"CURTIME=%s," //12
		L"Q=%s,"       //13
		L"RASH=%s,"    //14
		L"NADD2=%s,"   //15
		L"QGTAI=%s,"   //16
		L"QGTA=%s,"    //17
		L"QK=%s,"      //18
		L"RESULT=%s,"  //19
		L"ECONOMY=%s, " //20
		L"RESULT_ALT=%s "//21
		L"WHERE ID_PLAN_INFO=%s",
		m_list1->GetItemText(0,2),
		m_list1->GetItemText(1,2),
		m_list1->GetItemText(2,2),
		m_list1->GetItemText(3,2),
		m_list1->GetItemText(4,2),
		m_list1->GetItemText(5,2),
		m_list1->GetItemText(6,2),
		m_list1->GetItemText(7,2),
		m_list1->GetItemText(8,2),
		m_list1->GetItemText(9,2),
		m_list1->GetItemText(10,2),
		m_list1->GetItemText(11,2),
		m_list1->GetItemText(12,2),
		m_list2.GetItemText(0,2),
		m_list2.GetItemText(1,2),
		m_list2.GetItemText(2,2),
		m_list2.GetItemText(3,2),
		m_list2.GetItemText(4,2),
		m_list2.GetItemText(5,2),
		m_list2.GetItemText(6,2),
		m_list2.GetItemText(8,2),
		m_list2.GetItemText(7,2),
		ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 431",L"UPDATE 431F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	//AfxMessageBox(szBuffer);
	//AfxMessageBox(szBuffer);

	db.ExeSql(szBuffer);

	
	//AfxMessageBox(szBuffer);
	szBuffer.Format(
	L"UPDATE PLAN_INFO SET "
	L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list2.GetItemText(7,2),
	m_list2.GetItemText(8,2),
	m_list1->GetItemText(11,2),
	m_list1->GetItemText(12,2),
	ID_INFO);

	//AfxMessageBox(szBuffer);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}
	
	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}


bool CDlgCalcRes::e100(void)
{
	CString szBuffer;
	double KUTE = _wtof(m_list1->GetItemText(0,2));
	double PUT = _wtof(m_list1->GetItemText(1,2));
	double N = _wtof(m_list1->GetItemText(2,2));
	double PRICE = _wtof(m_list1->GetItemText(3,2));
	double COSTS = _wtof(m_list1->GetItemText(4,2));

	double RESULT = KUTE*PUT*N;
	double ECONOMY = RESULT*PRICE;

	szBuffer.Format(L"%.3f",RESULT); m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.3f",ECONOMY); m_list2.SetItemText(1,2,szBuffer);

	return false;
}


bool CDlgCalcRes::e100Rec(void)
{
	CString szBuffer;
	CString ID_INFO;
	CRetQuery crq;

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
#endif
	}
	db.fQueryRead(szBuffer,crq);

	//AfxMessageBox(szBuffer);

	ID_INFO=crq.m_RetValues[0][0];
	
	szBuffer.Format(L"UPDATE 100 SET "
		L"KUTE=%s,"		 //0
		L"PUT=%s,"		 //1
		L"N=%s,"	     //2
		L"PRICE=%s,"     //3
		L"COSTS=%s,"	 //4
		L"CURTIME=%s,"	 //5
		L"RESULT=%s,"	 //1
		L"ECONOMY=%s "   //2
		L"WHERE ID_PLAN_INFO=%s",
		m_list1->GetItemText(0,2),
		m_list1->GetItemText(1,2),
		m_list1->GetItemText(2,2),
		m_list1->GetItemText(3,2),
		m_list1->GetItemText(4,2),
		m_list1->GetItemText(5,2),
		m_list2.GetItemText(0,2),
		m_list2.GetItemText(1,2),
		ID_INFO);
	//AfxMessageBox(szBuffer);
	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 100",L"UPDATE 100F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	//AfxMessageBox(szBuffer);
	//AfxMessageBox(szBuffer);

	db.ExeSql(szBuffer);

	
	//AfxMessageBox(szBuffer);
	szBuffer.Format(
	L"UPDATE PLAN_INFO SET "
	L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list2.GetItemText(0,2),
	m_list2.GetItemText(1,2),
	m_list1->GetItemText(4,2),
	m_list1->GetItemText(5,2),
	ID_INFO);

	//AfxMessageBox(szBuffer);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}
	
	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}

bool CDlgCalcRes::e101(void)
{
	CString szBuffer;

	double P0		=_wtof(m_list1->GetItemText(0,2));
	double P1		=_wtof(m_list1->GetItemText(1,2));
	double T0		=_wtof(m_list1->GetItemText(2,2));
	double T1		=_wtof(m_list1->GetItemText(3,2));
	double q0		=_wtof(m_list1->GetItemText(4,2));
	double Din		=_wtof(m_list1->GetItemText(5,2));
	double L		=_wtof(m_list1->GetItemText(6,2));
	double PRICE	=_wtof(m_list1->GetItemText(7,2));
	double COSTS	=_wtof(m_list1->GetItemText(8,2));
	
	double DQ=q0/1.2044;
	double V=3.14*Din*Din*L/4;
	double Z0=1-((10.2*P0-6)*(0.345*0.01*DQ-0.446*0.001)+0.015)*(1.3-0.0144*(T0-283.2));
	double Z1=1-((10.2*P1-6)*(0.345*0.01*DQ-0.446*0.001)+0.015)*(1.3-0.0144*(T1-283.2));
	double RESULT=0.001*2893*V*(P0/(Z0*T0)-P1/(Z1*T1));
	double ECONOMY=RESULT*PRICE;

	szBuffer.Format(L"%.3f",DQ);	  m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.3f",V);		  m_list2.SetItemText(1,2,szBuffer);
	szBuffer.Format(L"%.3f",Z0);	  m_list2.SetItemText(2,2,szBuffer);
	szBuffer.Format(L"%.3f",Z1);	  m_list2.SetItemText(3,2,szBuffer);
	szBuffer.Format(L"%.3f",RESULT);  m_list2.SetItemText(4,2,szBuffer);
	szBuffer.Format(L"%.3f",ECONOMY); m_list2.SetItemText(5,2,szBuffer);
	
	return false;
}


bool CDlgCalcRes::e101Rec(void)
{
	CString szBuffer;
	CString ID_INFO;
	CRetQuery crq;

	//AfxMessageBox(L"if(db.m_iStatus==PLAN)");

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}
	//AfxMessageBox(szBuffer);
	if(db.m_iStatus==FACT)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);
#endif

	}
	//AfxMessageBox(szBuffer);

	db.fQueryRead(szBuffer,crq);

	ID_INFO=crq.m_RetValues[0][0];
	
	szBuffer.Format(L"UPDATE 101 SET "
		L"P0=%s, P1=%s, T0=%s, T1=%s, Q0=%s,"
		L"DIN=%s, L=%s, PRICE=%s, COSTS=%s, CURTIME=%s,"
		L"RESULT=%s, ECONOMY=%s "
		L"WHERE ID_PLAN_INFO=%s",
		m_list1->GetItemText(0,2),
		m_list1->GetItemText(1,2),
		m_list1->GetItemText(2,2),
		m_list1->GetItemText(3,2),
		m_list1->GetItemText(4,2),
		m_list1->GetItemText(5,2),
		m_list1->GetItemText(6,2),
		m_list1->GetItemText(7,2),
		m_list1->GetItemText(8,2),
		m_list1->GetItemText(9,2),
		m_list2.GetItemText(4,2),
		m_list2.GetItemText(5,2),
		ID_INFO);

	
	//AfxMessageBox(szBuffer);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 101",L"UPDATE 101F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}

	//AfxMessageBox(szBuffer);

	db.ExeSql(szBuffer);
	//AfxMessageBox(szBuffer);
	szBuffer.Format(
	L"UPDATE PLAN_INFO SET "
	L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list2.GetItemText(4,2),
	m_list2.GetItemText(5,2),
	m_list1->GetItemText(8,2),
	m_list1->GetItemText(9,2),
	ID_INFO);

	if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}
	
	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}

bool CDlgCalcRes::e81(void)
{
	CString szBuffer;

	double T=	   _wtof(m_list1->GetItemText(0,2));
	double NNOM=   _wtof(m_list1->GetItemText(1,2));
	double NNOM0=  _wtof(m_list1->GetItemText(2,2));
	double NNOM1=  _wtof(m_list1->GetItemText(3,2));
	double QRN=	   _wtof(m_list1->GetItemText(4,2));
	double KPD=	   _wtof(m_list1->GetItemText(5,2));
	double KEF=	   _wtof(m_list1->GetItemText(6,2));
	double PRICE=  _wtof(m_list1->GetItemText(7,2));

	double KNE0=NNOM0/NNOM;
	double KNE1=NNOM1/NNOM;
	double KKR=0.5*(1-KNE0/KNE1);
	double RESULT=0.001*(860/QRN)*(NNOM/KPD)*KEF*KKR*T;
	double ECONOMY=RESULT*PRICE;

	szBuffer.Format(L"%.3f",KNE0);		m_list2.SetItemText(0,2,szBuffer);
	szBuffer.Format(L"%.3f",KNE1);		m_list2.SetItemText(1,2,szBuffer);
	szBuffer.Format(L"%.3f",KKR);		m_list2.SetItemText(2,2,szBuffer);
	szBuffer.Format(L"%.3f",RESULT);	m_list2.SetItemText(3,2,szBuffer);
	szBuffer.Format(L"%.3f",ECONOMY);	m_list2.SetItemText(4,2,szBuffer);
	
	return false;
}


bool CDlgCalcRes::e81Rec(void)
{
	CString szBuffer;
	CString ID_INFO;
	CRetQuery crq;

	//AfxMessageBox(L"if(db.m_iStatus==PLAN)");

	if(db.m_iStatus==PLAN)
	{
#ifdef CONVERTING_ALIAS
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
		szBuffer.Format(
		L"SELECT PI.ID_PLAN_INFO FROM PLAN_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.m_szEventName);
#endif
	}
	//AfxMessageBox(szBuffer);
	/*if(db.m_iStatus==FACT)
		szBuffer.Format(
		L"SELECT PI.ID_FACT_INFO FROM FACT_INFO AS PI INNER JOIN "
		L"(SELECT ID_OBJECT FROM OBJECTS_PLANS AS T1 INNER JOIN PLACEOFUSEPLAN AS T2 ON T1.ID_PLACE=T2.ID_PLACE WHERE T2.NAME_PLACE='%s' AND T1.OBJECT_NAME='%s') T3 "
		L"ON PI.ID_OBJECT=T3.ID_OBJECT "
		L"WHERE NYEAR=%s AND NQUART=%d AND NMONTH=%d AND EVENT='%s'",
		db.m_szPlace,
		db.m_szCurObject,
		db.m_szYear,
		db.defineQuart(),
		db.defineMonth(),
		db.m_szEventName);*/

	//AfxMessageBox(szBuffer);

	db.fQueryRead(szBuffer,crq);

	ID_INFO=crq.m_RetValues[0][0];
	
	szBuffer.Format(L"UPDATE 81 SET "
		L"T=%s, NNOM=%s, NNOM0=%s, NNOM1=%s, QRN=%s,"
		L"KPD=%s, KEF=%s, PRICE=%s, COSTS=%s, CURTIME=%s,"
		L"RESULT=%s, ECONOMY=%s "
		L"WHERE ID_PLAN_INFO=%s",
		m_list1->GetItemText(0,2),
		m_list1->GetItemText(1,2),
		m_list1->GetItemText(2,2),
		m_list1->GetItemText(3,2),
		m_list1->GetItemText(4,2),
		m_list1->GetItemText(5,2),
		m_list1->GetItemText(6,2),
		m_list1->GetItemText(7,2),
		m_list1->GetItemText(8,2),
		m_list1->GetItemText(9,2),
		m_list2.GetItemText(3,2),
		m_list2.GetItemText(4,2),
		ID_INFO);

	
	//AfxMessageBox(szBuffer);

	/*if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE 101",L"UPDATE 101F");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}*/

	//AfxMessageBox(szBuffer);

	db.ExeSql(szBuffer);
	//AfxMessageBox(szBuffer);
	szBuffer.Format(
	L"UPDATE PLAN_INFO SET "
	L"SAVEDS=%s, ECONOMY=%s, CURCOSTS=%s, CURTIME=%s "
	L"WHERE ID_PLAN_INFO=%s",
	m_list2.GetItemText(3,2),
	m_list2.GetItemText(4,2),
	m_list1->GetItemText(8,2),
	m_list1->GetItemText(9,2),
	ID_INFO);

	/*if(db.m_iStatus==FACT)
	{
		szBuffer.Replace(L"UPDATE PLAN_INFO",L"UPDATE FACT_INFO");
		szBuffer.Replace(L"ID_PLAN_INFO",L"ID_FACT_INFO");
	}*/
	
	db.ExeSql(szBuffer);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}