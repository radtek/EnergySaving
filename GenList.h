#pragma once


// представление CGenList

#define WM_UPDLIST WM_USER+10

class CGenList : public CListView
{
	DECLARE_DYNCREATE(CGenList)

protected:
	CGenList();           // защищенный конструктор, используемый при динамическом создании
	virtual ~CGenList();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnTreeButViewedit();
protected:
	//afx_msg LRESULT OnUpdatelistview(WPARAM wParam, LPARAM lParam);
public:
	virtual void OnInitialUpdate();
	CString deletingEvent;
protected:
	afx_msg LRESULT OnUpdlist(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnAddobject();
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddplace();
	afx_msg void OnAddevent();
	afx_msg void OnDeleteitemtree();
	int m_iDelItem;
	int m_iDelSubItem;
	afx_msg void OnDeleteplace();
	afx_msg void OnPlanCorrection();
};


