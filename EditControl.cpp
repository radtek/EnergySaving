// EditControl.cpp: файл реализации
//

#include "stdafx.h"
#include "MFCApplication30.h"
#include "EditControl.h"


// CEditControl

IMPLEMENT_DYNAMIC(CEditControl, CEdit)

CEditControl::CEditControl()
: m_bESC(FALSE)
{

}

CEditControl::~CEditControl()
{
}


BEGIN_MESSAGE_MAP(CEditControl, CEdit)
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_CHAR()
END_MESSAGE_MAP()



// обработчики сообщений CEditControl




BOOL CEditControl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: добавьте специализированный код или вызов базового класса

	return CEdit::Create(dwStyle, rect, pParentWnd, nID);
}


void CEditControl::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	// TODO: добавьте свой код обработчика сообщений
	DestroyWindow();
}


void CEditControl::OnNcDestroy()
{
	CEdit::OnNcDestroy();

	// TODO: добавьте свой код обработчика сообщений
	delete this;
}


BOOL CEditControl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: добавьте специализированный код или вызов базового класса
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN
				|| pMsg->wParam == VK_DELETE
				|| pMsg->wParam == VK_ESCAPE
				|| GetKeyState( VK_CONTROL)
				)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;		    	// DO NOT process further
		}
	}
	return CEdit::PreTranslateMessage(pMsg);
}


void CEditControl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if( nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		if( nChar == VK_ESCAPE )
			m_bESC = TRUE;
		GetParent()->SetFocus();
		return;
	}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
