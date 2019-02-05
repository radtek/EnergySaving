#pragma once


// CEditControl

class CEditControl : public CEdit
{
	DECLARE_DYNAMIC(CEditControl)

public:
	CEditControl();
	virtual ~CEditControl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL m_bESC;
};


