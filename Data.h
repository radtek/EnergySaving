#pragma once
#include <odbcinst.h>
#include <afxdb.h>
#include <vector>
#include <afxtempl.h>

#define INIT 0
#define PLAN 1
#define FACT 2
#define ALIAS_TO_NAMES 3
#define NAMES_TO_ALIAS 4

#define CONVERTING_ALIAS//ДЛЯ РАЗРЕШЕНИЯ КОНВЕРТАЦИИ


struct CRetQuery
{
	CString** m_RetValues;
	long m_lRecCount;
	long m_lColCount;
};

class CData
{
public:
	bool FillListView(CListCtrl& /*Лист контрол из класса представлений*/); //обработка запросов и т.д.
	bool FillGenTreeCtrl(CTreeCtrl&);
	CData(void);
	~CData(void);
	CDatabase database;
	CString m_sSqlString;
	CString sDriver;
	CString sDsn;
	CString m_sFile;
	CString m_szYear;
	CString m_szQuart;
	CString m_szMonth;
	CString m_szEventName;//Наименование, выделенного мероприятия, для структуры отчетных данных (CClassView)
	CString m_szTypeResource;//Тип ресурсов (природный газ, электроэнергия, ....)
	CString m_szGenAdd;
	CString m_szCurObject;
	CString m_szPlace;
	int m_iStatus;
	bool m_bReconfiguredPlanGTU;
	CString m_szReconfigEvent;

	bool m_bDbOk;
	bool fQueryRead(CString /*запрос*/, CRetQuery&);
	void ExeSql(CString);
	
	bool (CData::*pF1)(CListCtrl&);
	bool (CData::*pF2)(CListCtrl&);
	bool (CData::*pF3)(CListCtrl&);

	bool procElemreports(CListCtrl&);

	int defineQuart(void);
	int defineResourceType(void);
	int defineMesuar();
	//Выбран год
	bool q00(CListCtrl&);
	bool q01(CListCtrl&);
	bool q02(CListCtrl&);
	//Выбран квартал
	bool q10(CListCtrl&);
	bool q11(CListCtrl&);
	bool q12(CListCtrl&);

	int m_strDirectCalc;// план, факт (PLAN, FACT)

	bool q20(CListCtrl& ctrl);
	bool q21(CListCtrl& ctrl);
	bool q22(CListCtrl& ctrl);
	int defineMonth(void);
	bool insertobjects(CString szObj);
	bool opendialog(CString& eventname);
	bool AddtoEventsValues(double,double,double,double);
	bool SubstractEventsInfo(double,double,double,double);
	bool UpdateTablesPlan(double& costs, double& saveds, double& time, double& economy, CString szColCosts, CString szColSaveds, CString szColTime, CString szColEconomy, CString TName, CString& Event_Name, CString Op);
	int ReturnEventsString(void);
	int ReturnEventsString(CString&);
	int defineEventId(void);
	int defineEventId(CString);
	CString GetCurrentDirectoryX(void);
	int Aliases(CRetQuery&,int /*элемент по вертикали*/, int /*режим*/);
	CString Aliases(CString& szEvent,int);
	CString Aliases(const CString& szEvent,int);
};




