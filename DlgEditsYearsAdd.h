#pragma once
#include "afxwin.h"


// ���������� ���� CDlgEditsYearsAdd

class CDlgEditsYearsAdd : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgEditsYearsAdd)

public:
	CDlgEditsYearsAdd(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CDlgEditsYearsAdd();

// ������ ����������� ����
	enum { IDD = IDD_EDITS_YEARS_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	//CEdit m_edit;
	afx_msg void OnBnClickedOk();
	CEdit m_edit;
};
