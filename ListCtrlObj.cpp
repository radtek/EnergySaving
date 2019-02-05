// ListCtrlObj.cpp: файл реализации
//

#include "stdafx.h"
#include "MFCApplication30.h"
#include "ListCtrlObj.h"
#include "InPlaceEdit.h"
#include "Data.h"

#include "DlgCalcRes.h"

// CListCtrlObj

IMPLEMENT_DYNAMIC(CListCtrlObj, CListCtrl)

CListCtrlObj::CListCtrlObj()
{

}

CListCtrlObj::~CListCtrlObj()
{
}


BEGIN_MESSAGE_MAP(CListCtrlObj, CListCtrl)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &CListCtrlObj::OnLvnEndlabeledit)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// обработчики сообщений CListCtrlObj

extern CData db;


int CListCtrlObj::HitTestEx(CPoint& point, int* col)
{
	int column=0;
	int row=HitTest(point,NULL);

	if(col) *col=0;

	//Удостовериться что тип лист контрола отчет
	if((GetWindowLong(m_hWnd,GWL_STYLE)&LVS_TYPEMASK)!=LVS_REPORT)
		return row;

	row=GetTopIndex();
	int bottom=row+GetCountPerPage();
	if(bottom>GetItemCount())
		bottom=GetItemCount();

	//Число столбцов
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();

	// Loop through the visible rows
	for( ;row <=bottom;row++)
	{
		// Get bounding rect of item and check whether point falls in it.
		CRect rect;
		GetItemRect( row, &rect, LVIR_BOUNDS );
		if( rect.PtInRect(point) )
		{
			// Now find the column
			for(int colnum = 0; colnum < nColumnCount; colnum++ )
			{
				int colwidth = GetColumnWidth(colnum);
				if( point.x >= rect.left 
					&& point.x <= (rect.left + colwidth ) )
				{
					if( col ) *col = colnum;
					return row;
				}
				rect.left += colwidth;
			}
		}
	}

	pHeader=NULL;
	delete pHeader;

	return -1;
}


CEdit* CListCtrlObj::EditSubLabel(int nItem, int nCol)
{
	// The returned pointer should not be saved
	CString szBuffer;
	//szBuffer.Format(L"%b",db.m_bReconfiguredPlanGTU);
	//AfxMessageBox(L"asdasd");
#ifdef CONVERTING_ALIAS
	if((StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Оптимизация режимов работы компрессорных станций и газопроводов")==0)&&(!db.m_bReconfiguredPlanGTU))
	{
		if((nItem==0)||(nItem==9))
		{
			return NULL;
		}
		if(((nItem>=1)&&(nItem<=8))&&(nCol<2))
		{
			return NULL;
		}
		if(((nItem>=10)&&(nItem<=17))&&(nCol<2))
		{
			return NULL;
		}
		if(((nItem>=18)&&(nItem<=20))&&(nCol!=3))
		{
			return NULL;
		}
		//db.m_bReconfiguredPlanGTU=false;
#else
	if((StrCmp(db.m_szEventName,L"Оптимизация режимов работы компрессорных станций и газопроводов")==0)&&(!db.m_bReconfiguredPlanGTU))
	{
		if((nItem==0)||(nItem==9))
		{
			return NULL;
		}
		if(((nItem>=1)&&(nItem<=8))&&(nCol<2))
		{
			return NULL;
		}
		if(((nItem>=10)&&(nItem<=17))&&(nCol<2))
		{
			return NULL;
		}
		if(((nItem>=18)&&(nItem<=20))&&(nCol!=3))
		{
			return NULL;
		}
		//db.m_bReconfiguredPlanGTU=false;
#endif
	}
	else
	{
		if(db.m_bReconfiguredPlanGTU)
		{
			if((nItem<0)||(nItem>3))
			{
				//db.m_bReconfiguredPlanGTU=false;
				return NULL;
			}
			if((nCol<2)||(nCol>5))
			{
				//db.m_bReconfiguredPlanGTU=false;
				return NULL;
			}
		}
		if((nCol!=2)&&(!db.m_bReconfiguredPlanGTU))
		{
			//db.m_bReconfiguredPlanGTU=false;
			return 0;
		}
	}
	
	// Make sure that the item is visible
	if( !EnsureVisible( nItem, TRUE ) ) return NULL;
 
	// Make sure that nCol is valid
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	if( nCol >= nColumnCount || GetColumnWidth(nCol) < 5 )
		return NULL;
 
	// Get the column offset
	int offset = 0;
	for( int i = 0; i < nCol; i++ )
		offset += GetColumnWidth( i );
 
	CRect rect;
	GetItemRect( nItem, &rect, LVIR_BOUNDS );
 
	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect( &rcClient );
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll( size );
		rect.left -= size.cx;
	}
 
	// Get Column alignment
	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	GetColumn( nCol, &lvcol );
	DWORD dwStyle ;
	if((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
	else if((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
	else dwStyle = ES_CENTER;
 
	rect.left += offset+4;
	rect.right = rect.left + GetColumnWidth( nCol ) - 3 ;
	if( rect.right > rcClient.right) rect.right = rcClient.right;
 
	dwStyle |= WS_BORDER|WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL;
	CEdit *pEdit = new CInPlaceEdit(nItem, nCol, GetItemText( nItem, nCol ));
	pEdit->Create( dwStyle, rect, this, IDC_IPEDIT );
 
	pHeader=NULL; delete pHeader;

	return pEdit;
}


void CListCtrlObj::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if( GetFocus() != this ) SetFocus();
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CListCtrlObj::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if( GetFocus() != this ) SetFocus();
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

extern CDlgCalcRes* pDlgCalcres;

void CListCtrlObj::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	//NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	LV_DISPINFO *plvDispInfo = (LV_DISPINFO *)pNMHDR;
	LV_ITEM	*plvItem = &plvDispInfo->item;
	
	CString szBuffer;
	
	int b=0;
	int countbadsymbols=0;
	int iFind=-1;
	
	//wccmp
	if (plvItem->pszText != NULL)
	{
		szBuffer.Format(L"%s",plvItem->pszText);

		while(-1!=(iFind=szBuffer.Find(L",",iFind+1)))
		{
			countbadsymbols++;
		}

		iFind=-1;

		while(-1!=(iFind=szBuffer.Find(L".",iFind+1)))
		{
			countbadsymbols++;
		}

		if((1>=countbadsymbols)&&(countbadsymbols>=0)&&!((szBuffer.GetLength()==1)&&((szBuffer.Find(L",",0)==0)||(szBuffer.Find(L".",0)==0))))
		{
			szBuffer.Replace(L",",L"0");

			szBuffer.Replace(L".",L"0");

			for(int i=0;i<szBuffer.GetLength();i++)
			{
				if(!iswdigit(szBuffer.GetAt(i)))
				{
					if(!((szBuffer.GetAt(i)=='-')&&(i==0)))
						b=1;
				}
			}
		
			if(b==1)
			{
			}
			else
			{
				szBuffer.Format(L"%s",plvItem->pszText);
				if(countbadsymbols==1)
				{
					szBuffer.Replace(L",",L".");
					
					if(szBuffer.Find(L".")==(szBuffer.GetLength()-1))
					{
						CString szSubString;
						szSubString=szBuffer.Left(szBuffer.Find(L"."));
						szBuffer.Format(L"%s.0",szSubString);
					}
				}
				else
				{
					CString szSubString;
					szSubString=szBuffer;
					szBuffer.Format(L"%s.0",szSubString);
				}

				SetItemText(plvItem->iItem, plvItem->iSubItem, szBuffer);
				
				if(!db.m_bReconfiguredPlanGTU) 
					pDlgCalcres->CalcResult();
			}
		}
	}

	plvDispInfo=NULL; delete plvDispInfo;
	plvItem=NULL; delete plvItem;

	*pResult = FALSE;
}


void CListCtrlObj::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	int index;
	CString s;
	CListCtrl::OnLButtonDown(nFlags, point);
	
	//AfxMessageBox(L"Begin void CListCtrlObj::OnLButtonDown(UINT nFlags, CPoint point)");

	int colnum;
	if( ( index = HitTestEx( point, &colnum )) != -1 )
	{
		UINT flag = LVIS_FOCUSED;
		if( (GetItemState( index, flag ) & flag) == flag && colnum > 0)
		{
			//AfxMessageBox(L"if( (GetItemState( index, flag ) & flag) == flag && colnum > 0)");
			// Add check for LVS_EDITLABELS
			if( GetWindowLong(m_hWnd, GWL_STYLE) & LVS_EDITLABELS )
			{
					EditSubLabel( index, colnum );
			}
		}
		else
			SetItemState( index, LVIS_SELECTED | LVIS_FOCUSED ,
				    	LVIS_SELECTED | LVIS_FOCUSED); 
	}
	//AfxMessageBox(L"End void CListCtrlObj::OnLButtonDown(UINT nFlags, CPoint point)");
	//CListCtrl::OnLButtonDown(nFlags, point);
}
