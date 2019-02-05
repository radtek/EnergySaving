#pragma once


// CInPlaceEdit

class CInPlaceEdit : public CEdit
{
	DECLARE_DYNAMIC(CInPlaceEdit)

public:
	CInPlaceEdit(int iItem, int iSubItem, CString sInitText);
	virtual ~CInPlaceEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	int m_iItem;
	int m_iSubItem;
	CString m_sInitText;
	BOOL m_bESC;
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


