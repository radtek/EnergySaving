
// MFCApplication30.h : главный файл заголовка для приложения MFCApplication30
//
//Доработки:
//06/01/2019 - добавлена функция для изменения имен на псевдонимы, т.к. каждый год появляются мероприятия
#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CMFCApplication30App:
// О реализации данного класса см. MFCApplication30.cpp
//

class CMFCApplication30App : public CWinApp
{
public:
	CMFCApplication30App();


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void Mes(void);
	bool UpdateTree(void);
// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	
	
};

extern CMFCApplication30App theApp;
