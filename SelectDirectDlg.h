#pragma once


// ���������� ���� CSelectDirectDlg

class CSelectDirectDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectDirectDlg)

public:
	CSelectDirectDlg(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CSelectDirectDlg();

// ������ ����������� ����
	enum { IDD = IDD_SELECT_DIRECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	BOOL radiobut_Plan;
};
