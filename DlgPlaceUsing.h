#pragma once
#include "afxwin.h"


// ���������� ���� CDlgPlaceUsing

class CDlgPlaceUsing : public CDialog
{
	DECLARE_DYNAMIC(CDlgPlaceUsing)

public:
	CDlgPlaceUsing(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CDlgPlaceUsing();

// ������ ����������� ����
	enum { IDD = IDD_DIALOG_ADDPLACEUSING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit;
	afx_msg void OnBnClickedOk();
};
