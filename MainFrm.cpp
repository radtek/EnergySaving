
// MainFrm.cpp : реализация класса CMainFrame
//

#include "stdafx.h"
#include "MFCApplication30.h"

#include "MainFrm.h"
#include "GenList.h"
#include "GenTree.h"
#include "SelectDirectDlg.h"
#include "Data.h"
#include "СDlgSelectPeriod.h"
#include "DlgAddEvents.h"
#include "DlgEditResIndexes.h"
#include "DlgEditYears.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

extern CData db;
extern CGenTree* pGenTree;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_SERVICE_SETTINGS, &CMainFrame::OnServiceSettings)
	//ON_COMMAND(ID_SET_DIRECT, &CMainFrame::OnSetDirect)
	ON_COMMAND(ID_MAIN_SERVICE_SETTINGS, &CMainFrame::OnMainServiceSettings)
	ON_COMMAND(ID_MAIN_SERVICE_TIME, &CMainFrame::OnMainServiceTime)
	ON_COMMAND(ID_PRINT_REPORT, &CMainFrame::OnPrintReport)
	ON_COMMAND(ID_PERIOD_ADDDEL, &CMainFrame::OnPeriodAdddel)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	/*ID_SEPARATOR,           // индикатор строки состояния
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
	*/
	ID_INDICATOR_DIRECTCALC,
	//ID_SEPARATOR,
	ID_INDICATOR_CURRENT_YEAR,
	ID_INDICATOR_CURRENT_QUARTAL,
	ID_INDICATOR_CURRENT_MONTH,
	ID_INDICATOR_GENADD,
	ID_INDICATOR_RES,
	ID_INDICATOR_CURRENT_EVENT,
	//ID_INDICATOR_CURRENT_OBJECT
};

// создание/уничтожение CMainFrame

CMainFrame::CMainFrame()
	: m_szOldYear(_T(""))
{
	// TODO: добавьте код инициализации члена
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | /*CBRS_GRIPPER |*/ CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAIN_TOOLBAR))
	{
		TRACE0("Не удалось создать панель инструментов\n");
		return -1;      // не удалось создать
	}

	if (!m_wndToolBarAdd.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_LEFT | /*CBRS_GRIPPER |*/ CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBarAdd.LoadToolBar(IDR_TOOLBAR_ADD))
	{
		TRACE0("Не удалось создать панель инструментов\n");
		return -1;      // не удалось создать
	}

	if (/*!m_wndStatusBar.Create(this)*/!m_wndStatusBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP))
	{
		TRACE0("Не удалось создать строку состояния\n");
		return -1;      // не удалось создать
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	//Задание размеров полей индикаторов статусов
	m_wndStatusBar.SetPaneInfo(m_wndStatusBar.CommandToIndex(ID_INDICATOR_DIRECTCALC), ID_INDICATOR_DIRECTCALC, SBPS_NORMAL, 15);
	m_wndStatusBar.SetPaneInfo(m_wndStatusBar.CommandToIndex(ID_INDICATOR_CURRENT_YEAR), ID_INDICATOR_CURRENT_YEAR, SBPS_NORMAL, 35);
	m_wndStatusBar.SetPaneInfo(m_wndStatusBar.CommandToIndex(ID_INDICATOR_CURRENT_QUARTAL), ID_INDICATOR_CURRENT_QUARTAL, SBPS_NORMAL, 70);
	m_wndStatusBar.SetPaneInfo(m_wndStatusBar.CommandToIndex(ID_INDICATOR_CURRENT_MONTH), ID_INDICATOR_CURRENT_MONTH, SBPS_NORMAL, 50);
	m_wndStatusBar.SetPaneInfo(m_wndStatusBar.CommandToIndex(ID_INDICATOR_CURRENT_EVENT), ID_INDICATOR_CURRENT_EVENT, SBPS_NORMAL, 700);
	m_wndStatusBar.SetPaneInfo(m_wndStatusBar.CommandToIndex(ID_INDICATOR_GENADD), ID_INDICATOR_GENADD, SBPS_NORMAL, 30);
	m_wndStatusBar.SetPaneInfo(m_wndStatusBar.CommandToIndex(ID_INDICATOR_RES), ID_INDICATOR_RES, SBPS_NORMAL, 30);
	//m_wndStatusBar.SetPaneInfo(m_wndStatusBar.CommandToIndex(ID_INDICATOR_CURRENT_OBJECT), ID_INDICATOR_CURRENT_OBJECT, SBPS_NORMAL, 200);
	
	//Заполнение статус бара пустыми строками
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_DIRECTCALC),L" ");
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_CURRENT_YEAR),L" ");
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_CURRENT_QUARTAL),L" ");
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_CURRENT_MONTH),L" ");
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_CURRENT_EVENT),L" ");
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_GENADD),L" ");
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_RES),L" ");
	//m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_CURRENT_OBJECT),L" ");

	int  index = 0;
    CRect rc;
	CRetQuery crq;

	 while (m_wndToolBar.GetItemID(index) != ID_BTN_FREEPLACE)
       index++;

	 // Делаем нашу кнопку разделителем и устанавливаем ширину в 120.
    m_wndToolBar.SetButtonInfo(index, ID_BTN_FREEPLACE, TBBS_SEPARATOR, 120);
    // Получаем размеры этого, теперь уже, разделителя.
    m_wndToolBar.GetItemRect(index, &rc);

    // Корректируем размеры в расчете на будущий контрол ComboBox.
    rc.top += 2;
    rc.bottom += 100;

    // Создаем желаемый контрол на ToolBar'е
    DWORD dwStyle = WS_VISIBLE | WS_CHILD | CBS_AUTOHSCROLL |
                    CBS_DROPDOWNLIST | CBS_HASSTRINGS;
    if (!m_wndComboPF.Create(dwStyle, rc, &m_wndToolBar, 100))
    {
        TRACE0("Failed to create combo-box\n");
        return FALSE;
    }

    // Добавляем элементы данных в наш контрол.
	//CRetQuery crq;
	//db.fQueryRead(L"SELECT YEAR_NAME FROM YEARS ORDER BY YEAR_NAME",crq);

	//for(int i=0;i<crq.m_lRecCount;i++)
	m_wndComboPF.AddString(L"ПЛАН");
	m_wndComboPF.AddString(L"ФАКТ");
	m_wndComboPF.SetCurSel(0);

	index=0;

	 while (m_wndToolBar.GetItemID(index) != ID_BTN_FREEPLACEYEAR)
       index++;

	 // Делаем нашу кнопку разделителем и устанавливаем ширину в 120.
    m_wndToolBar.SetButtonInfo(index, ID_BTN_FREEPLACEYEAR, TBBS_SEPARATOR, 120);
    // Получаем размеры этого, теперь уже, разделителя.
    m_wndToolBar.GetItemRect(index, &rc);

    // Корректируем размеры в расчете на будущий контрол ComboBox.
    rc.top += 2;
    rc.bottom += 100;

    // Создаем желаемый контрол на ToolBar'е
    dwStyle = WS_VISIBLE | WS_CHILD | CBS_AUTOHSCROLL |
                    CBS_DROPDOWNLIST | CBS_HASSTRINGS;
    if (!m_wndComboY.Create(dwStyle, rc, &m_wndToolBar, 100))
    {
        TRACE0("Failed to create combo-box\n");
        return FALSE;
    }

	db.fQueryRead(L"SELECT YEAR_NAME FROM YEARS ORDER BY ID_YEAR",crq);
	
	for(int i=0;i<crq.m_lRecCount;i++)
		m_wndComboY.AddString(crq.m_RetValues[i][0]);

	m_wndComboY.SetCurSel(crq.m_lRecCount-1);


	index=0;

	 while (m_wndToolBar.GetItemID(index) != ID_BTN_FREEPLACEQUART)
       index++;

	 // Делаем нашу кнопку разделителем и устанавливаем ширину в 120.
    m_wndToolBar.SetButtonInfo(index, ID_BTN_FREEPLACEQUART, TBBS_SEPARATOR, 120);
    // Получаем размеры этого, теперь уже, разделителя.
    m_wndToolBar.GetItemRect(index, &rc);

    // Корректируем размеры в расчете на будущий контрол ComboBox.
    rc.top += 2;
    rc.bottom += 100;

    // Создаем желаемый контрол на ToolBar'е
    dwStyle = WS_VISIBLE | WS_CHILD | CBS_AUTOHSCROLL |
                    CBS_DROPDOWNLIST | CBS_HASSTRINGS;
    if (!m_wndComboQRTL.Create(dwStyle, rc, &m_wndToolBar, 100))
    {
        TRACE0("Failed to create combo-box\n");
        return FALSE;
    }

	//db.fQueryRead(L"SELECT YEAR_NAME FROM YEARS ORDER BY YEAR_NAME",crq);
	m_wndComboQRTL.AddString(L"-");
	m_wndComboQRTL.AddString(L"I КВАРТАЛ");
	m_wndComboQRTL.AddString(L"II КВАРТАЛ");
	m_wndComboQRTL.AddString(L"III КВАРТАЛ");
	m_wndComboQRTL.AddString(L"IV КВАРТАЛ");

	m_wndComboQRTL.SetCurSel(0);

	index=0;

	while (m_wndToolBar.GetItemID(index) != ID_BTN_FREEPLACEQUART2)
      index++;

	 // Делаем нашу кнопку разделителем и устанавливаем ширину в 120.
    m_wndToolBar.SetButtonInfo(index, ID_BTN_FREEPLACEQUART2, TBBS_SEPARATOR, 120);
    // Получаем размеры этого, теперь уже, разделителя.
    m_wndToolBar.GetItemRect(index, &rc);

    // Корректируем размеры в расчете на будущий контрол ComboBox.
    rc.top += 2;
    rc.bottom += 100;

    // Создаем желаемый контрол на ToolBar'е
    dwStyle = WS_VISIBLE | WS_CHILD | CBS_AUTOHSCROLL |
                    CBS_DROPDOWNLIST | CBS_HASSTRINGS;
    if (!m_wndComboQ.Create(dwStyle, rc, &m_wndToolBar, 100))
    {
        TRACE0("Failed to create combo-box\n");
        return FALSE;
    }

	//db.fQueryRead(L"SELECT YEAR_NAME FROM YEARS ORDER BY YEAR_NAME",crq);
	m_wndComboQ.AddString(L"-");

	m_wndComboQ.SetCurSel(0);

	m_wndComboQRTL.pComboMonth=&m_wndComboQ;


	/*
	index=0;

	 while (m_wndToolBar.GetItemID(index) != ID_BTN_FREEPLACEQUART)
       index++;

	 // Делаем нашу кнопку разделителем и устанавливаем ширину в 120.
    m_wndToolBar.SetButtonInfo(index, ID_BTN_FREEPLACEQUART, TBBS_SEPARATOR, 120);
    // Получаем размеры этого, теперь уже, разделителя.
    m_wndToolBar.GetItemRect(index, &rc);

    // Корректируем размеры в расчете на будущий контрол ComboBox.
    rc.top += 2;
    rc.bottom += 100;

    // Создаем желаемый контрол на ToolBar'е
    dwStyle = WS_VISIBLE | WS_CHILD | CBS_AUTOHSCROLL |
                    CBS_DROPDOWNLIST | CBS_HASSTRINGS;
    if (!m_wndComboQ.Create(dwStyle, rc, &m_wndToolBar, 100))
    {
        TRACE0("Failed to create combo-box\n");
        return FALSE;
    }

	//db.fQueryRead(L"SELECT YEAR_NAME FROM YEARS ORDER BY YEAR_NAME",crq);
	
	m_wndComboQ.AddString(L"ЯНВАРЬ");
	m_wndComboQ.AddString(L"ФЕВРАЛЬ");
	m_wndComboQ.AddString(L"МАРТ");
	m_wndComboQ.AddString(L"АПРЕЛЬ");
	m_wndComboQ.AddString(L"МАЙ");
	m_wndComboQ.AddString(L"ИЮНЬ");
	m_wndComboQ.AddString(L"ИЮЛЬ");
	m_wndComboQ.AddString(L"АВГУСТ");
	m_wndComboQ.AddString(L"СЕНТЯБРЬ");
	m_wndComboQ.AddString(L"ОКТЯБРЬ");
	m_wndComboQ.AddString(L"НОЯБРЬ");
	m_wndComboQ.AddString(L"ДЕКАБРЬ");

	m_wndComboQ.SetCurSel(0);
	*/
	CString s; m_wndComboPF.GetLBText(m_wndComboPF.GetCurSel(),s);

	if(StrCmp(s,L"ПЛАН")==0)
	{
		db.m_iStatus=PLAN;
	}
	else
	{
		db.m_iStatus=FACT;
	}

	//this->SetWindowTextW(L"Энергосбережение");

	m_wndComboY.GetLBText(m_wndComboY.GetCurSel(),db.m_szYear);

	UpdateTreeCtrl();

	UpdateStatusBar();

	

	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	//AfxMessageBox(L"sd");

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs,
	CCreateContext* pContext)
{
	//return m_wndSplitter.Create(this,
	// 2, 2, // TODO: adjust the number of rows, columns
	// CSize(10, 10), // TODO: adjust the minimum pane size
	// pContext);
	
	// Создание статического разделенного окна из двух горизонтальных
	// областей (2 строки, 1 колонка)
	if(!this->m_wndSplitter.CreateStatic(this, 1,2))
	return FALSE;
	
	// Создание первой (верхней) области
	if(!this->m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CGenTree),
		CSize(250, lpcs->cy),pContext))
		return FALSE;
	
	// Создание второй (нижней) области
	if(!this->m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CGenList),
		CSize(500, lpcs->cy), pContext))
		return FALSE;
	
	//this->m_wndSplitter.GetPane(0,1)

	// Сделать активной вторую область
	this->SetActiveView((CView *)m_wndSplitter.GetPane(0,0));
	
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs
	//cs.x=0;
	//cs.y=200;
	return TRUE;
}

// диагностика CMainFrame

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// обработчики сообщений CMainFrame



void CMainFrame::OnServiceSettings()
{
	// TODO: добавьте свой код обработчика команд
}


void CMainFrame::OnSetDirect()
{
	// TODO: добавьте свой код обработчика команд
}


void CMainFrame::OnMainServiceSettings()
{ 
	//Если нажали на кнопку обновления статуса при неизменных комбобоксах выходим
	CString oldY;
	CString oldQ;
	CString oldM;

	int olddirect;
	int ioldM;
	CString szDirect;

 	m_wndComboY.GetLBText(m_wndComboY.GetCurSel(),oldY);
	m_wndComboQRTL.GetLBText(m_wndComboQRTL.GetCurSel(),oldQ);
	m_wndComboQ.GetLBText(m_wndComboQ.GetCurSel(),oldM);
	ioldM=m_wndComboQ.GetCurSel();
	m_wndComboPF.GetLBText(m_wndComboPF.GetCurSel(),szDirect);

	olddirect=(StrCmp(szDirect,L"ПЛАН")==0)?PLAN:FACT;

	if((StrCmp(db.m_szYear,oldY)==0)&&
		(StrCmp(db.m_szQuart,oldQ)==0)&&
		(StrCmp(db.m_szMonth,oldQ)==0)&&
		(db.m_iStatus==olddirect))
	{
		return;
	}
	//Сохранение текущих статусов три контрола
	//db.m_szCurObject=L"0";
	//db.m_szEventName=L"0";
	//db.m_szGenAdd=L"0";
	db.m_szMonth=L"0";
	db.m_szQuart=L"0";
	//db.m_szTypeResource=L"0";
	db.m_szYear=L"0";
	
	CString str;
	CString OldM;
	CString OldQ;

	m_wndComboY.GetLBText(m_wndComboY.GetCurSel(),db.m_szYear);

	m_wndComboQ.GetLBText(m_wndComboQ.GetCurSel(),str);
	
	//OldM=str;

	(StrCmp(str,L"-")==0)?db.m_szMonth=L"0":db.m_szMonth=str;
	
	m_wndComboQRTL.GetLBText(m_wndComboQRTL.GetCurSel(),str);

	//OldQ=str;

	(StrCmp(str,L"-")==0)?db.m_szQuart=L"0":db.m_szQuart=str;

	m_wndComboPF.GetLBText(m_wndComboPF.GetCurSel(),str);

	if(StrCmp(str,L"ПЛАН")==0)
	{
		db.m_iStatus=PLAN;
	}
	else
	{
		db.m_iStatus=FACT;
			
		int select=0;

		m_wndComboQ.GetLBText(m_wndComboQ.GetCurSel(),str);
	
		if((m_wndComboQ) && (m_wndComboQRTL.GetCurSel()!=0))
		{
			m_wndComboQ.ResetContent();
	
			if(m_wndComboQRTL.GetCurSel()==1)
			{
				m_wndComboQ.AddString(L"-");
				m_wndComboQ.AddString(L"ЯНВАРЬ");
				m_wndComboQ.AddString(L"ФЕВРАЛЬ");
				m_wndComboQ.AddString(L"МАРТ");
				m_wndComboQ.SetCurSel(ioldM);
			}
			else
			if(m_wndComboQRTL.GetCurSel()==2)
			{
				m_wndComboQ.AddString(L"-");
				m_wndComboQ.AddString(L"АПРЕЛЬ");
				m_wndComboQ.AddString(L"МАЙ");
				m_wndComboQ.AddString(L"ИЮНЬ");
				m_wndComboQ.SetCurSel(ioldM);
			}
			else
			if(m_wndComboQRTL.GetCurSel()==3)
			{
				m_wndComboQ.AddString(L"-");
				m_wndComboQ.AddString(L"ИЮЛЬ");
				m_wndComboQ.AddString(L"АВГУСТ");
				m_wndComboQ.AddString(L"СЕНТЯБРЬ");
				m_wndComboQ.SetCurSel(ioldM);
			}
			else
			if(m_wndComboQRTL.GetCurSel()==4)
			{
				m_wndComboQ.AddString(L"-");
				m_wndComboQ.AddString(L"ОКТЯБРЬ");
				m_wndComboQ.AddString(L"НОЯБРЬ");
				m_wndComboQ.AddString(L"ДЕКАБРЬ");
				m_wndComboQ.SetCurSel(ioldM);
			}
		}
		else
			if(m_wndComboQRTL.GetCurSel()==0)
			{
				m_wndComboQ.ResetContent();
				m_wndComboQ.AddString(L"-");
				m_wndComboQ.SetCurSel(0);
			}
			
	}

	//AfxMessageBox(L"->");

	theApp.Mes();
	
	if(StrCmp(m_szOldYear,db.m_szYear)!=0)
	{
		UpdateTreeCtrl();
	}

	m_szOldYear=db.m_szYear;

	UpdateStatusBar();
}


bool CMainFrame::UpdateStatusBar(void)
{
	//CMainFrame* pFrm=(CMainFrame*) theApp.m_pMainWnd;
	CString b,c;

	if(StrCmp(db.m_szTypeResource,L"ПРИРОДНЫЙ ГАЗ")==0)
	{
		b.Format(L"ПГ");
	}
	else
		if(StrCmp(db.m_szTypeResource,L"ЭЛЕКТРОЭНЕРГИЯ")==0)
		{
			b.Format(L"ЭЭ");
		}
		else
			if(StrCmp(db.m_szTypeResource,L"ТЕПЛОВАЯ ЭНЕРГИЯ")==0)
			{
				b.Format(L"ТЭ");
			}
	
	if(StrCmp(db.m_szGenAdd,L"ОСНОВНОЙ ОТЧЕТ")==0)
	{
		c.Format(L"ОСН");
	}
	else
		if(StrCmp(db.m_szGenAdd,L"ДОПОЛНИТЕЛЬНЫЙ ОТЧЕТ")==0)
		{
			c.Format(L"ДОП");
		}

	CString s; s.Format(L"%d",db.m_iStatus);
	this->m_wndStatusBar.SetPaneText(this->m_wndStatusBar.CommandToIndex(ID_INDICATOR_DIRECTCALC),(StrCmp(s,L"-1")==0)?L" ":(StrCmp(s,L"2")?L"П":L"Ф"));
	this->m_wndStatusBar.SetPaneText(this->m_wndStatusBar.CommandToIndex(ID_INDICATOR_CURRENT_YEAR),(StrCmp(db.m_szYear,L"0")==0)?L" ":db.m_szYear);
	this->m_wndStatusBar.SetPaneText(this->m_wndStatusBar.CommandToIndex(ID_INDICATOR_CURRENT_QUARTAL),(StrCmp(db.m_szQuart,L"0")==0)?L" ":db.m_szQuart);
	this->m_wndStatusBar.SetPaneText(this->m_wndStatusBar.CommandToIndex(ID_INDICATOR_CURRENT_MONTH),(StrCmp(db.m_szMonth,L"0")==0)?L" ":db.m_szMonth);
	this->m_wndStatusBar.SetPaneText(this->m_wndStatusBar.CommandToIndex(ID_INDICATOR_GENADD),c);
	this->m_wndStatusBar.SetPaneText(this->m_wndStatusBar.CommandToIndex(ID_INDICATOR_RES),b);
	this->m_wndStatusBar.SetPaneText(this->m_wndStatusBar.CommandToIndex(ID_INDICATOR_CURRENT_EVENT),(StrCmp(db.m_szEventName,L"0")==0)?L" ":db.m_szEventName);
	//this->m_wndStatusBar.SetPaneText(this->m_wndStatusBar.CommandToIndex(ID_INDICATOR_CURRENT_OBJECT),L" ");
	return false;
}


void CMainFrame::OnMainServiceTime()
{
	// TODO: добавьте свой код обработчика команд
	
	СDlgSelectPeriod dlg;
	//INT_PTR nRet;
	dlg.DoModal();
	UpdateStatusBar();
	UpdateTreeCtrl();	
}


bool CMainFrame::UpdateTreeCtrl(void)
{
	if(((db.m_iStatus==FACT)||(db.m_iStatus==PLAN))&&(StrCmp(db.m_szYear,L"0")!=0))
	{	
		pGenTree->FilGenTreeCtrl();
		return true;
	}
	return false;
}


void CMainFrame::OnPrintReport()
{
	//AfxMessageBox(db.GetCurrentDirectoryX());

	//this->
	if((StrCmp(db.m_szYear,L"0")!=0)&&(db.m_iStatus==PLAN)&&(StrCmp(db.m_szQuart,L"0")==0))
	{
		CString szCmdline;

		//szCmdline.Format(L"szYear=%s, Status=%d, szQuart=%s",db.m_szYear,db.m_iStatus,db.m_szQuart);

		

		//szCmdline.Format(L"D:\\Excel\\7. Работа с Excel\\bin\\Debug\\_7.Работа_с_Excel.exe %d %s %d %d",0,db.m_szYear,db.defineQuart(),db.defineMonth());
		szCmdline.Format(L"%s\\Report.exe %d %s %d %d",db.m_sFile,0,db.m_szYear,db.defineQuart(),db.defineMonth());
		AfxMessageBox(szCmdline);
		PROCESS_INFORMATION processInformation = {0};
		STARTUPINFO startupInfo                = {0};
		startupInfo.cb                         = sizeof(startupInfo);
		int nStrBuffer                         = szCmdline.GetLength() + 50;

		// Create the process
		BOOL result = CreateProcess(NULL, szCmdline.GetBuffer(nStrBuffer), 
									NULL, NULL, FALSE, 
									NORMAL_PRIORITY_CLASS, 
									NULL, NULL, &startupInfo, &processInformation);
		szCmdline.ReleaseBuffer();
	}
	else
		if((StrCmp(db.m_szYear,L"0")!=0)&&(StrCmp(db.m_szQuart,L"0")!=0)&&(StrCmp(db.m_szMonth,L"0")!=0)&&(db.m_iStatus==FACT))
		{
			CString szCmdline;
			szCmdline.Format(L"%s\\Report.exe %d %s %d %d",db.m_sFile,1,db.m_szYear,db.defineQuart(),db.defineMonth());
			AfxMessageBox(szCmdline);
			PROCESS_INFORMATION processInformation = {0};
			STARTUPINFO startupInfo                = {0};
			startupInfo.cb                         = sizeof(startupInfo);
			int nStrBuffer                         = szCmdline.GetLength() + 50;

			// Create the process
			BOOL result = CreateProcess(NULL, szCmdline.GetBuffer(nStrBuffer), 
									NULL, NULL, FALSE, 
									NORMAL_PRIORITY_CLASS, 
									NULL, NULL, &startupInfo, &processInformation);
			szCmdline.ReleaseBuffer();
		}
		else
		{
			AfxMessageBox(L"Не верно выбраны параметры...");
		}
}


bool CMainFrame::executeCommandLine(CString cmdLine, DWORD & exitCode)
{
	PROCESS_INFORMATION processInformation = {0};
	STARTUPINFO startupInfo                = {0};
    startupInfo.cb                         = sizeof(startupInfo);
    int nStrBuffer                         = cmdLine.GetLength() + 50;

    // Create the process
    BOOL result = CreateProcess(NULL, cmdLine.GetBuffer(nStrBuffer), 
                               NULL, NULL, FALSE, 
                               NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, 
                               NULL, NULL, &startupInfo, &processInformation);
    cmdLine.ReleaseBuffer();

    if (!result)
    {
      // CreateProcess() failed
      // Get the error from the system
      LPVOID lpMsgBuf;
      DWORD dw = GetLastError();
      FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
                    NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);

      // Display the error
      CString strError = (LPTSTR) lpMsgBuf;
      TRACE(_T("::executeCommandLine() failed at CreateProcess()\nCommand=%s\nMessage=%s\n\n"), cmdLine, strError);

      // Free resources created by the system
      LocalFree(lpMsgBuf);

      // We failed.
      return FALSE;
   }
   else
   {
      // Successfully created the process.  Wait for it to finish.
      WaitForSingleObject( processInformation.hProcess, INFINITE );

      // Get the exit code.
      result = GetExitCodeProcess(processInformation.hProcess, &exitCode);

      // Close the handles.
      CloseHandle( processInformation.hProcess );
      CloseHandle( processInformation.hThread );

      if (!result)
      {
         // Could not get exit code.
         TRACE(_T("Executed command but couldn't get exit code.\nCommand=%s\n"), cmdLine);
         return FALSE;
      }

      // We succeeded.
      return TRUE;
   }
	return false;
}


bool CMainFrame::UpdateComboYear(void)
{
	CRetQuery crq;
	int oldIndex=m_wndComboY.GetCurSel();

	db.fQueryRead(L"SELECT YEAR_NAME FROM YEARS ORDER BY ID_YEAR",crq);
	
	m_wndComboY.ResetContent();

	for(int i=0;i<crq.m_lRecCount;i++)
		m_wndComboY.AddString(crq.m_RetValues[i][0]);
//
	m_wndComboY.SetCurSel(oldIndex);

	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}


void CMainFrame::OnPeriodAdddel()
{
	// TODO: добавьте свой код обработчика команд
	DlgEditYears dlg;
	dlg.DoModal();
	UpdateComboYear();
}
