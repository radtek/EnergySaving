#pragma once
#include "afxwin.h"
#include "ListYears.h"
#include "afxcmn.h"
#include "DlgEditsYearsAdd.h"
#include "Data.h"
// диалоговое окно DlgEditYears

class DlgEditYears : public CDialogEx
{
	DECLARE_DYNAMIC(DlgEditYears)

public:
	DlgEditYears(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~DlgEditYears();

// Данные диалогового окна
	enum { IDD = IDD_EDIT_YEARS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_list;
	afx_msg void OnBnClickedOk();
	bool UpdateListView(void);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonDelete();
	bool CleaningTables(int, CString);
	bool ExecutionQueryForCleaningTablesCalcs(int N, CString);
	bool CleaningObjectsPlans(CString m_szYear);
	bool CleaningPlaceOfUsePlan(CString m_szYear);
	bool CleningInfoTables(CString m_szYear);
	bool CleaningPEtable(CString m_szYear);
	bool CleaningYearsTable(CString szYear);
};
