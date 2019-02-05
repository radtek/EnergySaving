#pragma once
#include "afxwin.h"


// ���������� ���� CDlgAddNewObject

class CDlgAddNewObject : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddNewObject)

public:
	CDlgAddNewObject(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CDlgAddNewObject();

// ������ ����������� ����
	enum { IDD = IDD_DIALOG_ADDNEWOBJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit1;
	afx_msg void OnBnClickedOk();
	bool UpdateList(void);
	afx_msg void OnBnClickedCancel();
	int defineNumberEvent(void);
	void AddToEvData(CString ID_OBJECT, CString MAXIDPLANINFO);
	void AddToEvData(CString ID_OBJECT, CString MAXIDPLANINFO, CString MAXIDFACTINFO);
};
