
// MFCApplication30.h : ������� ���� ��������� ��� ���������� MFCApplication30
//
//���������:
//06/01/2019 - ��������� ������� ��� ��������� ���� �� ����������, �.�. ������ ��� ���������� �����������
#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"       // �������� �������


// CMFCApplication30App:
// � ���������� ������� ������ ��. MFCApplication30.cpp
//

class CMFCApplication30App : public CWinApp
{
public:
	CMFCApplication30App();


// ���������������
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void Mes(void);
	bool UpdateTree(void);
// ����������
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	
	
};

extern CMFCApplication30App theApp;
