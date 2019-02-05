// DlgEditYears.cpp: ���� ����������
//

#include "stdafx.h"
#include "MFCApplication30.h"
#include "DlgEditYears.h"
#include "afxdialogex.h"
//#include "Data.h"


// ���������� ���� DlgEditYears

IMPLEMENT_DYNAMIC(DlgEditYears, CDialogEx)

DlgEditYears::DlgEditYears(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgEditYears::IDD, pParent)
{

}

DlgEditYears::~DlgEditYears()
{
}

void DlgEditYears::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(DlgEditYears, CDialogEx)
	ON_BN_CLICKED(IDOK, &DlgEditYears::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &DlgEditYears::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &DlgEditYears::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// ����������� ��������� DlgEditYears
extern CData db;

BOOL DlgEditYears::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CStatic* pStatObj;
	CButton* pBut;

	CRect rectBuffer;
	CRect rectBuffer2;
	CRect wndRect;
	CRetQuery crq;
	// TODO:  �������� �������������� �������������
	this->SetWindowTextW(L"��������� �������");

	int csx = GetSystemMetrics(SM_CXSCREEN);
	int csy = GetSystemMetrics(SM_CYSCREEN);

	int cwx=csx/3;
	int cwy=csy/3;

	this->MoveWindow(csx-(csx/3*2),csy-(csy/3*2),cwx,cwy);

	pStatObj=(CStatic*) this->GetDlgItem(IDC_GROUP_BOX1);

	this->GetWindowRect(wndRect);
	ScreenToClient(wndRect);

	pStatObj->GetWindowRect(rectBuffer);
	ScreenToClient(rectBuffer);

	pStatObj->MoveWindow(wndRect.left+10,wndRect.top+30,wndRect.Width()-25,wndRect.Height()-90);
	pStatObj->GetWindowRect(rectBuffer);
	ScreenToClient(rectBuffer);

	m_list.MoveWindow(rectBuffer.left+5,rectBuffer.top+15,(rectBuffer.right-rectBuffer.left-10),rectBuffer.bottom-rectBuffer.top-20);

	pBut=(CButton*) this->GetDlgItem(IDC_BUTTON_DELETE);
	pBut->GetWindowRect(rectBuffer2);
	ScreenToClient(rectBuffer2);
	pBut->MoveWindow(rectBuffer.right-rectBuffer2.Width(),rectBuffer.bottom+10,rectBuffer2.Width(),rectBuffer2.Height());
	pBut->GetWindowRect(rectBuffer2);
	ScreenToClient(rectBuffer2);


	pBut=(CButton*) this->GetDlgItem(IDOK);
	pBut->GetWindowRect(rectBuffer);
	ScreenToClient(rectBuffer);
	pBut->MoveWindow(rectBuffer2.left-rectBuffer.Width()-10, rectBuffer2.top,rectBuffer.Width(),rectBuffer.Height());
	
	db.fQueryRead(L"SELECT YEAR_NAME FROM YEARS ORDER BY YEAR_NAME DESC",crq);

	if(crq.m_lRecCount==0)
	{
		AfxMessageBox(L"������ �� �������.");

		//������������
		pStatObj=NULL;
		pBut=NULL;

		delete pStatObj;
		delete pBut;

		return TRUE;  // return TRUE unless you set the focus to a control
		// ����������: �������� ������� OCX ������ ���������� �������� FALSE
	}
	
	

	/*m_list.GetWindowRect(rectBuffer);
	ScreenToClient(rectBuffer);
	m_list.InsertColumn(0,L"���",LVCFMT_LEFT,rectBuffer.Width()-5);

	for(int i=0;i<crq.m_lRecCount;i++)
	{
		m_list.InsertItem(0,crq.m_RetValues[i][0]);
	}*/

//	CRetQuery crq;
	db.fQueryRead(L"SELECT YEAR_NAME FROM YEARS ORDER BY YEAR_NAME DESC",crq);
	//CRect rectBuffer;

	m_list.GetWindowRect(rectBuffer);
	ScreenToClient(rectBuffer);
	
	m_list.InsertColumn(0,L"���",LVCFMT_LEFT,rectBuffer.Width()-25);

	UpdateListView();

	DWORD style = GetWindowLong(m_list.m_hWnd,GWL_STYLE);
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, (style&~LVS_TYPEMASK) | LVS_REPORT);
	
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() |
						LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	//������������
	pStatObj=NULL;
	pBut=NULL;

	delete pStatObj;
	delete pBut;

	//���������� ��� crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����������: �������� ������� OCX ������ ���������� �������� FALSE
}


void DlgEditYears::OnBnClickedOk()
{
	// TODO: �������� ���� ��� ����������� �����������
	CDlgEditsYearsAdd dlg;
	dlg.DoModal();
	UpdateListView();
	//CDialogEx::OnOK();
}


bool DlgEditYears::UpdateListView()
{
	CRetQuery crq;

	m_list.DeleteAllItems();

	db.fQueryRead(L"SELECT YEAR_NAME FROM YEARS ORDER BY YEAR_NAME DESC",crq);
	CRect rectBuffer;

	m_list.GetWindowRect(rectBuffer);
	ScreenToClient(rectBuffer);
	
	//m_list.InsertColumn(0,L"���",LVCFMT_LEFT,rectBuffer.Width()-25);

	for(int i=0;i<crq.m_lRecCount;i++)
	{
		m_list.InsertItem(0,crq.m_RetValues[i][0]);
	}

	//���������� ��� crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	return false;
}


void DlgEditYears::OnBnClickedCancel()
{
	// TODO: �������� ���� ��� ����������� �����������
	
	CDialogEx::OnCancel();
}


void DlgEditYears::OnBnClickedButtonDelete()
{
	// TODO: �������� ���� ��� ����������� �����������
	CString szBuffer=L"";
	CString m_szYear=L"";
	CString* m_pszIdPI={0};
	CString* m_pszIdEP={0};
	CString* m_pszIdPlace={0};
	//szBuffer.Format(L"%d",m_list.GetNextItem(-1,LVNI_SELECTED));

	if(m_list.GetNextItem(-1,LVNI_SELECTED)!=-1)
	{
		//1 ������� ������� � ����������� ��� ��������
		CleaningTables(777,m_list.GetItemText(m_list.GetNextItem(-1,LVNI_SELECTED),0));
		////2 ������� Objects_plans
		CleaningObjectsPlans(m_list.GetItemText(m_list.GetNextItem(-1,LVNI_SELECTED),0));
		////3 ������� ������� ������� ���������
		CleaningPlaceOfUsePlan(m_list.GetItemText(m_list.GetNextItem(-1,LVNI_SELECTED),0));
		////4 ������� InfoTables
		CleningInfoTables(m_list.GetItemText(m_list.GetNextItem(-1,LVNI_SELECTED),0));
		////5 ������� Plan_event �������
		CleaningPEtable(m_list.GetItemText(m_list.GetNextItem(-1,LVNI_SELECTED),0));
		//6 ������� YEARS �������
		CleaningYearsTable(m_list.GetItemText(m_list.GetNextItem(-1,LVNI_SELECTED),0));
		UpdateListView();
		AfxMessageBox(L"�������� ���������!!!");
	}
	else
	{
		AfxMessageBox(L"�� ������� �������.");
	}

	delete [] m_pszIdPI;
	delete [] m_pszIdEP;
	delete [] m_pszIdPlace;


}



//������� ������ � ����������� ��� ��������
bool DlgEditYears::CleaningTables(int n, CString szYear)
{
	ExecutionQueryForCleaningTablesCalcs(1, szYear);
	ExecutionQueryForCleaningTablesCalcs(100, szYear);
	ExecutionQueryForCleaningTablesCalcs(12, szYear);
	ExecutionQueryForCleaningTablesCalcs(15, szYear);
	ExecutionQueryForCleaningTablesCalcs(17, szYear);
	ExecutionQueryForCleaningTablesCalcs(2, szYear);
	ExecutionQueryForCleaningTablesCalcs(20, szYear);
	ExecutionQueryForCleaningTablesCalcs(23, szYear);
	ExecutionQueryForCleaningTablesCalcs(25, szYear);
	ExecutionQueryForCleaningTablesCalcs(26, szYear);
	ExecutionQueryForCleaningTablesCalcs(27, szYear);
	ExecutionQueryForCleaningTablesCalcs(38, szYear);
	ExecutionQueryForCleaningTablesCalcs(4, szYear);
	ExecutionQueryForCleaningTablesCalcs(42, szYear);
	ExecutionQueryForCleaningTablesCalcs(43, szYear);
	ExecutionQueryForCleaningTablesCalcs(48, szYear);
	ExecutionQueryForCleaningTablesCalcs(481, szYear);
	ExecutionQueryForCleaningTablesCalcs(7, szYear);
	ExecutionQueryForCleaningTablesCalcs(8, szYear);
	ExecutionQueryForCleaningTablesCalcs(431, szYear);
	ExecutionQueryForCleaningTablesCalcs(101, szYear);
	ExecutionQueryForCleaningTablesCalcs(81, szYear);
	return false;
}
//������� 1 ������� � ���������
//������� ���������: N - ����� ����������� �� �������� �������, szYear - ������� ��� 
bool DlgEditYears::ExecutionQueryForCleaningTablesCalcs(int N, CString szYear)
{
	CString szBuffer=L"";
	CString szN;
	szN.Format(L"%d",N);
	szBuffer.Format(L"DELETE FROM %s WHERE ID_PLAN_INFO IN (SELECT ID_PLAN_INFO FROM PLAN_INFO WHERE NYEAR=%s)",szN,szYear);
	//AfxMessageBox(szBuffer);
	db.ExeSql(szBuffer);
	szN.Format(L"%d%s",N,L"F");
	szBuffer.Format(L"DELETE FROM %s WHERE ID_FACT_INFO IN (SELECT ID_FACT_INFO FROM FACT_INFO WHERE NYEAR=%s)",szN,szYear);
	//AfxMessageBox(szBuffer);
	db.ExeSql(szBuffer);

	return false;
}
//������� ������� Objects_Plans
//������� ���������: m_szYear - ���
bool DlgEditYears::CleaningObjectsPlans(CString m_szYear)
{
	CString szBuffer=L"";
	szBuffer.Format(L"DELETE FROM OBJECTS_PLANS WHERE ID_PLACE IN (SELECT ID_PLACE FROM PLACEOFUSEPLAN WHERE ID_EVENTPLAN IN (SELECT ID_EVENTPLAN FROM PLAN_EVENT WHERE NYEAR=%s))",m_szYear);
	db.ExeSql(szBuffer);
	return false;
}
//������� ������� PLACEOFUSEPLAN
//������� ������: m_szYear - ���
bool DlgEditYears::CleaningPlaceOfUsePlan(CString m_szYear)
{
	CString szBuffer;
	szBuffer.Format(L"DELETE FROM PLACEOFUSEPLAN WHERE ID_EVENTPLAN IN (SELECT ID_EVENTPLAN FROM PLAN_EVENT WHERE NYEAR=%s)",m_szYear);
	db.ExeSql(szBuffer);
	return false;
}
//������� ������ ���� � ���� - ����
//������� ������: m_szYear - ���
bool DlgEditYears::CleningInfoTables(CString m_szYear)
{
	CString szBuffer;
	szBuffer.Format(L"DELETE FROM PLAN_INFO WHERE NYEAR=%s",m_szYear);
	db.ExeSql(szBuffer);
	szBuffer.Format(L"DELETE FROM FACT_INFO WHERE NYEAR=%s",m_szYear);
	db.ExeSql(szBuffer);
	return false;
}
//������� ������� PLAN_eve
//������� ������: m_szYear - ���
bool DlgEditYears::CleaningPEtable(CString m_szYear)
{
	CString szBuffer;
	szBuffer.Format(L"DELETE FROM PLAN_EVENT WHERE NYEAR=%s",m_szYear);
	db.ExeSql(szBuffer);
	return false;
}


bool DlgEditYears::CleaningYearsTable(CString szYear)
{
	CString szBuffer;
	szBuffer.Format(L"DELETE FROM YEARS WHERE YEAR_NAME=%s",szYear);
	db.ExeSql(szBuffer);
	return false;
}
