#pragma once
#include "afxcmn.h"


// диалоговое окно CDlgAddEvent2

class CDlgAddEvent2 : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddEvent2)

public:
	CDlgAddEvent2(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CDlgAddEvent2();

// Данные диалогового окна
	enum { IDD = IDD_DIALOG_ADDEVENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list1;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
