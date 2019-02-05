#pragma once
#include "afxcmn.h"


// диалоговое окно CDlgObjects

class CDlgObjects : public CDialog
{
	DECLARE_DYNAMIC(CDlgObjects)

public:
	CDlgObjects(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CDlgObjects();

// Данные диалогового окна
	enum { IDD = IDD_DIALOG_OBJECTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	//virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	CListCtrl m_list1;
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddobjectplan();
	bool UpdateList(void);
	afx_msg void OnDelobjectplan();
	int iCurItem;
	int iCurSubItem;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	void DelDataEvents(CString OBJECT);
	int defineNumberEvent(void);
	CString DelCalcs(void);
	afx_msg void OnTakestoaddition();
};
