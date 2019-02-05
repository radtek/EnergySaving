#pragma once
#include "afxwin.h"


// диалоговое окно CDlgPlaceUsing

class CDlgPlaceUsing : public CDialog
{
	DECLARE_DYNAMIC(CDlgPlaceUsing)

public:
	CDlgPlaceUsing(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CDlgPlaceUsing();

// Данные диалогового окна
	enum { IDD = IDD_DIALOG_ADDPLACEUSING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit;
	afx_msg void OnBnClickedOk();
};
