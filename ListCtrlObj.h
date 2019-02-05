#pragma once


// CListCtrlObj

class CListCtrlObj : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlObj)

public:
	CListCtrlObj();
	virtual ~CListCtrlObj();

protected:
	DECLARE_MESSAGE_MAP()
public:
	int HitTestEx(CPoint& point, int* col);
	CEdit* EditSubLabel(int nItem, int nCol);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


