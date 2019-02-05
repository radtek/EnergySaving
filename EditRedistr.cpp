#include "stdafx.h"
#include "EditRedistr.h"


IMPLEMENT_DYNAMIC(CEditRedistr, CEdit)

	BEGIN_MESSAGE_MAP(CEditRedistr, CEdit)

		ON_WM_CHAR()
	END_MESSAGE_MAP()

CEditRedistr::CEditRedistr(void)
	: m_dCutValue(0)
{
}


CEditRedistr::~CEditRedistr(void)
{
}


double CEditRedistr::GetCutValue(void)
{
	return m_dCutValue;
}


void CEditRedistr::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if( nChar == VK_ESCAPE)
	{
		/*if( nChar == VK_ESCAPE )
			m_bESC = TRUE;
		GetParent()->SetFocus();*/
		//return;
		GetParent()->SetFocus();
		return;
	}

	if(nChar == VK_RETURN)
	{
		//AfxMessageBox(L"Enter.");
		GetParent()->SetFocus();
		return;
	}


	CEdit::OnChar(nChar, nRepCnt, nFlags);
}


BOOL CEditRedistr::PreTranslateMessage(MSG* pMsg)
{
	// TODO: добавьте специализированный код или вызов базового класса
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
