#pragma once


// ���������� ���� DlgEditResIndexes
#include "ListCtrlObj.h"
#include "Data.h"

extern CData db;

class DlgEditResIndexes : public CDialogEx
{
	DECLARE_DYNAMIC(DlgEditResIndexes)

public:
	CListCtrlObj* m_list1;
	DlgEditResIndexes(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~DlgEditResIndexes();

// ������ ����������� ����
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	bool UpdateListCntr(void);
};
