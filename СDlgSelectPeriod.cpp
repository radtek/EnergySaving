// �DlgSelectPeriod.cpp: ���� ����������
//

#include "stdafx.h"
#include "MFCApplication30.h"
#include "�DlgSelectPeriod.h"
#include "afxdialogex.h"
#include "Data.h"
#include "DlgAddEvents.h"

extern CData db;

// ���������� ���� �DlgSelectPeriod

IMPLEMENT_DYNAMIC(�DlgSelectPeriod, CDialog)

�DlgSelectPeriod::�DlgSelectPeriod(CWnd* pParent /*=NULL*/)
	: CDialog(�DlgSelectPeriod::IDD, pParent)
{

}

�DlgSelectPeriod::~�DlgSelectPeriod()
{
}

void �DlgSelectPeriod::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, tree);
}


BEGIN_MESSAGE_MAP(�DlgSelectPeriod, CDialog)
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &�DlgSelectPeriod::OnNMRClickTree1)
	ON_COMMAND(ID_TREE_BUT_VIEWEDIT, &�DlgSelectPeriod::OnTreeButViewedit)
	ON_COMMAND(ID_ADDPERIOD_PLAN, &�DlgSelectPeriod::OnAddperiodPlan)
END_MESSAGE_MAP()


// ����������� ��������� �DlgSelectPeriod


BOOL �DlgSelectPeriod::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �������� �������������� �������������

	DWORD old_style;
	old_style=tree.GetStyle();
	::SetWindowLong(tree.GetSafeHwnd(),GWL_STYLE,old_style|TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT);
	
	CRect rect;
	GetClientRect(&rect);
	
	int delta=60;
	tree.MoveWindow(10,delta,rect.Width()-20,rect.Height()-delta-10);

	CStatic* groupRect = (CStatic*) this->GetDlgItem(IDC_STATIC);
	groupRect->MoveWindow(5,40,rect.Width()-10,rect.Height()-45);

	CString s; s.Format(L"%d",db.m_iStatus);

	UpdateTree();

	groupRect=NULL;
	delete groupRect;

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����������: �������� ������� OCX ������ ���������� �������� FALSE
}


void �DlgSelectPeriod::OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �������� ���� ��� ����������� �����������

	CMenu* pPopupMenu=new CMenu;

	pPopupMenu->LoadMenuW(IDR_TREE_SELECT_NODE);

	CPoint obCursorPoint = (0, 0); 

    GetCursorPos(&obCursorPoint); 
	
	tree.ScreenToClient(&obCursorPoint); 
    
	UINT uFlags; 
	
	HTREEITEM SelectNode=tree.HitTest(obCursorPoint, &uFlags); 
    
	tree.Select(SelectNode,TVGN_CARET); 

	
	//�������� ������������� ���������� ������ ���� ������
	db.m_szYear = "0";
	db.m_szQuart = "0";
	db.m_szMonth = "0";

	//���������� ��� ������ � �������:
	HTREEITEM hTItem[3];//���������
	bool bHasSelect=false;//����
	int iStructDate=0;//����

	if (obCursorPoint != CPoint(-1, -1))
	{
		// ������� ������� �������:
		GetCursorPos(&obCursorPoint);

		UINT flags = 0;

		if (SelectNode != NULL)
		{
			iStructDate = 1;

			bHasSelect = true;

			tree.SelectItem(SelectNode);	

			hTItem[0] = tree.GetParentItem(SelectNode);

			if(hTItem[0])
			{		
				iStructDate = 2;

				hTItem[1] = tree.GetParentItem(hTItem[0]);

				if(hTItem[1])
				{
					iStructDate = 3;

					bHasSelect = true;
				}
				else
				{
					bHasSelect = true;
				}
			}
		}
	}

	if(iStructDate == 0)
	{
	
	}
	if(SelectNode!=NULL)
	{
		if(iStructDate ==1)
		{
			db.m_szYear = tree.GetItemText(SelectNode);
			db.m_szQuart = "0";
			db.m_szMonth = "0";
		}
		if(iStructDate ==2)
		{
			db.m_szYear = tree.GetItemText(hTItem[0]);
			db.m_szQuart = tree.GetItemText(SelectNode);
			db.m_szMonth = "0";
		}
		if(iStructDate ==3)
		{
			db.m_szYear = tree.GetItemText(hTItem[1]);
			db.m_szQuart = tree.GetItemText(hTItem[0]);
			db.m_szMonth = tree.GetItemText(SelectNode);
		}
	}
	tree.SelectItem(NULL);
	if(!bHasSelect)
	{	
		CMenu* pSub=pPopupMenu->GetSubMenu(1);

		if(pSub)
		{
			pSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, obCursorPoint.x,obCursorPoint.y, this);
		}

		pSub=NULL;
		delete pSub;
	}
	else
	{
		
		if((StrCmp(db.m_szYear,L"0")!=0)&&(StrCmp(db.m_szQuart,L"0")==0)&&(StrCmp(db.m_szMonth,L"0")==0))
		{
			CMenu* pSub=pPopupMenu->GetSubMenu(0);

			if(pSub)
			{
				pSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, obCursorPoint.x,obCursorPoint.y, this);
			}
			pSub=NULL;
			delete pSub;
			bHasSelect = true;
		}
		
		if((StrCmp(db.m_szYear,L"0")!=0)&&(StrCmp(db.m_szQuart,L"0")!=0)&&(StrCmp(db.m_szMonth,L"0")==0))
		{
			CMenu* pSub=pPopupMenu->GetSubMenu(0);

			if(pSub)
			{
				pSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, obCursorPoint.x,obCursorPoint.y, this);
			}
			pSub=NULL;
			delete pSub;
			bHasSelect = true;
		}
		if((StrCmp(db.m_szYear,L"0")!=0)&&(StrCmp(db.m_szQuart,L"0")!=0)&&(StrCmp(db.m_szMonth,L"0")!=0))
		{
			CMenu* pSub=pPopupMenu->GetSubMenu(0);

			if(pSub)
			{
				pSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, obCursorPoint.x,obCursorPoint.y, this);
			}
			pSub=NULL;
			delete pSub;
			bHasSelect = true;
		}
	}

	pPopupMenu=NULL;
	delete pPopupMenu;

	*pResult = 0;

	
}


void �DlgSelectPeriod::OnTreeButViewedit()
{
	// TODO: �������� ���� ��� ����������� ������

	if(((StrCmp(db.m_szYear,L"0")!=0)||(StrCmp(db.m_szQuart,L"0")!=0)||(StrCmp(db.m_szMonth,L"0")!=0)))
	{
		CDialog::OnOK();
	}
}


void �DlgSelectPeriod::OnAddperiodPlan()
{
	// TODO: �������� ���� ��� ����������� ������
	DlgAddEvents dlg;
	dlg.DoModal();
	UpdateTree();
}


void �DlgSelectPeriod::UpdateTree(void)
{
	CRetQuery crq;
	HTREEITEM* hYears;
	HTREEITEM hKvart[4];
	TVITEM tv; // �������� �������� ������	
	TVINSERTSTRUCT tvins; // ���������� ��� ���������� ������ �������� � ������
	CString pszKvart[4];
	CString pszMonths[12];
	int limit_month=0;
	long sel_image=0;

	pszKvart[0]="I �������";
	pszKvart[1]="II �������";
	pszKvart[2]="III �������";
	pszKvart[3]="IV �������";

	pszMonths[0]="������";
	pszMonths[1]="�������";
	pszMonths[2]="����";
	pszMonths[3]="������";
	pszMonths[4]="���";
	pszMonths[5]="����";
	pszMonths[6]="����";
	pszMonths[7]="������";
	pszMonths[8]="��������";
	pszMonths[9]="�������";
	pszMonths[10]="������";
	pszMonths[11]="�������";

	tree.DeleteAllItems();
	
	db.fQueryRead(L"SELECT YEAR_NAME FROM YEARS",crq);

	if(crq.m_lRecCount==0)
		return;

	hYears=new HTREEITEM[crq.m_lRecCount];

	for(int i=0;i<crq.m_lRecCount;i++)
	{
		tv.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tv.pszText = T2W(crq.m_RetValues[i][0].GetBuffer());
		crq.m_RetValues[i][0].ReleaseBuffer();
		tv.cchTextMax = 5;
		tv.iImage = 0;
		tv.iSelectedImage = 0;
		tvins.hParent = TVI_ROOT;
		tvins.hInsertAfter = TVI_LAST;
		tvins.item = tv;
		hYears[i] = tree.InsertItem(&tvins);
		tree.SetItemState(hYears[i], 0, 0);
		tree.Expand(hYears[i], TVE_EXPAND);

		for(int i2=0;i2<4;i2++)
		{
			// ���������� ��������1, ����������� �� �����
			tv.pszText = T2W(pszKvart[i2].GetBuffer());
			pszKvart[i2].ReleaseBuffer();
			tv.cchTextMax = 10;	
			tv.iImage = 1;
			tv.iSelectedImage = 1;
			tvins.hParent = hYears[i];
			tvins.hInsertAfter = TVI_LAST;
			tvins.item = tv;
			hKvart[i2] = tree.InsertItem(&tvins);
			
			for(int i3=limit_month;i3<limit_month+3;i3++)
			{
				tv.pszText = T2W(pszMonths[i3].GetBuffer());
				pszMonths[i3].ReleaseBuffer();
				tv.cchTextMax = 10;	
				tv.iImage = i3+2;
				tv.iSelectedImage = i3+2;
				tvins.hParent = hKvart[i2];
				tvins.hInsertAfter = TVI_LAST;
				tvins.item = tv;
				tree.InsertItem(&tvins);
			}
			if(limit_month<8)
				limit_month+=3;
		}
		limit_month=0;
	}

	delete [] hYears;

	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;
}
