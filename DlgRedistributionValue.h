#pragma once
#include "afxwin.h"
#include "EditRedistr.h"


// диалоговое окно CDlgRedistributionValue

class CDlgRedistributionValue : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRedistributionValue)

public:
	CDlgRedistributionValue(CWnd* pParent = NULL);   // стандартный конструктор
	CDlgRedistributionValue(CString szValue, CString, CWnd* pParent = NULL);   // стандартный конструктор
	CDlgRedistributionValue(int dDirect,CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CDlgRedistributionValue();
	CString m_szValue;
	CString m_szValue1;
	CEditRedistr* pEdit;
// ƒанные диалогового окна
	enum { IDD = IDD_DIALOGREDISTRIBUTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedRemain();
	afx_msg void OnBnClickedOk();
	
	void CreateEditConrol(void);
};
