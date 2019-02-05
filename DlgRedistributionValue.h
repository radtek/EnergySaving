#pragma once
#include "afxwin.h"
#include "EditRedistr.h"


// ���������� ���� CDlgRedistributionValue

class CDlgRedistributionValue : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRedistributionValue)

public:
	CDlgRedistributionValue(CWnd* pParent = NULL);   // ����������� �����������
	CDlgRedistributionValue(CString szValue, CString, CWnd* pParent = NULL);   // ����������� �����������
	CDlgRedistributionValue(int dDirect,CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CDlgRedistributionValue();
	CString m_szValue;
	CString m_szValue1;
	CEditRedistr* pEdit;
// ������ ����������� ����
	enum { IDD = IDD_DIALOGREDISTRIBUTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedRemain();
	afx_msg void OnBnClickedOk();
	
	void CreateEditConrol(void);
};
