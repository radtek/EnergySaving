
// MainFrm.h : интерфейс класса CMainFrame
//

#pragma once
#include "afxwin.h"
#include "ComboQuartal.h"
#include "afxext.h"

//Для Excel

class CMainFrame : public CFrameWnd
{
	
protected: // создать только из сериализации
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Атрибуты
protected:
	
public:
	CSplitterWnd m_wndSplitter;
// Операции
public:

// Переопределение
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Реализация
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // встроенные члены панели элементов управления
	CToolBar          m_wndToolBar;
	//CStatusBar        m_wndStatusBar;
public:
	CStatusBar        m_wndStatusBar;
// Созданные функции схемы сообщений
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


