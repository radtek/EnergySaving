#include "stdafx.h"
#include "Data.h"
#include <afxtempl.h>
//#include "EvRem.h"
#include "resource.h"

CData::CData(void)
	: m_sSqlString(_T(""))
	, sDriver(_T(""))
	, sDsn(_T(""))
	, m_sFile(_T(""))
	, m_bDbOk(false),
	m_iStatus(-1),
	m_szPlace(_T("")),
	m_bReconfiguredPlanGTU(false),
	m_szReconfigEvent(_T("0"))
{
	sDriver = "MICROSOFT ACCESS DRIVER (*.mdb)";

	m_sFile=GetCurrentDirectoryX();
	
	sDsn.Format(L"ODBC;DRIVER={%s};DSN='';DBQ=%s\\DataBase.mdb",sDriver,m_sFile);
	TRY{
		database.Open(NULL,false,false,sDsn);
		m_bDbOk = true;
	} CATCH(CDBException,e){
		AfxMessageBox(L"Database error:"+e->m_strError);
		m_bDbOk = false;
	}
	END_CATCH;
	m_szYear = "0";
	m_szQuart = "0";
	m_szMonth = "0";
	m_szEventName = "0";
	m_szTypeResource = "0";
	m_szGenAdd = "0";
	m_szCurObject;
	m_szPlace="0";
}

CData::~CData(void)
{
	database.Close();
}

CData db;

bool CData::fQueryRead(CString Query,CRetQuery& crq)
{
	bool RetRes = false;
	TRY{
		CRecordset recset(&database);
		recset.Open(AFX_DB_USE_DEFAULT_TYPE,L"SELECT COUNT(*) FROM ("+Query+")");
		CDBVariant varValue;
		recset.GetFieldValue((short)0,varValue);
		recset.Close();

		recset.Open(AFX_DB_USE_DEFAULT_TYPE,Query);
		
		crq.m_lColCount=recset.GetODBCFieldCount();
		crq.m_lRecCount = varValue.m_lVal;

		crq.m_RetValues =  new CString* [crq.m_lRecCount];
		for(long i =0;i<crq.m_lRecCount;i++)
			crq.m_RetValues[i] = new CString[crq.m_lColCount];

		short m_lSelectRow=0;

		if (!(crq.m_lRecCount==0))
		{
			while(!recset.IsEOF()&&(!(crq.m_lRecCount==0)))
			{
				for(short i = 0;i<crq.m_lColCount;i++)
					recset.GetFieldValue(i,crq.m_RetValues[m_lSelectRow][i]);

				m_lSelectRow +=1;

				recset.MoveNext();
			}
			RetRes=true;
		}
		else
		{
			RetRes=false;
		}
	} CATCH(CDBException,e){
		AfxMessageBox(L"Database error 2:"+e->m_strError);
		m_bDbOk = false;
		RetRes = false;
	}
	END_CATCH;
	return RetRes;
}

//Заполнение CTreeCtrl
bool CData::FillGenTreeCtrl(CTreeCtrl& ref_tree)
{
	CRetQuery crq;
	HTREEITEM* hYears;
	HTREEITEM hKvart[4];
	TVITEM tv; // Атрибуты элемента дерева	
	TVINSERTSTRUCT tvins; // Информация для добавления нового элемента к дереву
	CString pszKvart[4];
	CString pszMonths[12];
	int limit_month=0;
	long sel_image=0;

	pszKvart[0]="I КВАРТАЛ";
	pszKvart[1]="II КВАРТАЛ";
	pszKvart[2]="III КВАРТАЛ";
	pszKvart[3]="IV КВАРТАЛ";

	pszMonths[0]="ЯНВАРЬ";
	pszMonths[1]="ФЕВРАЛЬ";
	pszMonths[2]="МАРТ";
	pszMonths[3]="АПРЕЛЬ";
	pszMonths[4]="МАЙ";
	pszMonths[5]="ИЮНЬ";
	pszMonths[6]="ИЮЛЬ";
	pszMonths[7]="АВГУСТ";
	pszMonths[8]="СЕНТЯБРЬ";
	pszMonths[9]="ОКТЯБРЬ";
	pszMonths[10]="НОЯБРЬ";
	pszMonths[11]="ДЕКАБРЬ";

	ref_tree.DeleteAllItems();
	
	db.fQueryRead(L"SELECT YEAR_NAME FROM YEARS",crq);

	if(crq.m_lRecCount==0)
		return 0;

	hYears=new HTREEITEM[crq.m_lRecCount];

	for(int i=0;i<crq.m_lRecCount;i++)
	{
		tv.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tv.pszText = T2W(crq.m_RetValues[i][0].GetBuffer());
		crq.m_RetValues[i][0].ReleaseBuffer();
		tv.cchTextMax = 5;
		tv.iImage = 0;
		tv.iSelectedImage = 0;
		tvins.hParent = TVI_ROOT;
		tvins.hInsertAfter = TVI_LAST;
		tvins.item = tv;
		hYears[i] = ref_tree.InsertItem(&tvins);
		ref_tree.SetItemState(hYears[i], 0, 0);
		ref_tree.Expand(hYears[i], TVE_EXPAND);

		for(int i2=0;i2<4;i2++)
		{
			// Добавление элемента1, выпадающего из корня
			tv.pszText = T2W(pszKvart[i2].GetBuffer());
			pszKvart[i2].ReleaseBuffer();
			tv.cchTextMax = 10;	
			tv.iImage = 1;
			tv.iSelectedImage = 1;
			tvins.hParent = hYears[i];
			tvins.hInsertAfter = TVI_LAST;
			tvins.item = tv;
			hKvart[i2] = ref_tree.InsertItem(&tvins);
			
			for(int i3=limit_month;i3<limit_month+3;i3++)
			{
				tv.pszText = T2W(pszMonths[i3].GetBuffer());
				pszMonths[i3].ReleaseBuffer();
				tv.cchTextMax = 10;	
				tv.iImage = i3+2;
				tv.iSelectedImage = i3+2;
				tvins.hParent = hKvart[i2];
				tvins.hInsertAfter = TVI_LAST;
				tvins.item = tv;
				ref_tree.InsertItem(&tvins);
			}
			if(limit_month<8)
				limit_month+=3;
		}
		limit_month=0;
		//деструктор для crq
		for (int i = 0; i < crq.m_lRecCount; i++)	
		{
			delete [] crq.m_RetValues[i];
		}
		if(crq.m_lRecCount>0)
			delete [] crq.m_RetValues;
	}
	

	hYears=NULL;
	delete hYears;

	return 1;
}

void CData::ExeSql(CString Query)
{
	database.ExecuteSQL(Query);
}

/*
1 параметр:

Year - 0
Quart - 1
Month - 2

2 параметр:

Отчет - 0
Ресурс - 1
Мероприятие - 2
*/

bool CData::procElemreports(CListCtrl& reflistObj)
{

	if(!(this->pF1)||!(this->pF2)||!(this->pF3)) return 0;

		if(StrCmp(db.m_szEventName,L"0")==0)
		{
			if(StrCmp(db.m_szTypeResource,L"0")==0)
			{
				(this->*pF1)(reflistObj);
			}
			else
			{
				(this->*pF2)(reflistObj);
			}
		}
		else
		{
			(this->*pF3)(reflistObj);
		}
	return 0;
}

int CData::defineQuart(void)
{
	int quart=0;

	if(StrCmp(db.m_szQuart,L"I КВАРТАЛ")==0)
	{
		quart = 1;
	}
	else
	if(StrCmp(db.m_szQuart,L"II КВАРТАЛ")==0)
	{
		quart = 2;
	}
	else
	if(StrCmp(db.m_szQuart,L"III КВАРТАЛ")==0)
	{
		quart = 3;
	}
	else
	if(StrCmp(db.m_szQuart,L"IV КВАРТАЛ")==0)
	{
		quart = 4;
	}
	return quart;
}

int CData::defineResourceType(void)
{
	int resource = 0;
	if(StrCmp(db.m_szTypeResource,L"ПРИРОДНЫЙ ГАЗ")==0)
	{
		resource = 2;
	}
	else
	if(StrCmp(db.m_szTypeResource,L"ЭЛЕКТРОЭНЕРГИЯ")==0)
	{
		resource = 3;
	}
	else
	if(StrCmp(db.m_szTypeResource,L"ТЕПЛОВАЯ ЭНЕРГИЯ")==0)
	{
		resource = 4;
	}
	return resource;
}

bool CData::q00(CListCtrl& refListObj)
{
	CString str;
	CRetQuery crq;
	int iDirect = (StrCmp(db.m_szGenAdd,L"Основной")==0)?0:1; //0 - основной, 1 - доп.
	refListObj.SetExtendedStyle(refListObj.GetExtendedStyle() | LVS_EX_GRIDLINES);
					refListObj.InsertColumn(0,L"#п/п",LVCFMT_LEFT,50);
					refListObj.InsertColumn(1,L"Ресурс",LVCFMT_LEFT,100);
					refListObj.InsertColumn(2,L"Е.И.",LVCFMT_LEFT,100);
					refListObj.InsertColumn(3,L"Плановый эффект",LVCFMT_LEFT,100);
					refListObj.InsertColumn(4,L"Экономический эффект,руб",LVCFMT_LEFT,100);
					refListObj.InsertItem(0,L"1",0);
					refListObj.InsertItem(1,L"2",0);
					refListObj.InsertItem(2,L"3",0);
					refListObj.SetItemText(0,1,L"Природный газ");
					refListObj.SetItemText(1,1,L"Электроэнергия");
					refListObj.SetItemText(2,1,L"Тепловая энергия");
					refListObj.SetItemText(0,2,L"тыс.м3");
					refListObj.SetItemText(1,2,L"кВт x ч");
					refListObj.SetItemText(2,2,L"Гкал");
					//Плановые показатели
					str.Format(L"%s%d%s%s%s",L"SELECT SUM(PLAN_INFO.SAVEDS) FROM (EVENTS INNER JOIN PLAN_INFO ON EVENTS.EVENT_NAME = PLAN_INFO.EVENT) WHERE PLAN_INFO.GENADD = ",
						iDirect,L" AND PLAN_INFO.NYEAR=",
						db.m_szYear,
						L" AND EVENTS.ID_EVENT_TYPE=2");
					db.fQueryRead(str,crq);
					refListObj.SetItemText(0,3,crq.m_RetValues[0][0]);
					str.Format(L"%s%d%s%s%s",L"SELECT SUM(PLAN_INFO.SAVEDS) FROM (EVENTS INNER JOIN PLAN_INFO ON EVENTS.EVENT_NAME = PLAN_INFO.EVENT)"
						L" WHERE PLAN_INFO.GENADD = ",iDirect,L" AND PLAN_INFO.NYEAR=",db.m_szYear,L" AND EVENTS.ID_EVENT_TYPE=3");
					db.fQueryRead(str,crq);
					refListObj.SetItemText(1,3,crq.m_RetValues[0][0]);
					str.Format(L"%s%d%s%s%s",L"SELECT SUM(PLAN_INFO.SAVEDS) FROM (EVENTS INNER JOIN PLAN_INFO ON EVENTS.EVENT_NAME = PLAN_INFO.EVENT)"
						L" WHERE PLAN_INFO.GENADD = ",iDirect,L" AND PLAN_INFO.NYEAR=",db.m_szYear,L" AND EVENTS.ID_EVENT_TYPE=4");
					db.fQueryRead(str,crq);
					refListObj.SetItemText(2,3,crq.m_RetValues[0][0]);

					str.Format(L"%s%d%s%s%s",L"SELECT SUM(PLAN_INFO.ECONOMY) FROM (EVENTS INNER JOIN PLAN_INFO ON EVENTS.EVENT_NAME = PLAN_INFO.EVENT)"
						L" WHERE PLAN_INFO.GENADD = ",iDirect,L" AND PLAN_INFO.NYEAR=",db.m_szYear,L" AND EVENTS.ID_EVENT_TYPE=2");
					db.fQueryRead(str,crq);
					refListObj.SetItemText(0,4,crq.m_RetValues[0][0]);
					str.Format(L"%s%d%s%s%s",L"SELECT SUM(PLAN_INFO.ECONOMY) FROM (EVENTS INNER JOIN PLAN_INFO ON EVENTS.EVENT_NAME = PLAN_INFO.EVENT)"
						L" WHERE PLAN_INFO.GENADD = ",iDirect,L" AND PLAN_INFO.NYEAR=",db.m_szYear,L" AND EVENTS.ID_EVENT_TYPE=3");
					db.fQueryRead(str,crq);
					refListObj.SetItemText(1,4,crq.m_RetValues[0][0]);
					str.Format(L"%s%d%s%s%s",L"SELECT SUM(PLAN_INFO.ECONOMY) FROM (EVENTS INNER JOIN PLAN_INFO ON EVENTS.EVENT_NAME = PLAN_INFO.EVENT)"
						L" WHERE PLAN_INFO.GENADD = ",iDirect,L" AND PLAN_INFO.NYEAR=",db.m_szYear,L" AND EVENTS.ID_EVENT_TYPE=4");
					db.fQueryRead(str,crq);
					refListObj.SetItemText(2,4,crq.m_RetValues[0][0]);
					//деструктор для crq
					for (int i = 0; i < crq.m_lRecCount; i++)	
					{
						delete [] crq.m_RetValues[i];
					}
					if(crq.m_lRecCount>0)
						delete [] crq.m_RetValues;
					return 0;
}

bool CData::q01(CListCtrl& refListObj)
{
	CString str;
	CRetQuery crq;
	int rsrce = defineResourceType();
	refListObj.SetExtendedStyle(refListObj.GetExtendedStyle() | LVS_EX_GRIDLINES);
					refListObj.InsertColumn(0,L"#п/п",LVCFMT_LEFT,50);
					refListObj.InsertColumn(1,L"Наименование мероприятия",LVCFMT_LEFT,100);
					refListObj.InsertColumn(2,L"Е.И.",LVCFMT_LEFT,100);
					refListObj.InsertColumn(3,L"Эффект",LVCFMT_LEFT,100);
					refListObj.InsertColumn(4,L"Эк.эффект, руб",LVCFMT_LEFT,100);
					refListObj.InsertColumn(5,L"Эк. эффект общий, руб",LVCFMT_LEFT,100);
					refListObj.InsertColumn(6,L"З план.за год, руб.",LVCFMT_LEFT,100);
					refListObj.InsertColumn(7,L"З план. общее, руб.",LVCFMT_LEFT,100);
					refListObj.InsertColumn(8,L"Т план.за год, ч",LVCFMT_LEFT,100);
					refListObj.InsertColumn(9,L"Т план. общее, ч",LVCFMT_LEFT,100);
					
					str.Format(L"%s%d%s%d%s%s",L"SELECT PLAN_EVENT.EVENT FROM "
											 L"(PLAN_EVENT INNER JOIN EVENTS ON PLAN_EVENT.EVENT=EVENTS.EVENT_NAME)" 
											 L" WHERE PLAN_EVENT.GENADD=",((StrCmp(db.m_szGenAdd,L"Основной")==0)?0:1),L" AND "
											 L"EVENTS.ID_EVENT_TYPE=",rsrce,L" AND "
											 L"PLAN_EVENT.NYEAR=",db.m_szYear);
					db.fQueryRead(str,crq);
					
					
					CString* buffer=new CString[crq.m_lRecCount];
					for(int i=0;i<crq.m_lRecCount;i++)
					{
						buffer[i] = crq.m_RetValues[i][0];
					}
					//buffer = crq.m_RetValues[][0];
					int iCountRow = crq.m_lRecCount;
					for(int i=0;i<iCountRow;i++)
					{
						str.Format(L"%d",(i+1));
						refListObj.InsertItem(i,str,0);
						refListObj.SetItemText(i,1,crq.m_RetValues[i][0]);
						switch(rsrce)
						{
						case 2:
							refListObj.SetItemText(i,2,L"тыс.м3");
							break;
						case 3:
							refListObj.SetItemText(i,2,L"тыс.кВт x ч");
							break;
						case 4:
							refListObj.SetItemText(i,2,L"Гкал");
							break;
						default:
							break;
						}
					}

					for(int i =0;i<crq.m_lRecCount;i++)
					{
						str.Format(L"%s%s%s%d%s%s%s",L"SELECT SUM(PLAN_INFO.SAVEDS), SUM(ECONOMY), SUM(CURCOSTS), SUM(CURTIME) FROM "
							L"(PLAN_INFO INNER JOIN PLAN_EVENT ON PLAN_INFO.EVENT=PLAN_EVENT.EVENT) WHERE "
							L"PLAN_INFO.NYEAR = ",db.m_szYear,
							L" AND PLAN_INFO.GENADD=",((StrCmp(db.m_szGenAdd,L"Основной")==0)?0:1),
							L" AND PLAN_EVENT.EVENT = '",buffer[i],
							L"'");
						
						db.fQueryRead(str,crq);
						refListObj.SetItemText(i,3,crq.m_RetValues[0][0]);
						refListObj.SetItemText(i,4,crq.m_RetValues[0][1]);
						refListObj.SetItemText(i,6,crq.m_RetValues[0][2]);
						refListObj.SetItemText(i,8,crq.m_RetValues[0][3]);
					}

					for(int i =0;i<crq.m_lRecCount;i++)
					{
						str.Format(L"%s%s%s",L"SELECT TOTALPLANECONOMY, TOTALPLANCOSTS, TOTALPLANTIME"
								   L" FROM EVENTS"
								   L" WHERE EVENT_NAME='",buffer[i],"'");
						
						db.fQueryRead(str,crq);
						refListObj.SetItemText(i,5,crq.m_RetValues[0][0]);
						refListObj.SetItemText(i,7,crq.m_RetValues[0][1]);
						refListObj.SetItemText(i,9,crq.m_RetValues[0][2]);
					}
					//деструктор для crq
					for (int i = 0; i < crq.m_lRecCount; i++)	
					{
						delete [] crq.m_RetValues[i];
					}
					if(crq.m_lRecCount>0)
						delete [] crq.m_RetValues;

					return 0;
}

bool CData::q02(CListCtrl& refListObj)
{
	//DISABLED
	return 0;
}

bool CData::q10(CListCtrl& refListObj)
{
	
	int quart = defineQuart();
	CString str;
	CRetQuery crq;
	int iDirect = (StrCmp(db.m_szGenAdd,L"Основной")==0)?0:1; //0 - основной, 1 - доп.
	refListObj.SetExtendedStyle(refListObj.GetExtendedStyle() | LVS_EX_GRIDLINES);
					refListObj.InsertColumn(0,L"#п/п",LVCFMT_LEFT,50);
					refListObj.InsertColumn(1,L"Ресурс",LVCFMT_LEFT,100);
					refListObj.InsertColumn(2,L"Е.И.",LVCFMT_LEFT,100);
					refListObj.InsertColumn(3,L"Плановый эффект",LVCFMT_LEFT,100);
					refListObj.InsertColumn(4,L"Экономический эффект,руб",LVCFMT_LEFT,100);
					refListObj.InsertItem(0,L"1",0);
					refListObj.InsertItem(1,L"2",0);
					refListObj.InsertItem(2,L"3",0);
					refListObj.SetItemText(0,1,L"Природный газ");
					refListObj.SetItemText(1,1,L"Электроэнергия");
					refListObj.SetItemText(2,1,L"Тепловая энергия");
					refListObj.SetItemText(0,2,L"тыс.м3");
					refListObj.SetItemText(1,2,L"кВт x ч");
					refListObj.SetItemText(2,2,L"Гкал");
					//Плановые показатели
					str.Format(L"%s%d%s%s%s%s%d",L"SELECT SUM(PLAN_INFO.SAVEDS) FROM (EVENTS INNER JOIN PLAN_INFO ON EVENTS.EVENT_NAME = PLAN_INFO.EVENT) WHERE PLAN_INFO.GENADD = ",
						iDirect,L" AND PLAN_INFO.NYEAR=",
						db.m_szYear,
						L" AND EVENTS.ID_EVENT_TYPE=2 AND ",
						L"PLAN_INFO.NQUART=",
						quart);
					db.fQueryRead(str,crq);
					refListObj.SetItemText(0,3,crq.m_RetValues[0][0]);
					str.Format(L"%s%d%s%s%s%d",L"SELECT SUM(PLAN_INFO.SAVEDS) FROM (EVENTS INNER JOIN PLAN_INFO ON EVENTS.EVENT_NAME = PLAN_INFO.EVENT)"
						L" WHERE PLAN_INFO.GENADD = ",iDirect,L" AND PLAN_INFO.NYEAR=",db.m_szYear,L" AND EVENTS.ID_EVENT_TYPE=3 AND PLAN_INFO.NQUART=",quart);
					db.fQueryRead(str,crq);
					refListObj.SetItemText(1,3,crq.m_RetValues[0][0]);
					str.Format(L"%s%d%s%s%s%d",L"SELECT SUM(PLAN_INFO.SAVEDS) FROM (EVENTS INNER JOIN PLAN_INFO ON EVENTS.EVENT_NAME = PLAN_INFO.EVENT)"
						L" WHERE PLAN_INFO.GENADD = ",iDirect,L" AND PLAN_INFO.NYEAR=",db.m_szYear,L" AND EVENTS.ID_EVENT_TYPE=4 AND PLAN_INFO.NQUART=",quart);
					db.fQueryRead(str,crq);
					refListObj.SetItemText(2,3,crq.m_RetValues[0][0]);
					str.Format(L"%s%d%s%s%s%d",L"SELECT SUM(PLAN_INFO.ECONOMY) FROM (EVENTS INNER JOIN PLAN_INFO ON EVENTS.EVENT_NAME = PLAN_INFO.EVENT)"
						L" WHERE PLAN_INFO.GENADD = ",iDirect,L" AND PLAN_INFO.NYEAR=",db.m_szYear,L" AND EVENTS.ID_EVENT_TYPE=2 AND PLAN_INFO.NQUART=",quart);
					db.fQueryRead(str,crq);
					refListObj.SetItemText(0,4,crq.m_RetValues[0][0]);
					str.Format(L"%s%d%s%s%s%d",L"SELECT SUM(PLAN_INFO.ECONOMY) FROM (EVENTS INNER JOIN PLAN_INFO ON EVENTS.EVENT_NAME = PLAN_INFO.EVENT)"
						L" WHERE PLAN_INFO.GENADD = ",iDirect,L" AND PLAN_INFO.NYEAR=",db.m_szYear,L" AND EVENTS.ID_EVENT_TYPE=3 AND PLAN_INFO.NQUART=",quart);
					db.fQueryRead(str,crq);
					refListObj.SetItemText(1,4,crq.m_RetValues[0][0]);
					str.Format(L"%s%d%s%s%s%d",L"SELECT SUM(PLAN_INFO.ECONOMY) FROM (EVENTS INNER JOIN PLAN_INFO ON EVENTS.EVENT_NAME = PLAN_INFO.EVENT)"
						L" WHERE PLAN_INFO.GENADD = ",iDirect,L" AND PLAN_INFO.NYEAR=",db.m_szYear,L" AND EVENTS.ID_EVENT_TYPE=4 AND PLAN_INFO.NQUART=",quart);
					db.fQueryRead(str,crq);
					refListObj.SetItemText(2,4,crq.m_RetValues[0][0]);

					//деструктор для crq
					for (int i = 0; i < crq.m_lRecCount; i++)	
					{
						delete [] crq.m_RetValues[i];
					}
					if(crq.m_lRecCount>0)
						delete [] crq.m_RetValues;

					return 0;
}

bool CData::q11(CListCtrl& refListObj)
{
	CString str;
	CRetQuery crq;
	int quart = defineQuart();
	int rsrce = defineResourceType();
	refListObj.SetExtendedStyle(refListObj.GetExtendedStyle() | LVS_EX_GRIDLINES);
					refListObj.InsertColumn(0,L"#п/п",LVCFMT_LEFT,50);
					refListObj.InsertColumn(1,L"Наименование мероприятия",LVCFMT_LEFT,100);
					refListObj.InsertColumn(2,L"Е.И.",LVCFMT_LEFT,100);
					refListObj.InsertColumn(3,L"Эффект",LVCFMT_LEFT,100);
					refListObj.InsertColumn(4,L"Эк.эффект, руб",LVCFMT_LEFT,100);
					refListObj.InsertColumn(5,L"Эк. эффект общий, руб",LVCFMT_LEFT,100);
					refListObj.InsertColumn(6,L"З план.за квартал, руб.",LVCFMT_LEFT,100);
					refListObj.InsertColumn(7,L"З план. общее, руб.",LVCFMT_LEFT,100);
					refListObj.InsertColumn(8,L"Т план.за квартал, ч",LVCFMT_LEFT,100);
					refListObj.InsertColumn(9,L"Т план. общее, ч",LVCFMT_LEFT,100);
					
					str.Format(L"%s%d%s%d%s%s",L"SELECT PLAN_EVENT.EVENT FROM "
											 L"(PLAN_EVENT INNER JOIN EVENTS ON PLAN_EVENT.EVENT=EVENTS.EVENT_NAME)" 
											 L" WHERE PLAN_EVENT.GENADD=",((StrCmp(db.m_szGenAdd,L"Основной")==0)?0:1),L" AND "
											 L"EVENTS.ID_EVENT_TYPE=",rsrce,L" AND "
											 L"PLAN_EVENT.NYEAR=",db.m_szYear);
					db.fQueryRead(str,crq);
					
					//const int n =  crq.m_lRecCount;
					CString* buffer=new CString[crq.m_lRecCount];
					for(int i=0;i<crq.m_lRecCount;i++)
					{
						buffer[i] = crq.m_RetValues[i][0];
					}
					//buffer = crq.m_RetValues[][0];
					int iCountRow = crq.m_lRecCount;
					for(int i=0;i<iCountRow;i++)
					{
						str.Format(L"%d",(i+1));
						refListObj.InsertItem(i,str,0);
						refListObj.SetItemText(i,1,crq.m_RetValues[i][0]);
						switch(rsrce)
						{
						case 2:
							refListObj.SetItemText(i,2,L"тыс.м3");
							break;
						case 3:
							refListObj.SetItemText(i,2,L"тыс.кВт x ч");
							break;
						case 4:
							refListObj.SetItemText(i,2,L"Гкал");
							break;
						default:
							break;
						}
					}
					
					for(int i =0;i<crq.m_lRecCount;i++)
					{
						str.Format(L"SELECT T3.TOTAL1, T3.TOTAL2, T3.TOTAL3, T3.TOTAL4 "
						L"FROM (SELECT T2.EVENT, SUM(T2.SAVEDS) AS TOTAL1, SUM(ECONOMY) AS TOTAL2, SUM(CURCOSTS) AS TOTAL3, SUM(CURTIME) AS TOTAL4 FROM (SELECT T1.EVENT, T1.SAVEDS, T1.ECONOMY, T1.CURCOSTS, T1.CURTIME "
						L"FROM PLAN_INFO AS T1 "
						L"WHERE T1.NYEAR=%s AND T1.NQUART=%d AND T1.EVENT='%s') T2 "
						L"GROUP BY T2.EVENT) T3 INNER JOIN PLAN_EVENT AS T4 ON T3.EVENT=T4.EVENT WHERE T4.NYEAR=%s",db.m_szYear,quart,buffer[i],db.m_szYear);
						
						db.fQueryRead(str,crq);
						refListObj.SetItemText(i,3,crq.m_RetValues[0][0]);
						refListObj.SetItemText(i,4,crq.m_RetValues[0][1]);
						refListObj.SetItemText(i,6,crq.m_RetValues[0][2]);
						refListObj.SetItemText(i,8,crq.m_RetValues[0][3]);
					}

					for(int i =0;i<crq.m_lRecCount;i++)
					{
						str.Format(L"%s%s%s",L"SELECT TOTALPLANECONOMY, TOTALPLANCOSTS, TOTALPLANTIME"
								   L" FROM EVENTS"
								   L" WHERE EVENT_NAME='",buffer[i],"'");
						
						db.fQueryRead(str,crq);
						refListObj.SetItemText(i,5,crq.m_RetValues[0][0]);
						refListObj.SetItemText(i,7,crq.m_RetValues[0][1]);
						refListObj.SetItemText(i,9,crq.m_RetValues[0][2]);
					}

					//деструктор для crq
					for (int i = 0; i < crq.m_lRecCount; i++)	
					{
						delete [] crq.m_RetValues[i];
					}
					if(crq.m_lRecCount>0)
					delete [] crq.m_RetValues;

					buffer=NULL;
					delete buffer;

					return 0;
}

bool CData::q12(CListCtrl& refListObj)
{
	//DISABLED
	return 0;
}

bool CData::FillListView(CListCtrl& refListObj)
{
//	CRetQuery crq;

	CString str;
	
	int iCountRow = 0;

	if(StrCmp(db.m_szMonth,L"0")==0)
	{
		if(StrCmp(db.m_szQuart,L"0")==0)
		{
			pF1 = &CData::q00;
			pF2 = &CData::q01;
			pF3 = &CData::q02;
		}
		else
		{
			pF1 = &CData::q10;
			pF2 = &CData::q11;
			pF3 = &CData::q12;
		}
	}
	else
	{
		pF1 = &CData::q20;
		pF2 = &CData::q21;
		pF3 = &CData::q22;
	}
	procElemreports(refListObj);

	

	return 0;
}

bool CData::q20(CListCtrl& refListObj)
{
	CString str;
	CRetQuery crq;
	int month = defineMonth();
	int iDirect = (StrCmp(db.m_szGenAdd,L"Основной")==0)?0:1; //0 - основной, 1 - доп.
	refListObj.SetExtendedStyle(refListObj.GetExtendedStyle() | LVS_EX_GRIDLINES);
					refListObj.InsertColumn(0,L"#п/п",LVCFMT_LEFT,50);
					refListObj.InsertColumn(1,L"Ресурс",LVCFMT_LEFT,100);
					refListObj.InsertColumn(2,L"Е.И.",LVCFMT_LEFT,100);
					refListObj.InsertColumn(3,L"Плановый эффект",LVCFMT_LEFT,100);
					refListObj.InsertColumn(4,L"Экономический эффект,руб",LVCFMT_LEFT,100);
					refListObj.InsertItem(0,L"1",0);
					refListObj.InsertItem(1,L"2",0);
					refListObj.InsertItem(2,L"3",0);
					refListObj.SetItemText(0,1,L"Природный газ");
					refListObj.SetItemText(1,1,L"Электроэнергия");
					refListObj.SetItemText(2,1,L"Тепловая энергия");
					refListObj.SetItemText(0,2,L"тыс.м3");
					refListObj.SetItemText(1,2,L"кВт x ч");
					refListObj.SetItemText(2,2,L"Гкал");
					str.Format(L"%s%s%s%d",L"SELECT SUM(SAVEDS), SUM(ECONOMY) "
						       L"FROM PLAN_INFO "
							   L"WHERE NYEAR=",db.m_szYear,
							   L" AND MESUAR='тыс.м3' AND NMONTH=",month);
					db.fQueryRead(str,crq);
					refListObj.SetItemText(0,3,crq.m_RetValues[0][0]);
					refListObj.SetItemText(0,3,crq.m_RetValues[0][1]);
					str.Format(L"%s%s%s%d",L"SELECT SUM(SAVEDS), SUM(ECONOMY) "
						       L"FROM PLAN_INFO "
							   L"WHERE NYEAR=",db.m_szYear,
							   L" AND MESUAR='тыс.кВт x ч.' AND NMONTH=",month);
					db.fQueryRead(str,crq);
					refListObj.SetItemText(1,3,crq.m_RetValues[0][0]);
					refListObj.SetItemText(1,3,crq.m_RetValues[0][1]);
					str.Format(L"%s%s%s%d",L"SELECT SUM(SAVEDS), SUM(ECONOMY) "
						       L"FROM PLAN_INFO "
							   L"WHERE NYEAR=",db.m_szYear,
							   L" AND MESUAR='Гкал' AND NMONTH=",month);
					db.fQueryRead(str,crq);
					refListObj.SetItemText(2,3,crq.m_RetValues[0][0]);
					refListObj.SetItemText(2,3,crq.m_RetValues[0][1]);

					//деструктор для crq
					for (int i = 0; i < crq.m_lRecCount; i++)	
					{
						delete [] crq.m_RetValues[i];
					}
					if(crq.m_lRecCount>0)
					delete [] crq.m_RetValues;

	return false;
}

bool CData::q21(CListCtrl& refListObj)
{
	CString str;
	CRetQuery crq;
	int quart = defineQuart();
	int rsrce = defineResourceType();
	int month = defineMonth();
	int iDirect = (StrCmp(db.m_szGenAdd,L"Основной")==0)?0:1; //0 - основной, 1 - доп.
	refListObj.SetExtendedStyle(refListObj.GetExtendedStyle() | LVS_EX_GRIDLINES);
					refListObj.InsertColumn(0,L"#п/п",LVCFMT_LEFT,50);
					refListObj.InsertColumn(1,L"Наименование мероприятия",LVCFMT_LEFT,100);
					refListObj.InsertColumn(2,L"Е.И.",LVCFMT_LEFT,100);
					refListObj.InsertColumn(3,L"Эффект",LVCFMT_LEFT,100);
					refListObj.InsertColumn(4,L"Эк.эффект, руб",LVCFMT_LEFT,100);
					refListObj.InsertColumn(5,L"Затраты, руб",LVCFMT_LEFT,100);
					refListObj.InsertColumn(6,L"Т план, ч",LVCFMT_LEFT,100);
					str.Format(L"%s%s%s%s%s%d%s%d",L"SELECT T4.EVENT,T4.MESUAR,T4.SAVEDS,T4.ECONOMY,T4.CURCOSTS,T4.CURTIME" 
							   L" FROM "
							   L"((SELECT T1.EVENT,T2.EVENT_TYPE_NAME " 
							   L"FROM PLAN_EVENT AS T1 " 
							   L"INNER JOIN EVENT_TYPES AS T2 "
							   L"ON T1.ID_EVENT_TYPE=T2.ID_EVENT_TYPE "
							   L"WHERE T1.NYEAR=2037 AND T2.EVENT_TYPE_NAME='",db.m_szTypeResource,L"') T3 "
							   L"INNER JOIN PLAN_INFO AS T4 "
							   L"ON T3.EVENT=T4.EVENT) " 
							   L"WHERE T4.NYEAR=",db.m_szYear,L" AND T4.NMONTH=",month,L" AND T4.GENADD=",iDirect);
					
					db.fQueryRead(str,crq);
					for(int i =0;i<crq.m_lRecCount;i++)
					{
						str.Format(L"%d",i+1);
						refListObj.InsertItem(i,str,0);
						refListObj.SetItemText(i,1,crq.m_RetValues[i][0]);
						refListObj.SetItemText(i,2,crq.m_RetValues[i][1]);
						refListObj.SetItemText(i,3,crq.m_RetValues[i][2]);
						refListObj.SetItemText(i,4,crq.m_RetValues[i][3]);
						refListObj.SetItemText(i,5,crq.m_RetValues[i][4]);
						refListObj.SetItemText(i,6,crq.m_RetValues[i][5]);
					}

					//деструктор для crq
					for (int i = 0; i < crq.m_lRecCount; i++)	
					{
						delete [] crq.m_RetValues[i];
					}
					if(crq.m_lRecCount>0)
					delete [] crq.m_RetValues;
	return false;
}

bool CData::q22(CListCtrl& refListObj)
{
	//viewobjects(refListObj);
	CString str;
	CRetQuery crq;
	CString id_plan_info;
	int quart = defineQuart();
	int rsrce = defineResourceType();
	int month = defineMonth();
	int iDirect = (StrCmp(db.m_szGenAdd,L"Основной")==0)?0:1; //0 - основной, 1 - доп.
	refListObj.SetExtendedStyle(refListObj.GetExtendedStyle() | LVS_EX_GRIDLINES);
					refListObj.InsertColumn(0,L"#п/п",LVCFMT_LEFT,50);
					refListObj.InsertColumn(1,L"Объект",LVCFMT_LEFT,100);
					refListObj.InsertColumn(2,L"Е.И.",LVCFMT_LEFT,100);
					refListObj.InsertColumn(3,L"Эффект",LVCFMT_LEFT,100);
					refListObj.InsertColumn(4,L"Эк.эффект, руб",LVCFMT_LEFT,100);
					refListObj.InsertColumn(5,L"Затраты, руб",LVCFMT_LEFT,100);
					refListObj.InsertColumn(6,L"Т план, ч",LVCFMT_LEFT,100);
					/*
					str.Format(L"%s%s%s%d%s%d%s%s%s",L"SELECT T2.OBJECT_NAME,T1.MESUAR,T1.SAVEDS,"
							       L"T1.ECONOMY,T1.CURCOSTS,T1.CURTIME "
								   L"FROM PLAN_INFO AS T1 INNER JOIN OBJECTS_PLANS AS T2 "
								   L"ON T1.ID_PLAN_INFO=T2.ID_PLAN_INFO "
								   L"WHERE T1.NYEAR=",db.m_szYear,L" AND T1.NMONTH=",month,L" AND T1.GENADD=",((StrCmp(db.m_szGenAdd,L"Основной")==0)?0:1),
								   L" AND EVENT='",db.m_szEventName,"'");
					*/
					str.Format(L"SELECT T1.OBJECT_NAME, T2.MESUAR, T4.RESULT, T4.ECONOMY, T4.COSTS, T4.CURTIME FROM (SELECT T1.ID_OBJECT, T1.OBJECT_NAME, T2.MESUAR FROM OBJECTS_PLANS AS T1 INNER JOIN PLAN_INFO AS T2 "
					L"ON T1.ID_PLAN_INFO=T2.ID_PLAN_INFO "
					L"WHERE T2.NYEAR=%s AND T2.NMONTH=%d AND T2.EVENT='%s') T3 "
					L"INNER JOIN %d AS T4 ON T3.ID_OBJECT=T4.ID_OBJECT",
					db.m_szYear,
					month,
					db.m_szEventName,
					ReturnEventsString()
					);
					
					db.fQueryRead(str,crq);
					for(int i=0;i<crq.m_lRecCount;i++)
					{
						str.Format(L"%d",i+1);
						refListObj.InsertItem(i,str,0);
						refListObj.SetItemText(i,1,crq.m_RetValues[i][0]);
						refListObj.SetItemText(i,2,crq.m_RetValues[i][1]);
						refListObj.SetItemText(i,3,crq.m_RetValues[i][2]);
						refListObj.SetItemText(i,4,crq.m_RetValues[i][3]);
						refListObj.SetItemText(i,5,crq.m_RetValues[i][4]);
						refListObj.SetItemText(i,6,crq.m_RetValues[i][5]);
					}

					//деструктор для crq
					for (int i = 0; i < crq.m_lRecCount; i++)	
					{
						delete [] crq.m_RetValues[i];
					}
					if(crq.m_lRecCount>0)
					delete [] crq.m_RetValues;
	return 0;
}

int CData::defineMonth(void)
{
	int m=0;
	
	if(StrCmp(db.m_szMonth,L"ЯНВАРЬ")==0)
	{
		m = 1;
	}
	else
	if(StrCmp(db.m_szMonth,L"ФЕВРАЛЬ")==0)
	{
		m = 2;
	}
	else
	if(StrCmp(db.m_szMonth,L"МАРТ")==0)
	{
		m = 3;
	}
	else
	if(StrCmp(db.m_szMonth,L"АПРЕЛЬ")==0)
	{
		m = 4;
	}
	else
	if(StrCmp(db.m_szMonth,L"МАЙ")==0)
	{
		m = 5;
	}
	else
	if(StrCmp(db.m_szMonth,L"ИЮНЬ")==0)
	{
		m = 6;
	}
	else
	if(StrCmp(db.m_szMonth,L"ИЮЛЬ")==0)
	{
		m = 7;
	}
	else
	if(StrCmp(db.m_szMonth,L"АВГУСТ")==0)
	{
		m = 8;
	}
	else
	if(StrCmp(db.m_szMonth,L"СЕНТЯБРЬ")==0)
	{
		m = 9;
	}
	else
	if(StrCmp(db.m_szMonth,L"ОКТЯБРЬ")==0)
	{
		m = 10;
	}
	else
	if(StrCmp(db.m_szMonth,L"НОЯБРЬ")==0)
	{
		m = 11;
	}
	else
	if(StrCmp(db.m_szMonth,L"ДЕКАБРЬ")==0)
	{
		m = 12;
	}
	return m;
}


bool CData::insertobjects(CString szObj)
{
	CString str;
	CRetQuery crq;
	CString id_plan_info;
	CString max_id_obj_plan;
	int id_event;
	int quart = defineQuart();
	int rsrce = defineResourceType();
	int month = defineMonth();
	int iDirect = (StrCmp(db.m_szGenAdd,L"Основной")==0)?0:1; //0 - основной, 1 - доп.

#ifdef CONVERTING_ALIAS
	str.Format(L"%s%s%s%d%s%s%s",L"SELECT ID_PLAN_INFO FROM PLAN_INFO WHERE NYEAR=",db.m_szYear,L" AND NMONTH=",month,L" AND EVENT='",Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"'");
#else
	str.Format(L"%s%s%s%d%s%s%s",L"SELECT ID_PLAN_INFO FROM PLAN_INFO WHERE NYEAR=",db.m_szYear,L" AND NMONTH=",month,L" AND EVENT='",db.m_szEventName,L"'");
#endif
	db.fQueryRead(str,crq);

	id_plan_info=crq.m_RetValues[0][0];
	str.Format(L"%s%s%s%s%s",L"INSERT INTO OBJECTS_PLANS(OBJECT_NAME,ID_PLAN_INFO) VALUES ('",szObj,L"',",id_plan_info,L")");
	
	db.ExeSql(str);
	str.Format(L"%s",L"SELECT MAX(ID_OBJECT) FROM OBJECTS_PLANS");
	
	db.fQueryRead(str,crq);
	max_id_obj_plan=crq.m_RetValues[0][0];
	
#ifdef CONVERTING_ALIAS
	str.Format(L"SELECT ID_EVENT FROM EVENTS WHERE EVENT_NAME='%s'",Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
	str.Format(L"SELECT ID_EVENT FROM EVENTS WHERE EVENT_NAME='%s'",db.m_szEventName);
#endif

	db.fQueryRead(str,crq);
	id_event=_wtoi(crq.m_RetValues[0][0]);

	switch(id_event)
	{
	case 8:
		str.Format(
			L"INSERT INTO 8(ID_OBJECT,Q,NOM,NFACTBEF,NFACTAFTER,KEC,PRICE,COSTS,RESULT,ECONOMY,CURTIME) "
			L"VALUES (%s,0,0,0,0,0,0,0,0,0,0)",max_id_obj_plan);
		db.ExeSql(str);
		break;
	default:
		AfxMessageBox(L"Error: Event 0 not supported!!!");
		break;
	}

	//деструктор для crq
					for (int i = 0; i < crq.m_lRecCount; i++)	
					{
						delete [] crq.m_RetValues[i];
					}
					if(crq.m_lRecCount>0)
					delete [] crq.m_RetValues;

	return false;
}

/*
bool CData::opendialog(CString& eventname)
{
	CString str;
	CRetQuery crq;
	CDialog* dlg;
	int id_event;
	str.Format(L"SELECT ID_EVENT FROM EVENTS WHERE EVENT_NAME='%s'",db.m_szEventName);
	db.fQueryRead(str,crq);
	id_event=_wtoi(crq.m_RetValues[0][0]);
	switch(id_event)
	{
	case 8:
		dlg=new EvRem;
		dlg->DoModal();
		break;
	default:
		AfxMessageBox(L"Error: На данный момент данный подходящий диалог отсутствует");
		break;
	}
	return false;
}
*/
bool CData::AddtoEventsValues(double costs, double saveds,double time, double economy)
{
	CString szBuffer;
	CRetQuery crq;
	double fBuffer[4];
	int month = db.defineMonth();

#ifdef CONVERTING_ALIAS
	szBuffer.Format(L"SELECT TOTALPLANCOSTS,TOTALPLANSAVEDS,TOTALPLANTIME,TOTALPLANECONOMY FROM EVENTS WHERE EVENT_NAME='%s'",Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
	szBuffer.Format(L"SELECT TOTALPLANCOSTS,TOTALPLANSAVEDS,TOTALPLANTIME,TOTALPLANECONOMY FROM EVENTS WHERE EVENT_NAME='%s'",db.m_szEventName);
#endif
	db.fQueryRead(szBuffer,crq);
	fBuffer[0]=_wtof(crq.m_RetValues[0][0]);
	fBuffer[1]=_wtof(crq.m_RetValues[0][1]);
	fBuffer[2]=_wtof(crq.m_RetValues[0][2]);
	fBuffer[3]=_wtof(crq.m_RetValues[0][3]);
	
	fBuffer[0]+=costs;
	fBuffer[1]+=saveds;
	fBuffer[2]+=time;
	fBuffer[3]+=economy;
	szBuffer.Format(
		L"UPDATE EVENTS "
		L"SET TOTALPLANCOSTS=%f, "
		L"TOTALPLANSAVEDS=%f, "
		L"TOTALPLANTIME=%f, "
		L"TOTALPLANECONOMY=%f "
		L"WHERE "
		L" EVENT_NAME='%s'",
		fBuffer[0],
		fBuffer[1],
		fBuffer[2],
		fBuffer[3],
		db.m_szEventName
		);
	
	db.ExeSql(szBuffer);

	szBuffer.Format(
	L"UPDATE PLAN_INFO AS T1 INNER JOIN OBJECTS_PLANS AS T2 " 
	L"ON ((T1.ID_PLAN_INFO=T2.ID_PLAN_INFO) AND (T1.NYEAR=%s) AND (T1.NMONTH=%d) AND (T2.OBJECT_NAME='%s')) " 
	L"SET T1.SAVEDS=%f,T1.ECONOMY=%f,T1.CURCOSTS=%f,T1.CURTIME=%f",db.m_szYear,month,db.m_szCurObject,fBuffer[1],fBuffer[3],fBuffer[0],fBuffer[2]);

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

bool CData::SubstractEventsInfo(double costs, double saveds,double time, double economy)
{
	CString szBuffer;
	CRetQuery crq;
	double fBuffer[4];
	int month = db.defineMonth();

#ifdef CONVERTING_ALIAS
	szBuffer.Format(L"SELECT TOTALPLANCOSTS,TOTALPLANSAVEDS,TOTALPLANTIME,TOTALPLANECONOMY FROM EVENTS WHERE EVENT_NAME='%s'",Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
	szBuffer.Format(L"SELECT TOTALPLANCOSTS,TOTALPLANSAVEDS,TOTALPLANTIME,TOTALPLANECONOMY FROM EVENTS WHERE EVENT_NAME='%s'",db.m_szEventName);
#endif
	db.fQueryRead(szBuffer,crq);
	fBuffer[0]=_wtof(crq.m_RetValues[0][0]);
	fBuffer[1]=_wtof(crq.m_RetValues[0][1]);
	fBuffer[2]=_wtof(crq.m_RetValues[0][2]);
	fBuffer[3]=_wtof(crq.m_RetValues[0][3]);
	
	fBuffer[0]-=costs;
	fBuffer[1]-=saveds;
	fBuffer[2]-=time;
	fBuffer[3]-=economy;
	szBuffer.Format(
		L"UPDATE EVENTS "
		L"SET TOTALPLANCOSTS=%f, "
		L"TOTALPLANSAVEDS=%f, "
		L"TOTALPLANTIME=%f, "
		L"TOTALPLANECONOMY=%f "
		L"WHERE "
		L" EVENT_NAME='%s'",
		fBuffer[0],
		fBuffer[1],
		fBuffer[2],
		fBuffer[3],
		db.m_szEventName
		);
	db.ExeSql(szBuffer);
	
	szBuffer.Format(
	L"UPDATE PLAN_INFO AS T1 INNER JOIN OBJECTS_PLANS AS T2 " 
	L"ON ((T1.ID_PLAN_INFO=T2.ID_PLAN_INFO) AND (T1.NYEAR=%s) AND (T1.NMONTH=%d) AND (T2.OBJECT_NAME='%s')) " 
	L"SET T1.SAVEDS=%f,T1.ECONOMY=%f,T1.CURCOSTS=%f,T1.CURTIME=%f",db.m_szYear,month,db.m_szCurObject,fBuffer[1],fBuffer[3],fBuffer[0],fBuffer[2]);
	
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


bool CData::UpdateTablesPlan(double& costs, double& saveds, double& time, double& economy, CString szColCosts, CString szColSaveds, CString szColTime, CString szColEconomy, CString TName, CString& Event_Name, CString Op)
{
	
	CString szBuffer;
	CRetQuery crq;
	double fBuffer[4];

#ifdef CONVERTING_ALIAS
	szBuffer.Format(L"SELECT %s,%s,%s,%s FROM %s WHERE EVENT_NAME='%s'",szColCosts,szColSaveds,szColTime,szColEconomy,TName,Aliases(Event_Name,ALIAS_TO_NAMES));
#else
	szBuffer.Format(L"SELECT %s,%s,%s,%s FROM %s WHERE EVENT_NAME='%s'",szColCosts,szColSaveds,szColTime,szColEconomy,TName,Event_Name);
#endif

	db.fQueryRead(szBuffer,crq);
	
	fBuffer[0]=_wtof(crq.m_RetValues[0][0]);
	fBuffer[1]=_wtof(crq.m_RetValues[0][1]);
	fBuffer[2]=_wtof(crq.m_RetValues[0][2]);
	fBuffer[3]=_wtof(crq.m_RetValues[0][3]);
	
	if(StrCmp(Op,L"+")==0)
	{
		fBuffer[0]+=costs;
		fBuffer[1]+=saveds;
		fBuffer[2]+=time;
		fBuffer[3]+=economy;
	}
	else
		if(StrCmp(Op,L"-")==0)
		{
			fBuffer[0]-=costs;
			fBuffer[1]-=saveds;
			fBuffer[2]-=time;
			fBuffer[3]-=economy;
		}
	
	szBuffer.Format(
		L"UPDATE EVENTS "
		L"SET TOTALPLANCOSTS=%f, "
		L"TOTALPLANSAVEDS=%f, "
		L"TOTALPLANTIME=%f, "
		L"TOTALPLANECONOMY=%f "
		L"WHERE "
		L" EVENT_NAME='%s'",
		fBuffer[0],
		fBuffer[1],
		fBuffer[2],
		fBuffer[3],
		db.m_szEventName
		);
	
	//db.ExeSql(szBuffer);

	//деструктор для crq
					for (int i = 0; i < crq.m_lRecCount; i++)	
					{
						delete [] crq.m_RetValues[i];
					}
					if(crq.m_lRecCount>0)
					delete [] crq.m_RetValues;

	return false;
}

//Функция для определения имени таблицы мероприятий для выгрузки
int CData::ReturnEventsString(void)
{
#ifdef CONVERTING_ALIAS
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Улучшение технического состояния ГПА за счет ремонта")==0) return 8;
#else
	if(StrCmp(db.m_szEventName,L"Улучшение технического состояния ГПА за счет ремонта")==0) return 8;
#endif
	return 0;
}

int CData::ReturnEventsString(CString& szStr)
{
#ifdef CONVERTING_ALIAS
	if(StrCmp(szStr,L"Улучшение технического состояния ГПА за счет ремонта")==0) return 8;
#else
	if(StrCmp(szStr,L"Улучшение технического состояния ГПА за счет ремонта")==0) return 8;
#endif
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////ИЗМЕНИТЬ ПРИ ДОБАВЛЕНИИ МЕРОПРИЯТИЯ

int CData::defineEventId(void)
{
#ifdef CONVERTING_ALIAS
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Химическая промывка трубных пучков утилизационных теплообменников ГПА"														   )==0) return 100;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Оптимизация режимов работы компрессорных станций и газопроводов"															   )==0) return 1;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Выработка газа потребителям перед выполнением ремонтных работ"																   )==0) return 2;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Выработка (оттягивание) газа ГПА КС из ремонтируемых участков газопроводов"												   )==0) return 4;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Очистка (промывка) проточной части осевых компрессоров"																	   )==0) return 7;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Ремонт газоперекачивающих агрегатов"																						   )==0) return 8;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Применение безрасходной системы продувки пылеуловителей ГРС"																   )==0) return 12;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Оптимизация количества и времени продувок пылеуловителей"																	   )==0) return 15;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Режимно-наладочные испытания котельных"																					   )==0) return 17;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Перевод освещения промышленных и бытовых помещений на осветительное оборудование с использованием светодиодов"   			   )==0) return 20;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Оптимизация схемы электроснабжения путем вывода в резерв ТП и отключения силовых трансформаторов, работающих на холостом ходу")==0) return 23;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Оптимизация режимов работы вентиляторов АВО газа и АВО масла"																   )==0) return 25;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Очистка поверхностей теплообмена трубных пучков АВО газа и АВО масла"         												   )==0) return 26;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Использование устройств плавного пуска и частотно регулируемого электропривода АВО масла, АВО газа, насосов водоснабжения 2"  )==0) return 261;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Использование устройств плавного пуска и частотно регулируемого электропривода АВО масла, АВО газа, насосов водоснабжения"	   )==0) return 27;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Установка конденсаторных батарей с автоматическим управлением"																   )==0) return 38;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Проведение тепловизионного обследования зданий и сооружений на выявление теплопотерь с их последующим устранением"			   )==0) return 42;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Использование утилизаторов тепла работающих газоперекачивающих агрегатов"													   )==0) return 43;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Регулирование температуры в тепловой сети отопления в зависимости от наружной температуры"									   )==0) return 48;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Регулирование температуры в тепловой сети отопления в зависимости от наружной температуры1"								   )==0) return 481;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Использование утилизаторов тепла работающих газоперекачивающих агрегатов1"													   )==0) return 431;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Выработка (оттягивание) газа мобильными компрессорными установками из ремонтируемых участков газопроводов"					   )==0) return 101;
	if(StrCmp(Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Ремонт газоперекачивающих агрегатов1"																						   )==0) return 81;
#else
	if(StrCmp(db.m_szEventName,L"Химическая промывка трубных пучков утилизационных теплообменников ГПА"														   )==0) return 100;
	if(StrCmp(db.m_szEventName,L"Оптимизация режимов работы компрессорных станций и газопроводов"															   )==0) return 1;
	if(StrCmp(db.m_szEventName,L"Выработка газа потребителям перед выполнением ремонтных работ"																   )==0) return 2;
	if(StrCmp(db.m_szEventName,L"Выработка (оттягивание) газа ГПА КС из ремонтируемых участков газопроводов"												   )==0) return 4;
	if(StrCmp(db.m_szEventName,L"Очистка (промывка) проточной части осевых компрессоров"																	   )==0) return 7;
	if(StrCmp(db.m_szEventName,L"Ремонт газоперекачивающих агрегатов"																						   )==0) return 8;
	if(StrCmp(db.m_szEventName,L"Применение безрасходной системы продувки пылеуловителей ГРС"																   )==0) return 12;
	if(StrCmp(db.m_szEventName,L"Оптимизация количества и времени продувок пылеуловителей"																	   )==0) return 15;
	if(StrCmp(db.m_szEventName,L"Режимно-наладочные испытания котельных"																					   )==0) return 17;
	if(StrCmp(db.m_szEventName,L"Перевод освещения промышленных и бытовых помещений на осветительное оборудование с использованием светодиодов"   			   )==0) return 20;
	if(StrCmp(db.m_szEventName,L"Оптимизация схемы электроснабжения путем вывода в резерв ТП и отключения силовых трансформаторов, работающих на холостом ходу")==0) return 23;
	if(StrCmp(db.m_szEventName,L"Оптимизация режимов работы вентиляторов АВО газа и АВО масла"																   )==0) return 25;
	if(StrCmp(db.m_szEventName,L"Очистка поверхностей теплообмена трубных пучков АВО газа и АВО масла"         												   )==0) return 26;
	if(StrCmp(db.m_szEventName,L"Использование устройств плавного пуска и частотно регулируемого электропривода АВО масла, АВО газа, насосов водоснабжения 2"  )==0) return 261;
	if(StrCmp(db.m_szEventName,L"Использование устройств плавного пуска и частотно регулируемого электропривода АВО масла, АВО газа, насосов водоснабжения"	   )==0) return 27;
	if(StrCmp(db.m_szEventName,L"Установка конденсаторных батарей с автоматическим управлением"																   )==0) return 38;
	if(StrCmp(db.m_szEventName,L"Проведение тепловизионного обследования зданий и сооружений на выявление теплопотерь с их последующим устранением"			   )==0) return 42;
	if(StrCmp(db.m_szEventName,L"Использование утилизаторов тепла работающих газоперекачивающих агрегатов"													   )==0) return 43;
	if(StrCmp(db.m_szEventName,L"Регулирование температуры в тепловой сети отопления в зависимости от наружной температуры"									   )==0) return 48;
	if(StrCmp(db.m_szEventName,L"Регулирование температуры в тепловой сети отопления в зависимости от наружной температуры1"								   )==0) return 481;
	if(StrCmp(db.m_szEventName,L"Использование утилизаторов тепла работающих газоперекачивающих агрегатов1"													   )==0) return 431;
	if(StrCmp(db.m_szEventName,L"Выработка (оттягивание) газа мобильными компрессорными установками из ремонтируемых участков газопроводов"					   )==0) return 101;
	if(StrCmp(db.m_szEventName,L"Ремонт газоперекачивающих агрегатов1"																						   )==0) return 81;
#endif
	return 0;
}

int CData::defineEventId(CString str)
{
	if(StrCmp(str,L"Химическая промывка трубных пучков утилизационных теплообменников ГПА"													      )==0) return 100;
	if(StrCmp(str,L"Оптимизация режимов работы компрессорных станций и газопроводов"															  )==0) return 1;
	if(StrCmp(str,L"Выработка газа потребителям перед выполнением ремонтных работ"																  )==0) return 2;
	if(StrCmp(str,L"Выработка (оттягивание) газа ГПА КС из ремонтируемых участков газопроводов"													  )==0) return 4;
	if(StrCmp(str,L"Очистка (промывка) проточной части осевых компрессоров"																		  )==0) return 7;
	if(StrCmp(str,L"Ремонт газоперекачивающих агрегатов"																					      )==0) return 8;
	if(StrCmp(str,L"Применение безрасходной системы продувки пылеуловителей ГРС"																  )==0) return 12;
	if(StrCmp(str,L"Оптимизация количества и времени продувок пылеуловителей"																	  )==0) return 15;
	if(StrCmp(str,L"Режимно-наладочные испытания котельных"																						  )==0) return 17;
	if(StrCmp(str,L"Перевод освещения промышленных и бытовых помещений на осветительное оборудование с использованием светодиодов"   			  )==0) return 20;
	if(StrCmp(str,L"Оптимизация схемы электроснабжения путем вывода в резерв ТП и отключения силовых трансформаторов, работающих на холостом ходу")==0) return 23;
	if(StrCmp(str,L"Оптимизация режимов работы вентиляторов АВО газа и АВО масла"																  )==0) return 25;
	if(StrCmp(str,L"Очистка поверхностей теплообмена трубных пучков АВО газа и АВО масла"														  )==0) return 26;
	if(StrCmp(str,L"Использование устройств плавного пуска и частотно регулируемого электропривода АВО масла, АВО газа, насосов водоснабжения2"	  )==0) return 261;
	if(StrCmp(str,L"Использование устройств плавного пуска и частотно регулируемого электропривода АВО масла, АВО газа, насосов водоснабжения"	  )==0) return 27;
	if(StrCmp(str,L"Установка конденсаторных батарей с автоматическим управлением"																  )==0) return 38;
	if(StrCmp(str,L"Проведение тепловизионного обследования зданий и сооружений на выявление теплопотерь с их последующим устранением"			  )==0) return 42;
	if(StrCmp(str,L"Использование утилизаторов тепла работающих газоперекачивающих агрегатов"													  )==0) return 43;
	if(StrCmp(str,L"Регулирование температуры в тепловой сети отопления в зависимости от наружной температуры"									  )==0) return 48;
	if(StrCmp(str,L"Регулирование температуры в тепловой сети отопления в зависимости от наружной температуры1"									  )==0) return 481;
	if(StrCmp(str,L"Использование утилизаторов тепла работающих газоперекачивающих агрегатов1"													  )==0) return 431;
	if(StrCmp(str,L"Выработка (оттягивание) газа мобильными компрессорными установками из ремонтируемых участков газопроводов"					  )==0) return 101;
	if(StrCmp(str,L"Ремонт газоперекачивающих агрегатов1"																					      )==0) return 81;
	return 0;
}


CString CData::GetCurrentDirectoryX(void)
{
	char info1[MAX_PATH];
	CString szBuffer;
	CString szPath=L"";
	int i=0;

	GetModuleFileNameA(NULL,info1,sizeof(info1));
	szBuffer.Format(_T("%S"),info1);

	CString sToken=szBuffer.Tokenize(L"\\",i);
	szPath=sToken;

	sToken=szBuffer.Tokenize(L"\\",i);
	while(!sToken.IsEmpty())
	{
		if(sToken.Find(L".exe")==-1)
			szPath=szPath+"\\"+sToken;
			//szPath.Format(L"%s*%s",szPath,sToken);

		sToken=szBuffer.Tokenize(L"\\",i);
	}
	szPath=szPath+L"\\";

	return szPath;
}


int CData::Aliases(CRetQuery& crq, int ncol, int direct)
{
	CRetQuery crq_alias;
	
	CString szBuffer;
	
	szBuffer.Format(L"SELECT T1.ALIAS, T2.EVENT_NAME FROM ALIASES AS T1 INNER JOIN EVENTS AS T2 ON T1.ID_EVENT=T2.ID_EVENT WHERE T1.NYEAR=%s",this->m_szYear);
	
	this->fQueryRead(szBuffer,crq_alias);

	for(int i=0;i<crq.m_lRecCount;i++)
	{
		for(int b=0;b<crq_alias.m_lRecCount;b++)
		{
			if(crq.m_RetValues[i][ncol].Compare(crq_alias.m_RetValues[b][1])==0)
			{
				crq.m_RetValues[i][ncol]=crq_alias.m_RetValues[b][(direct==NAMES_TO_ALIAS)?0:1];
			}
		}
	}

	return 0;
}


CString CData::Aliases(CString& szEvent,int direct)
{
	CRetQuery crq;

	CString szBuffer;
	
	szBuffer.Format(L"SELECT T1.ALIAS, T2.EVENT_NAME FROM ALIASES AS T1 INNER JOIN EVENTS AS T2 ON T1.ID_EVENT=T2.ID_EVENT WHERE T1.NYEAR=%s",this->m_szYear);
	
	this->fQueryRead(szBuffer,crq);

	for(int b=0;b<crq.m_lRecCount;b++)
	{
		if(crq.m_RetValues[b][(direct==NAMES_TO_ALIAS)?1:0].Compare(szEvent)==0)
		{
			return crq.m_RetValues[b][(direct==NAMES_TO_ALIAS)?0:1];
		}		
	}

	return szEvent;
}

CString CData::Aliases(const CString& szEvent,int direct)
{
	CRetQuery crq;

	CString szBuffer;
	
	szBuffer.Format(L"SELECT T1.ALIAS, T2.EVENT_NAME FROM ALIASES AS T1 INNER JOIN EVENTS AS T2 ON T1.ID_EVENT=T2.ID_EVENT WHERE T1.NYEAR=%s",this->m_szYear);
	
	this->fQueryRead(szBuffer,crq);

	for(int b=0;b<crq.m_lRecCount;b++)
	{
		if(crq.m_RetValues[b][(direct==NAMES_TO_ALIAS)?1:0].Compare(szEvent)==0)
		{
			return crq.m_RetValues[b][(direct==NAMES_TO_ALIAS)?0:1];
		}		
	}

	return szEvent;
}
