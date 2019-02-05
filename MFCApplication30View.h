
// MFCApplication30View.h : интерфейс класса CMFCApplication30View
//

#pragma once

#include "GenList.h"

#define WM_UPDATELISTVIEW WM_USER+15

class CMFCApplication30View : public CView
{
protected: // создать только из сериализации
	CMFCApplication30View();
	DECLARE_DYNCREATE(CMFCApplication30View)

// Атрибуты
public:
	CMFCApplication30Doc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Реализация
public:
	virtual ~CMFCApplication30View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUpdatelistview(WPARAM wParam, LPARAM lParam);
public:
	void UpdView(void);
	bool CriterianY(CGenList* c, CString& s,int& id);
	int defineResourceType(void);
	bool SetColumnListCtrl(int& id, CListCtrl*);
	void SetReportItems(void);
	virtual void OnInitialUpdate();
	int	defineQuart(void);
	int defineMonth(void);
	bool FillTable11(CString Query, int iRow, CGenList* pList);
};

#ifndef _DEBUG  // отладочная версия в MFCApplication30View.cpp
inline CMFCApplication30Doc* CMFCApplication30View::GetDocument() const
   { return reinterpret_cast<CMFCApplication30Doc*>(m_pDocument); }
#endif

