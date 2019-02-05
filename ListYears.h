#pragma once


// диалоговое окно CListYears

class CListYears : public CDialogEx
{
	DECLARE_DYNAMIC(CListYears)

public:
	CListYears(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CListYears();

// Данные диалогового окна
	enum { IDD = IDD_EDIT_YEARS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
};
