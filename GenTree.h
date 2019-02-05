#pragma once
#include "afxwin.h"


// ������������� CGenTree

#define WM_TREECTRL_UPDATETREE WM_USER+10



class CGenTree : public CTreeView
{
	DECLARE_DYNCREATE(CGenTree)

protected:
	CGenTree();           // ���������� �����������, ������������ ��� ������������ ��������
	//virtual ~CGenTree();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	virtual ~CGenTree();
protected:
	DECLARE_MESSAGE_MAP()
public:
	CMenu* pPopupMenu;
	int m_iHavePress;//���������� ������������ ���� �������
	HTREEITEM hTreeItemOld;//���������� ��������� ����
	virtual void OnInitialUpdate();
	CImageList im_tree;
	bool UpdateStatusBar(void);
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTreeButViewedit();
	void SelectInBoldItem(bool bWhat, HTREEITEM hTree);
	void UpdateTree(void);
	//CTreeCtrl* pTree;
protected:
	afx_msg LRESULT OnTreectrlUpdatetree(WPARAM wParam, LPARAM lParam);

public:
	bool FilGenTreeCtrl(void);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	bool SelectItemTree(bool /*1 ������� ����, 0 - �� ��������*/);
	bool ExeOpClick(void);
	bool ExpandTreeItems(HTREEITEM htree);
};


