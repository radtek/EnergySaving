
// MainFrm.h : ��������� ������ CMainFrame
//

#pragma once
#include "afxwin.h"
#include "ComboQuartal.h"
#include "afxext.h"

//��� Excel

class CMainFrame : public CFrameWnd
{
	
protected: // ������� ������ �� ������������
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ��������
protected:
	
public:
	CSplitterWnd m_wndSplitter;
// ��������
public:

// ���������������
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����������
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ���������� ����� ������ ��������� ����������
	CToolBar          m_wndToolBar;
	//CStatusBar        m_wndStatusBar;
public:
	CStatusBar        m_wndStatusBar;
// ��������� ������� ����� ���������
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnServiceSettings();
	afx_msg void OnSetDirect();
	afx_msg void OnMainServiceSettings();
	bool UpdateStatusBar(void);
	afx_msg void OnMainServiceTime();
	bool UpdateTreeCtrl(void);
	CComboBox m_wndComboPF;
	CComboBox m_wndComboY;
	CComboBox m_wndComboQ;
	CComboQuartal m_wndComboQRTL;
	CString m_szOldYear;
	CToolBar m_wndToolBarAdd;
	afx_msg void OnPrintReport();
//	CApplication oExcel;
//	CWorkbook oBook;
	bool executeCommandLine(CString cmdLine, DWORD & exitCode);
	bool UpdateComboYear(void);
	afx_msg void OnPeriodAdddel();
};


