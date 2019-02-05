#pragma once
#include "afxcmn.h"
#include "ListCtrlObj.h"

// диалоговое окно CDlgCalcRes

#define WM_UPDATERESULT WM_USER+200

class CDlgCalcRes : public CDialog
{
	DECLARE_DYNAMIC(CDlgCalcRes)

public:
	CDlgCalcRes(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CDlgCalcRes();

// Данные диалогового окна
	enum { IDD = IDD_DIALOG_CALCRESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list2;
	CString m_szNameWindow;
	CString m_szPeriod;
	CString m_szObject;
	int m_inumEvent;
	CListCtrlObj* m_list1;
	bool SetStaticText(void);
	bool SetListCtrls(void);
	bool SetItemsList1(void);
	bool SetItemsList2(void);
	int cx;
	int cy;
	virtual BOOL OnInitDialog();
	int EventMatchFunction(void);
	void CalcResult(void);
	//CDlgCalcRes* pDlgCalcRes;
protected:
	afx_msg LRESULT OnUpdateresult(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedOk();
	bool e1Rec(void);
	bool e7Rec(void);
	void e1(void);
	void e7(void);
	
	bool e8(void);
	bool e8Rec(void);
	bool e12(void);
	bool e12Rec(void);
	bool e15(void);
	bool e15Rec(void);
	bool e25(void);
	bool e25Rec(void);
	bool e27(void);
	bool e27Rec(void);
	bool e38(void);
	bool e38Rec(void);
	bool e42(void);
	bool e42Rec(void);
	bool e48(void);
	bool e48Rec(void);
	bool e481(void);
	bool e481Rec(void);
	bool e431(void);
	bool e431Rec(void);
	bool GetOldParamForTotalValues(void);
	double m_dblTCOST;
	double m_dblTSAVEDS;
	double m_dblTTIME;
	double m_dblTECONOMY;
	bool e2(void);
	bool e2Rec(void);
	bool e4(void);
	bool e4Rec(void);
	bool e17(void);
	bool e17Rec(void);
	bool e26(void);
	bool e26Rec(void);
	bool e20(void);
	bool e20Rec(void);
	bool e23(void);
	bool e23Rec(void);
	bool e43(void);
	bool e43Rec(void);
	bool e100(void);
	bool e100Rec(void);
	bool e101(void);
	bool e101Rec(void);
	bool e81(void);
	bool e81Rec(void);
};
