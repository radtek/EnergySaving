#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// диалоговое окно СDlgSelectPeriod

class СDlgSelectPeriod : public CDialog
{
	DECLARE_DYNAMIC(СDlgSelectPeriod)

public:
	СDlgSelectPeriod(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~СDlgSelectPeriod();

// Данные диалогового окна
	enum { IDD = IDD_SELECT_PERIOD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl tree;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTreeButViewedit();
	afx_msg void OnAddperiodPlan();
	void UpdateTree(void);

};
