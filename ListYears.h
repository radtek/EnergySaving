#pragma once


// ���������� ���� CListYears

class CListYears : public CDialogEx
{
	DECLARE_DYNAMIC(CListYears)

public:
	CListYears(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CListYears();

// ������ ����������� ����
	enum { IDD = IDD_EDIT_YEARS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
};
