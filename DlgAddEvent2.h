#pragma once
#include "afxcmn.h"


// ���������� ���� CDlgAddEvent2

class CDlgAddEvent2 : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddEvent2)

public:
	CDlgAddEvent2(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CDlgAddEvent2();

// ������ ����������� ����
	enum { IDD = IDD_DIALOG_ADDEVENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list1;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
