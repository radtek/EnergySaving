// GenList.cpp: файл реализации
//

#include "stdafx.h"
#include "MFCApplication30.h"
#include "GenList.h"
#include "Data.h"
#include "DlgAddNewObject.h"
#include "DlgCalcRes.h"
#include "DlgPlaceUsing.h"
#include "DlgAddEvent2.h"
#include "DlgObjects.h"
#include "DlgEditResIndexes.h"

// CGenList

IMPLEMENT_DYNCREATE(CGenList, CListView)

extern CData db;

CGenList::CGenList()
	: m_iDelItem(0)
	, m_iDelSubItem(0)
{
	deletingEvent=L"0";
}

CGenList::~CGenList()
{
}

BEGIN_MESSAGE_MAP(CGenList, CListView)
	//ON_COMMAND(ID_TREE_BUT_VIEWEDIT, &CGenList::OnTreeButViewedit)
	//ON_MESSAGE(WM_UPDATELISTVIEW, &CGenList::OnUpdatelistview)
	ON_MESSAGE(WM_UPDLIST, &CGenList::OnUpdlist)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CGenList::OnNMRClick)
//	ON_WM_CONTEXTMENU()
ON_COMMAND(ID_ADDOBJECT, &CGenList::OnAddobject)
ON_NOTIFY_REFLECT(NM_DBLCLK, &CGenList::OnNMDblclk)
ON_COMMAND(ID_ADDPLACE, &CGenList::OnAddplace)
ON_COMMAND(ID_ADDEVENT, &CGenList::OnAddevent)
ON_COMMAND(ID_DELETEITEMTREE, &CGenList::OnDeleteitemtree)
ON_COMMAND(ID_DELETEPLACE, &CGenList::OnDeleteplace)
ON_COMMAND(ID_DELETEITEM_32813, &CGenList::OnPlanCorrection)
END_MESSAGE_MAP()


// диагностика CGenList

#ifdef _DEBUG
void CGenList::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CGenList::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG



void CGenList::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: добавьте специализированный код или вызов базового класса
	//Настрйока контрола листа
	DWORD style = GetWindowLong(this->GetListCtrl().m_hWnd,GWL_STYLE);
	SetWindowLong(this->GetListCtrl().m_hWnd, GWL_STYLE, (style&~LVS_TYPEMASK) | LVS_REPORT);
	
	this->GetListCtrl().SetExtendedStyle(this->GetListCtrl().GetExtendedStyle() |
						LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	/*
	this->GetListCtrl().InsertColumn(0, L"Название", LVCFMT_LEFT, 70);
	this->GetListCtrl().InsertColumn(1, L"Инф1", LVCFMT_LEFT, 70);
	this->GetListCtrl().InsertColumn(2, L"Инф2", LVCFMT_LEFT, 70);

	this->GetListCtrl().InsertItem(0,L"1 элемент",0);
	this->GetListCtrl().InsertItem(1,L"2 элемент",0);
	this->GetListCtrl().InsertItem(2,L"3 элемент",0);

	this->GetListCtrl().SetItemText(0, 1, L"elem1-inf1");
	this->GetListCtrl().SetItemText(0, 2, L"elem1-inf2");
	this->GetListCtrl().SetItemText(1, 1, L"elem2-inf1");
	this->GetListCtrl().SetItemText(1, 2, L"elem2-inf2");
	this->GetListCtrl().SetItemText(2, 1, L"elem3-inf1");
	this->GetListCtrl().SetItemText(2, 2, L"elem3-inf2");
	/*
	//Заполняем List View
	// Добавление трех колонок для таблицы
	this->GetListCtrl().InsertColumn(0, L"Название", LVCFMT_LEFT, 70);
	this->GetListCtrl().InsertColumn(1, L"Инф1", LVCFMT_LEFT, 70);
	this->GetListCtrl().InsertColumn(2, L"Инф2", LVCFMT_LEFT, 70);
	// Добавление в список дополнительной информации
	// по элементам для таблицы
	this->GetListCtrl().SetItemText(0, 1, L"elem1-inf1");
	this->GetListCtrl().SetItemText(0, 2, L"elem1-inf2");
	this->GetListCtrl().SetItemText(1, 1, L"elem2-inf1");
	this->GetListCtrl().SetItemText(1, 2, L"elem2-inf2");
	this->GetListCtrl().SetItemText(2, 1, L"elem3-inf1");
	this->GetListCtrl().SetItemText(2, 2, L"elem3-inf2");
	*/
}


afx_msg LRESULT CGenList::OnUpdlist(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


void CGenList::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: добавьте свой код обработчика уведомлений
	if(StrCmp(db.m_szEventName,L"0")==0)
	{
		if(((StrCmp(db.m_szTypeResource,L"0")!=0)&&(StrCmp(db.m_szQuart,L"0")==0)&&(db.m_iStatus==PLAN)))
		{
			CPoint obCursorPoint = (0, 0); 
	
			CMenu* pPopupMenu=new CMenu;

			pPopupMenu->LoadMenuW(IDR_TREE_SELECT_NODE);
	
			GetCursorPos(&obCursorPoint);
	
			if(pNMItemActivate->iItem==-1)
			{
				CMenu* pSub=pPopupMenu->GetSubMenu(4);
	
				if(pSub)
				{		
					pSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, obCursorPoint.x,obCursorPoint.y, this);
				}

				pSub=NULL;
				delete pSub;
			}
			else
			{
				CMenu* pSub=pPopupMenu->GetSubMenu(5);
	
				deletingEvent=this->GetListCtrl().GetItemText(pNMItemActivate->iItem,1);
				
				if(pSub)
				{		
					pSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, obCursorPoint.x,obCursorPoint.y, this);
				}

				pSub=NULL;
				delete pSub;
			}
			pPopupMenu=NULL;
			delete pPopupMenu;
		}
	}
	else
	{
		if(((StrCmp(db.m_szTypeResource,L"0")!=0)&&(StrCmp(db.m_szQuart,L"0")==0)&&(db.m_iStatus==PLAN)))
		{
			CPoint obCursorPoint = (0, 0); 
	
			CMenu* pPopupMenu=new CMenu;

			pPopupMenu->LoadMenuW(IDR_TREE_SELECT_NODE);
	
			GetCursorPos(&obCursorPoint);
	
			if(pNMItemActivate->iItem==-1)
			{
				CMenu* pSub=pPopupMenu->GetSubMenu(6);
	
				if(pSub)
				{		
					pSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, obCursorPoint.x,obCursorPoint.y, this);
				}
				pSub=NULL;
				delete pSub;
			}
			else
			{
				m_iDelItem=pNMItemActivate->iItem;
				m_iDelSubItem=pNMItemActivate->iSubItem;
				
				db.m_szPlace=this->GetListCtrl().GetItemText(pNMItemActivate->iItem,1);

				CMenu* pSub=pPopupMenu->GetSubMenu(7);
				if(pSub)
				{		
					pSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, obCursorPoint.x,obCursorPoint.y, this);
				}
				pSub=NULL;
				delete pSub;
			}
			pPopupMenu=NULL;
			delete pPopupMenu;
		}
	}
	*pResult = 0;
}


//void CGenList::OnContextMenu(CWnd* pWnd, CPoint point)
//{
//
//}


void CGenList::OnAddobject()
{
	// TODO: добавьте свой код обработчика команд
	CDlgAddNewObject dlg;
	dlg.DoModal();
	theApp.Mes();
}


void CGenList::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	//AfxMessageBox(L"DblClickGenList!!!");

	if (pNMItemActivate->iItem==-1)
		return;
	// TODO: добавьте свой код обработчика уведомлений

	CString str;

	if(db.m_iStatus==PLAN)
	{
		//Вывести диалог с объектами
		if(!(StrCmp(db.m_szQuart,L"0")==0) && !(StrCmp(db.m_szEventName,L"0")==0))
		{
			this->GetListCtrl().GetItemData(pNMItemActivate->iItem);

			str = this->GetListCtrl().GetItemText(pNMItemActivate->iItem,1);
			
			if(str.IsEmpty())
			{
				*pResult = 0;
				return;
			}
			else
			{
				db.m_szPlace=str;
				CDlgObjects dlg;
				dlg.DoModal();
			}
		}

		if(!(StrCmp(db.m_szYear,L"0")==0) && !(StrCmp(db.m_szTypeResource,L"0")==0)&&(StrCmp(db.m_szQuart,L"0")==0) && !(StrCmp(db.m_szEventName,L"0")==0))
		{
			this->GetListCtrl().GetItemData(pNMItemActivate->iItem);

			str = this->GetListCtrl().GetItemText(pNMItemActivate->iItem,1);
			
			if(str.IsEmpty())
			{
				*pResult = 0;
				return;
			}
			else
			{
				db.m_szPlace=str;
				CDlgObjects dlg;
				dlg.DoModal();
			}
		}
	}
	else
		if(db.m_iStatus==FACT)
		{
			//Вывести диалог с объектами
			if(!(StrCmp(db.m_szMonth,L"0")==0)&&!(StrCmp(db.m_szEventName,L"0")==0))
			{
				this->GetListCtrl().GetItemData(pNMItemActivate->iItem);
	
				str = this->GetListCtrl().GetItemText(pNMItemActivate->iItem,1);
				
				//AfxMessageBox(str);

				if(str.IsEmpty())
				{
					*pResult = 0;
					return;
				}
				else
				{
					db.m_szPlace=str;
					CDlgObjects dlg;
					dlg.DoModal();
				}
			}
		}

	*pResult = 0;
}

////////////////////////////////////////////////////
//ПУНКТ КОНТЕКСТНОГО МЕНЮ ДОБАВЛЕНИЯ МЕСТА ВНЕДРЕНИЯ
////////////////////////////////////////////////////
void CGenList::OnAddplace()
{
	// TODO: добавьте свой код обработчика команд
	CDlgPlaceUsing dlg;
	dlg.DoModal();
}

////////////////////////////////////////////////
//ПУНКТ КОНТЕКСТНОГО МЕНЮ ДОБАВЛЕНИЯ МЕРОПРИЯТИЯ
////////////////////////////////////////////////
void CGenList::OnAddevent()
{
	// TODO: добавьте свой код обработчика команд
	CDlgAddEvent2 dlg;
	dlg.DoModal();

}

void CGenList::OnDeleteitemtree()
{
	// TODO: добавьте свой код обработчика команд

	CString s; 
	CString ID_EVENTPLAN;
	CString* ID_PLACE;
//	CString* ID_OBJECT;
//	CString* ID_PLAN_INFO;
	CRetQuery crq;

#ifdef CONVERTING_ALIAS
	s.Format(
		L"SELECT ID_EVENTPLAN FROM PLAN_EVENT WHERE NYEAR=%s AND EVENT='%s'",
		db.m_szYear,
		db.Aliases(deletingEvent,ALIAS_TO_NAMES));
#else
	s.Format(
		L"SELECT ID_EVENTPLAN FROM PLAN_EVENT WHERE NYEAR=%s AND EVENT='%s'",
		db.m_szYear,
		deletingEvent);
#endif
	db.fQueryRead(s,crq);
	ID_EVENTPLAN=crq.m_RetValues[0][0];
	s.Format(
		L"DELETE FROM PLAN_EVENT WHERE ID_EVENTPLAN=%s",
		ID_EVENTPLAN);
	db.ExeSql(s);

	s.Format(
		L"SELECT ID_PLACE FROM PLACEOFUSEPLAN WHERE ID_EVENTPLAN=%s",
		ID_EVENTPLAN);
	db.fQueryRead(s,crq);
	ID_PLACE=new CString[crq.m_lRecCount];

	for(int i=0;i<crq.m_lRecCount;i++)
		ID_PLACE[i]=crq.m_RetValues[i][0];



	for(int i=0;i<crq.m_lRecCount;i++)
	{
		s.Format(L"DELETE FROM OBJECTS_PLANS WHERE ID_PLACE=%s",ID_PLACE[i]);
		db.ExeSql(s);
	}

	s.Format(
		L"DELETE FROM PLACEOFUSEPLAN WHERE ID_EVENTPLAN=%s",
		ID_EVENTPLAN);
	db.ExeSql(s);
	
	if(db.defineEventId(deletingEvent)!=0)
	{
#ifdef CONVERTING_ALIAS
		s.Format(
			L"DELETE FROM %d " 
			L"WHERE ID_PLAN_INFO IN "
			L"(SELECT ID_PLAN_INFO FROM PLAN_INFO WHERE NYEAR=%s AND EVENT='%s')",db.defineEventId(deletingEvent),db.m_szYear,db.Aliases(deletingEvent,ALIAS_TO_NAMES));
		db.ExeSql(s);

		s.Format(
			L"DELETE FROM %dF " 
			L"WHERE ID_FACT_INFO IN "
			L"(SELECT ID_FACT_INFO FROM FACT_INFO WHERE NYEAR=%s AND EVENT='%s')",db.defineEventId(deletingEvent),db.m_szYear,db.Aliases(deletingEvent,ALIAS_TO_NAMES));
		db.ExeSql(s);
#else
		s.Format(
			L"DELETE FROM %d " 
			L"WHERE ID_PLAN_INFO IN "
			L"(SELECT ID_PLAN_INFO FROM PLAN_INFO WHERE NYEAR=%s AND EVENT='%s')",db.defineEventId(deletingEvent),db.m_szYear,deletingEvent);
		db.ExeSql(s);

		s.Format(
			L"DELETE FROM %dF " 
			L"WHERE ID_FACT_INFO IN "
			L"(SELECT ID_FACT_INFO FROM FACT_INFO WHERE NYEAR=%s AND EVENT='%s')",db.defineEventId(deletingEvent),db.m_szYear,deletingEvent);
		db.ExeSql(s);
#endif
	}

	/*//Удаляем строки из таблицы мероприятия
	switch(db.defineEventId(deletingEvent))
	{
	case 1:
		s.Format(
			L"DELETE FROM 1 " 
			L"WHERE ID_PLAN_INFO IN "
			L"(SELECT ID_PLAN_INFO FROM PLAN_INFO WHERE NYEAR=%s AND EVENT='%s')",db.m_szYear,deletingEvent);
		db.ExeSql(s);
		break;
	case 7:
		s.Format(
			L"DELETE FROM 7 " 
			L"WHERE ID_PLAN_INFO IN "
			L"(SELECT ID_PLAN_INFO FROM PLAN_INFO WHERE NYEAR=%s AND EVENT='%s')",db.m_szYear,deletingEvent);
		db.ExeSql(s);
		break;
	case 8:
		s.Format(
			L"DELETE FROM 8 " 
			L"WHERE ID_PLAN_INFO IN "
			L"(SELECT ID_PLAN_INFO FROM PLAN_INFO WHERE NYEAR=%s AND EVENT='%s')",db.m_szYear,deletingEvent);
		db.ExeSql(s);
		break;
	case 12:
		s.Format(
			L"DELETE FROM 12 " 
			L"WHERE ID_PLAN_INFO IN "
			L"(SELECT ID_PLAN_INFO FROM PLAN_INFO WHERE NYEAR=%s AND EVENT='%s')",db.m_szYear,deletingEvent);
		db.ExeSql(s);
		break;
	case 15:
		s.Format(
			L"DELETE FROM 15 " 
			L"WHERE ID_PLAN_INFO IN "
			L"(SELECT ID_PLAN_INFO FROM PLAN_INFO WHERE NYEAR=%s AND EVENT='%s')",db.m_szYear,deletingEvent);
		db.ExeSql(s);
		break;
	default:
		break;
	}
	*/

#ifdef CONVERTING_ALIAS
	s.Format(L"DELETE FROM PLAN_INFO WHERE NYEAR=%s AND EVENT='%s'", db.m_szYear, db.Aliases(deletingEvent,ALIAS_TO_NAMES));
#else
	s.Format(L"DELETE FROM PLAN_INFO WHERE NYEAR=%s AND EVENT='%s'", db.m_szYear, deletingEvent);
#endif
	db.ExeSql(s);

#ifdef CONVERTING_ALIAS
	s.Format(L"DELETE FROM FACT_INFO WHERE NYEAR=%s AND EVENT='%s'", db.m_szYear, db.Aliases(deletingEvent,ALIAS_TO_NAMES));
#else
	s.Format(L"DELETE FROM FACT_INFO WHERE NYEAR=%s AND EVENT='%s'", db.m_szYear, deletingEvent);
#endif
	db.ExeSql(s);

	theApp.Mes();

	theApp.UpdateTree();

	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	delete [] ID_PLACE;
//	delete [] ID_OBJECT;
//	delete [] ID_PLAN_INFO;
}


void CGenList::OnDeleteplace()
{
	// TODO: добавьте свой код обработчика команд
	CString szBuffer;
	CString szBuffer2;

	CString ID_EVENTPLAN;
	CString ID_PLACE;
//	CString* ID_OBJECT;

	CRetQuery crq;
	//CRetQuery crq2;

#ifdef CONVERTING_ALIAS
	szBuffer.Format(
		L"SELECT ID_EVENTPLAN FROM PLAN_EVENT "
		L"WHERE NYEAR=%s AND EVENT='%s'",
		db.m_szYear,
		db.Aliases(db.m_szEventName,ALIAS_TO_NAMES));
#else
	szBuffer.Format(
		L"SELECT ID_EVENTPLAN FROM PLAN_EVENT "
		L"WHERE NYEAR=%s AND EVENT='%s'",
		db.m_szYear,
		db.m_szEventName);
#endif

	db.fQueryRead(szBuffer,crq);

	if(crq.m_lRecCount!=0)
	{
		ID_EVENTPLAN=crq.m_RetValues[0][0];
	}
	else
	{
		AfxMessageBox(L"void CGenList::OnDeleteplace() Ошибка!!!");
		return;
	}

	szBuffer.Format(L"SELECT ID_PLACE FROM PLACEOFUSEPLAN WHERE ID_EVENTPLAN=%s AND NAME_PLACE='%s'",ID_EVENTPLAN,db.m_szPlace);
	db.fQueryRead(szBuffer,crq);

	if(crq.m_lRecCount!=0)
	{
		ID_PLACE=crq.m_RetValues[0][0];
	}
	else
	{
		AfxMessageBox(L"void CGenList::OnDeleteplace() Ошибка!!!");
		return;
	}

	szBuffer.Format(L"SELECT ID_OBJECT FROM OBJECTS_PLANS WHERE ID_PLACE=%s",ID_PLACE);
	db.fQueryRead(szBuffer,crq);

	for(int j=0;j<crq.m_lRecCount;j++)
	{
		szBuffer2.Format(L"DELETE FROM PLAN_INFO WHERE ID_OBJECT=%s",crq.m_RetValues[j][0]);
		db.ExeSql(szBuffer2);
	}

	for(int j=0;j<crq.m_lRecCount;j++)
	{
		szBuffer2.Format(L"DELETE FROM FACT_INFO WHERE ID_OBJECT=%s",crq.m_RetValues[j][0]);
		db.ExeSql(szBuffer2);
	}
	
	szBuffer.Format(L"DELETE FROM OBJECTS_PLANS WHERE ID_PLACE=%s",ID_PLACE);
	db.ExeSql(szBuffer);

	szBuffer.Format(L"DELETE FROM PLACEOFUSEPLAN WHERE ID_PLACE=%s", ID_PLACE);
	db.ExeSql(szBuffer);

	if(db.defineEventId()!=0)
	{
		for(int i=0;i<crq.m_lRecCount;i++)
		{
			szBuffer.Format(L"DELETE FROM %d WHERE ID_OBJECT=%s",db.defineEventId(),crq.m_RetValues[i][0]);
			db.ExeSql(szBuffer);
		}

		for(int i=0;i<crq.m_lRecCount;i++)
		{
			szBuffer.Format(L"DELETE FROM %dF WHERE ID_OBJECT=%s",db.defineEventId(),crq.m_RetValues[i][0]);
			db.ExeSql(szBuffer);
		}
	}

	/*
	switch(db.defineEventId())
	{
	case 1:
		for(int i=0;i<crq.m_lRecCount;i++)
		{
			szBuffer.Format(L"DELETE FROM 1 WHERE ID_OBJECT=%s",crq.m_RetValues[i][0]);
			db.ExeSql(szBuffer);
		}
		break;
	case 7:
		for(int i=0;i<crq.m_lRecCount;i++)
		{
			szBuffer.Format(L"DELETE FROM 7 WHERE ID_OBJECT=%s",crq.m_RetValues[i][0]);
			db.ExeSql(szBuffer);
		}
		break;
	case 8:
		for(int i=0;i<crq.m_lRecCount;i++)
		{
			szBuffer.Format(L"DELETE FROM 8 WHERE ID_OBJECT=%s",crq.m_RetValues[i][0]);
			db.ExeSql(szBuffer);
		}
		break;
	case 12:
		for(int i=0;i<crq.m_lRecCount;i++)
		{
			szBuffer.Format(L"DELETE FROM 12 WHERE ID_OBJECT=%s",crq.m_RetValues[i][0]);
			db.ExeSql(szBuffer);
		}
		break;
	case 15:
		for(int i=0;i<crq.m_lRecCount;i++)
		{
			szBuffer.Format(L"DELETE FROM 15 WHERE ID_OBJECT=%s",crq.m_RetValues[i][0]);
			db.ExeSql(szBuffer);
		}
		break;
	default:
		break;
	}
	*/
	
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

//	delete [] ID_OBJECT;

	db.m_szPlace=L"0";

	theApp.Mes();
}


void CGenList::OnPlanCorrection()
{
	// TODO: добавьте свой код обработчика команд
	db.m_szReconfigEvent=deletingEvent;
	DlgEditResIndexes dlg;
	dlg.DoModal();
	theApp.Mes();
}
