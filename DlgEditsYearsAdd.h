#pragma once
#include "afxwin.h"


// диалоговое окно CDlgEditsYearsAdd

class CDlgEditsYearsAdd : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgEditsYearsAdd)

public:
	CDlgEditsYearsAdd(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CDlgEditsYearsAdd();

// Данные диалогового окна
	enum { IDD = IDD_EDITS_YEARS_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	//CEdit m_edit;
	afx_msg void OnBnClickedOk();
	CEdit m_edit;
};
