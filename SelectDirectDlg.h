#pragma once


// диалоговое окно CSelectDirectDlg

class CSelectDirectDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectDirectDlg)

public:
	CSelectDirectDlg(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CSelectDirectDlg();

// Данные диалогового окна
	enum { IDD = IDD_SELECT_DIRECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	BOOL radiobut_Plan;
};
