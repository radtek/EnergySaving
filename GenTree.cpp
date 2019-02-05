// GenTree.cpp: файл реализации
//

#include "stdafx.h"
#include "MFCApplication30.h"
//#include "MFCApplication30View.h"
#include "GenTree.h"
#include "Data.h"
#include "MainFrm.h"


// CGenTree

#define WM_UPDATELISTVIEW WM_USER+15

IMPLEMENT_DYNCREATE(CGenTree, CTreeView)

CGenTree* pGenTree;

CGenTree::CGenTree()
{
//	*pTree=this->GetTreeCtrl();
	pGenTree=this;
	m_iHavePress=0;
	pPopupMenu=new CMenu;
	pPopupMenu->LoadMenuW(IDR_TREE_SELECT_NODE);
}

CGenTree::~CGenTree()
{
	pPopupMenu->DestroyMenu();
	delete pPopupMenu;
}

BEGIN_MESSAGE_MAP(CGenTree, CTreeView)
//	ON_WM_CONTEXTMENU()
//	ON_WM_RBUTTONDOWN()
//	ON_WM_RBUTTONUP()
//	ON_WM_RBUTTONDBLCLK()
//	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CGenTree::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CGenTree::OnNMRClick)
	ON_COMMAND(ID_TREE_BUT_VIEWEDIT, &CGenTree::OnTreeButViewedit)
//	ON_NOTIFY_REFLECT(NM_DBLCLK, &CGenTree::OnNMDblclk)
	ON_MESSAGE(WM_TREECTRL_UPDATETREE, &CGenTree::OnTreectrlUpdatetree)
//	ON_NOTIFY_REFLECT(NM_CLICK, &CGenTree::OnNMClick)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CGenTree::OnNMDblclk)
END_MESSAGE_MAP()


// диагностика CGenTree

#ifdef _DEBUG
void CGenTree::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CGenTree::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// обработчики сообщений CGenTree

extern CData db;

void CGenTree::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	

	//Загрузка иконок для элементов TreeView
	im_tree.Create(16,16,NULL,0,1);
	im_tree.Add(AfxGetApp()->LoadIconW(IDI_TREE_GASTYPE));
	im_tree.Add(AfxGetApp()->LoadIconW(IDI_TREE_ELECTRTYPE));
	im_tree.Add(AfxGetApp()->LoadIconW(IDI_TREE_HEATTYPE));
	im_tree.Add(AfxGetApp()->LoadIconW(IDI_TREE_EVENTGAS));
	im_tree.Add(AfxGetApp()->LoadIconW(IDI_TREE_EVENTEL));
	im_tree.Add(AfxGetApp()->LoadIconW(IDI_TREE_EVENTHEAT));
	im_tree.Add(AfxGetApp()->LoadIconW(IDI_TREE_DESTREPGEN));
	im_tree.Add(AfxGetApp()->LoadIconW(IDI_TREE_DESTREPADD));
	this->GetTreeCtrl().SetImageList(&im_tree,TVSIL_NORMAL);
	
	//Настройка стиля
	DWORD old_style;
	old_style=this->GetTreeCtrl().GetStyle();
	::SetWindowLong(this->GetTreeCtrl().GetSafeHwnd(),GWL_STYLE,old_style|TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT);

	theApp.Mes();
}

bool CGenTree::UpdateStatusBar(void)
{
	CMainFrame* pFrm=(CMainFrame*) theApp.m_pMainWnd;
	
	pFrm->UpdateStatusBar();

	pFrm=NULL;

	delete pFrm;
	
	return false;
}

void CGenTree::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{

	SelectItemTree(true);

	*pResult = 0;
}

void CGenTree::OnTreeButViewedit()
{
	ExeOpClick();
}

void CGenTree::SelectInBoldItem(bool bWhat, HTREEITEM hTree)
{
	HTREEITEM hTItem[3];

	bool bChangeState[3];
	
	UINT iStyle=0;

	for(int i=0;i<3;i++) 
	{
		hTItem[i] = NULL;

		bChangeState[i] = false;
	}

	hTItem[0] = hTree;

	if(bWhat)
	{
		iStyle = TVIS_BOLD;
	}
	else
	{
		iStyle = TVSIL_NORMAL;
	}
	if(hTItem[0])
	{
		bChangeState[0] = true;

		hTItem[1] = this->GetTreeCtrl().GetParentItem(hTItem[0]);

		if(hTItem[1])
		{
			bChangeState[1] = true;

			hTItem[2] = this->GetTreeCtrl().GetParentItem(hTItem[1]);

			if(hTItem[2])
			{
				bChangeState[2] = true;
			}
		}
	}
	
	if(bChangeState[0])
	{
		this->GetTreeCtrl().SetItemState(hTItem[0],iStyle,TVIS_BOLD);
	}
	if(bChangeState[1])
	{
		this->GetTreeCtrl().SetItemState(hTItem[1],iStyle,TVIS_BOLD);
	}
	if(bChangeState[2])
	{
		this->GetTreeCtrl().SetItemState(hTItem[2],iStyle,TVIS_BOLD);
	}
}

void CGenTree::UpdateTree(void)
{
	//TODO: insert return statement here
	this->FilGenTreeCtrl();

	//Осн отчет
	HTREEITEM hti=this->GetTreeCtrl().GetRootItem();
	//Доп отч
	HTREEITEM hti2=this->GetTreeCtrl().GetNextItem(hti,TVGN_NEXT);

	if(StrCmp(db.m_szGenAdd,L"ОСНОВНОЙ ОТЧЕТ")==0)
	{
		this->GetTreeCtrl().Expand(hti,TVE_EXPAND);
		SelectInBoldItem(1,hti);
		ExpandTreeItems(hti);
	}
	else
	{
		this->GetTreeCtrl().Expand(hti2,TVE_EXPAND);
		SelectInBoldItem(1,hti2);
		ExpandTreeItems(hti2);
	}
}

bool CGenTree::ExpandTreeItems(HTREEITEM root)
{
	HTREEITEM resource = this->GetTreeCtrl().GetChildItem(root);
	if(StrCmp(db.m_szTypeResource,this->GetTreeCtrl().GetItemText(resource))==0)
	{
		this->GetTreeCtrl().Expand(resource,TVE_EXPAND);
		SelectInBoldItem(1,resource);
		hTreeItemOld=resource;
	}
	else
	{
		resource = this->GetTreeCtrl().GetNextItem(resource,TVGN_NEXT);
		if(StrCmp(db.m_szTypeResource,this->GetTreeCtrl().GetItemText(resource))==0)
		{
			this->GetTreeCtrl().Expand(resource,TVE_EXPAND);
			SelectInBoldItem(1,resource);
			hTreeItemOld=resource;
		}
		else
		{
			resource = this->GetTreeCtrl().GetNextItem(resource,TVGN_NEXT);
			if(StrCmp(db.m_szTypeResource,this->GetTreeCtrl().GetItemText(resource))==0)
			{
				this->GetTreeCtrl().Expand(resource,TVE_EXPAND);
				SelectInBoldItem(1,resource);
				hTreeItemOld=resource;
			}
		}
	}

	return false;
}

afx_msg LRESULT CGenTree::OnTreectrlUpdatetree(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

bool CGenTree::FilGenTreeCtrl(void)
{
	this->GetTreeCtrl().DeleteAllItems();
	CRetQuery crq;
	HTREEITEM* hYears;
	HTREEITEM hTER[3];
	HTREEITEM hREP[2];
	TVITEM tv; // Атрибуты элемента дерева	
	TVINSERTSTRUCT tvins; // Информация для добавления нового элемента к дереву
	CString pszTER[3];
	CString pszREP[2];
	int cur_im;
	pszREP[0]=L"ОСНОВНОЙ ОТЧЕТ";
	pszREP[1]=L"ДОПОЛНИТЕЛЬНЫЙ ОТЧЕТ";

	pszTER[0]="ПРИРОДНЫЙ ГАЗ";
	pszTER[1]="ЭЛЕКТРОЭНЕРГИЯ";
	pszTER[2]="ТЕПЛОВАЯ ЭНЕРГИЯ";

	CString q; q.Format(L"SELECT EVENT FROM PLAN_EVENT WHERE NYEAR=%s AND GENADD=-D AND ID_EVENT_TYPE=TER",db.m_szYear);
	CString b,c,d;
	c=q;

	for(int direct_rep=0; direct_rep<2;direct_rep++)
	{
		tv.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tv.pszText = T2W(pszREP[direct_rep].GetBuffer());
		pszREP[direct_rep].ReleaseBuffer();
		tv.cchTextMax = 5;
		tv.iImage = (direct_rep==0?6:7);
		tv.iSelectedImage = (direct_rep==0?6:7);
		tvins.hParent = TVI_ROOT;
		tvins.hInsertAfter = TVI_LAST;
		tvins.item = tv;
		hREP[direct_rep] = this->GetTreeCtrl().InsertItem(&tvins);
		b.Format(L"%d",direct_rep);
		q.Replace(L"-D",b);

		for(int i=0;i<3;i++)
		{
			
			tv.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			tv.pszText = T2W(pszTER[i].GetBuffer());
			pszTER[i].ReleaseBuffer();
			tv.cchTextMax = 5;
			tv.iImage = i;
			tv.iSelectedImage = i;
			tvins.hParent = hREP[direct_rep];
			tvins.hInsertAfter = TVI_LAST;
			tvins.item = tv;
			hTER[i] = this->GetTreeCtrl().InsertItem(&tvins);
			
			if(i==0)
			{
				d=L"2";
			}
			else
				if(i==1)
				{
					d=L"3";
				}
				else
					if(i==2)
					{
						d=L"4";
					}

			q.Replace(L"TER",d);

			db.fQueryRead(q,crq);

			switch(i)
			{
			case 0:
				cur_im=3;
				break;
			case 1:
				cur_im=4;
				break;
			case 2:
				cur_im=5;
				break;
			default:
				cur_im=0;
				break;
			}

			db.fQueryRead(q,crq);

#ifdef CONVERTING_ALIAS
			//06/01/2019 - Имена в псевдонимы (т.к. каждый год изменяются наименования.
			db.Aliases(crq,0,NAMES_TO_ALIAS);
#endif

			b.Format(L"ID_EVENT_TYPE=%s",d);

			q.Replace(b,L"ID_EVENT_TYPE=TER");

			for(int i2=0;i2<crq.m_lRecCount;i2++)
			{
				
				tv.pszText = T2W(crq.m_RetValues[i2][0].GetBuffer());//T2W(pszTER[i].GetBuffer());
				crq.m_RetValues[i2][0].ReleaseBuffer();
				tv.cchTextMax = 5;
				tv.iImage = cur_im;
				tv.iSelectedImage = cur_im;
				tvins.hParent = hTER[i];
				tvins.hInsertAfter = TVI_LAST;
				tvins.item = tv;
				this->GetTreeCtrl().InsertItem(&tvins);
			}
		}
		q=c;
	}

	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	hYears=NULL;
	delete hYears;

	//theApp.Mes();

	return false;
}
	


void CGenTree::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: добавьте свой код обработчика уведомлений

	SelectItemTree(false);
	//AfxMessageBox(L"SelectItemTree(false)");
	ExeOpClick();
	//AfxMessageBox(L"ExeOpClick();");
	hTreeItemOld=this->GetTreeCtrl().GetSelectedItem();

	*pResult = 0;
}


bool CGenTree::SelectItemTree(bool show)
{
	// TODO: добавьте свой код обработчика уведомлений
	if ((m_iHavePress == 0)&&(show))
		hTreeItemOld = this->GetTreeCtrl().GetSelectedItem();

	CPoint obCursorPoint = (0, 0); 

    GetCursorPos(&obCursorPoint); 
	
	this->GetTreeCtrl().ScreenToClient(&obCursorPoint); 
    
	UINT uFlags; 
	
	HTREEITEM SelectNode=this->GetTreeCtrl().HitTest(obCursorPoint, &uFlags); 
    
	this->GetTreeCtrl().Select(SelectNode,TVGN_CARET); 

	//Обнуляем промежуточные переменные класса базы данных
	//db.m_szYear = "0";
	//db.m_szQuart = "0";
	//db.m_szMonth = "0";

	//Переменные для работы с деревом:

	HTREEITEM hTItem[3];//описатель
	
	bool bHasSelect=false;//флаг
	
	int iStructDate=0;//дата

	if (obCursorPoint != CPoint(-1, -1))
	{
		// Выбрать нажатый элемент:
		GetCursorPos(&obCursorPoint);

		UINT flags = 0;

		if (SelectNode != NULL)
		{
			iStructDate = 1;

			bHasSelect = true;

			this->GetTreeCtrl().SelectItem(SelectNode);	

			hTItem[0] = this->GetTreeCtrl().GetParentItem(SelectNode);

			if(hTItem[0])
			{		
				iStructDate = 2;

				hTItem[1] = this->GetTreeCtrl().GetParentItem(hTItem[0]);

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
			db.m_szGenAdd = this->GetTreeCtrl().GetItemText(SelectNode);
			db.m_szTypeResource = "0";
			db.m_szEventName = "0";
		}
		if(iStructDate ==2)
		{
			db.m_szGenAdd = this->GetTreeCtrl().GetItemText(hTItem[0]);
			db.m_szTypeResource = this->GetTreeCtrl().GetItemText(SelectNode);
			db.m_szEventName  = "0";
		}
		if(iStructDate ==3)
		{
			db.m_szGenAdd = this->GetTreeCtrl().GetItemText(hTItem[1]);
			db.m_szTypeResource = this->GetTreeCtrl().GetItemText(hTItem[0]);
			db.m_szEventName = this->GetTreeCtrl().GetItemText(SelectNode);
		}
	}

	if(show)
	{
		if(!bHasSelect)
		{
		
		}
		else
		{
			if((StrCmp(db.m_szGenAdd,L"0")!=0)&&(StrCmp(db.m_szTypeResource,L"0")==0)&&(StrCmp(db.m_szEventName,L"0")==0))
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
		
			if((StrCmp(db.m_szGenAdd,L"0")!=0)&&(StrCmp(db.m_szTypeResource,L"0")!=0)&&(StrCmp(db.m_szEventName,L"0")==0))
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

			if((StrCmp(db.m_szGenAdd,L"0")!=0)&&(StrCmp(db.m_szTypeResource,L"0")!=0)&&(StrCmp(db.m_szEventName,L"0")!=0)&&((StrCmp(db.m_szQuart,L"0")!=0)&&
				(db.m_iStatus==PLAN)))
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
	}

	if(!show) bHasSelect=true;
	
	m_iHavePress ++;
	
	return false;
}

bool CGenTree::ExeOpClick(void)
{
	// TODO: добавьте свой код обработчика команд

	//Обновление статус бара
	m_iHavePress = 0;

	HTREEITEM hItem;

	hItem = this->GetTreeCtrl().GetSelectedItem();

	SelectInBoldItem(0,hTreeItemOld);

	SelectInBoldItem(1,hItem);

	theApp.Mes();
	
	//AfxMessageBox(L"theApp.Mes();");

	this->UpdateStatusBar();

	return false;
}



