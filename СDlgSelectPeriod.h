#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// ���������� ���� �DlgSelectPeriod

class �DlgSelectPeriod : public CDialog
{
	DECLARE_DYNAMIC(�DlgSelectPeriod)

public:
	�DlgSelectPeriod(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~�DlgSelectPeriod();

// ������ ����������� ����
	enum { IDD = IDD_SELECT_PERIOD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl tree;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTreeButViewedit();
	afx_msg void OnAddperiodPlan();
	void UpdateTree(void);

};
