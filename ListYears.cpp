// ListYears.cpp: ���� ����������
//

#include "stdafx.h"
#include "MFCApplication30.h"
#include "ListYears.h"
#include "afxdialogex.h"


// ���������� ���� CListYears

IMPLEMENT_DYNAMIC(CListYears, CDialogEx)

CListYears::CListYears(CWnd* pParent /*=NULL*/)
	: CDialogEx(CListYears::IDD, pParent)
{

}

CListYears::~CListYears()
{
}

void CListYears::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CListYears, CDialogEx)
END_MESSAGE_MAP()


// ����������� ��������� CListYears
