#pragma once
#include "afxwin.h"


// диалоговое окно CDlgAddEvents

class DlgAddEvents : public CDialog
{
	DECLARE_DYNAMIC(DlgAddEvents)

public:
	DlgAddEvents(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~DlgAddEvents();

// Данные диалогового окна
	enum { IDD = IDD_ADDEVENTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTabCtrl tab1;
	CButton ButtonOk;
	CButton bCancel;
	CTabCtrl tab2;
	CListCtrl listctrl1;
	CArray <CString,CString> arrNaturalGas;
	CArray <CString,CString> arrElecto;
	CArray <CString,CString> arrHeat;
	CArray <CString,CString> arrNaturalGas2;
	CArray <CString,CString> arrElecto2;
	CArray <CString,CString> arrHeat2;
	afx_msg void OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangingTab2(NMHDR *pNMHDR, LRESULT *pResult);
	void SetCheckLc(CListCtrl& reflistctrl, CArray<CString,CString>& refArray);
	void FillArray(CArray<CString,CString>& arr, CListCtrl& reflist);
	afx_msg void OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	//void UpdateList(CArray<CString,CString>&,CArray<CString,CString>&,CRetQuery&,CListCtrl&,CString*);
	bool bHasTransition;
	void InitListsControls(void);
	CString** m_pszAllEvents;
	int* m_iEnableElements;
	int m_iCountElements;
	int* m_piGenAddReport;
	void InitEventsList();
	void SetItemsListControl(void);
	int FindElement(CString&);
	void CalcCheckTrueBoxes();
	afx_msg void OnBnClickedOk();
	CEdit ectrlYear;
};
