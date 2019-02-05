// ComboQuartal.cpp: ���� ����������
//

#include "stdafx.h"
#include "MFCApplication30.h"
#include "ComboQuartal.h"
#include "Data.h"


// CComboQuartal

IMPLEMENT_DYNAMIC(CComboQuartal, CComboBox)

CComboQuartal::CComboQuartal()
{

}

CComboQuartal::~CComboQuartal()
{
}

extern CData db;

BEGIN_MESSAGE_MAP(CComboQuartal, CComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, &CComboQuartal::OnCbnSelchange)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, &CComboQuartal::OnCbnDropdown)
END_MESSAGE_MAP()



// ����������� ��������� CComboQuartal




void CComboQuartal::OnCbnSelchange()
{
	// TODO: �������� ���� ��� ����������� �����������
	CString str; 
		
	//this->pComboMonth->ResetContent();
	//this->pComboMonth->AddString(L"-");
	//����������� ��� ������� ������ ��� ������� �����
	if(db.m_iStatus==PLAN)
	{
		return;
	}
		
	this->GetLBText(this->GetCurSel(),str);
	
	if((this->pComboMonth) && (this->GetCurSel()!=0))
	{
		this->pComboMonth->ResetContent();

		if(this->GetCurSel()==1)
		{
			this->pComboMonth->AddString(L"-");
			this->pComboMonth->AddString(L"������");
			this->pComboMonth->AddString(L"�������");
			this->pComboMonth->AddString(L"����");
			this->pComboMonth->SetCurSel(0);
		}
		else
		if(this->GetCurSel()==2)
		{
			this->pComboMonth->AddString(L"-");
			this->pComboMonth->AddString(L"������");
			this->pComboMonth->AddString(L"���");
			this->pComboMonth->AddString(L"����");
			this->pComboMonth->SetCurSel(0);
		}
		else
		if(this->GetCurSel()==3)
		{
			this->pComboMonth->AddString(L"-");
			this->pComboMonth->AddString(L"����");
			this->pComboMonth->AddString(L"������");
			this->pComboMonth->AddString(L"��������");
			this->pComboMonth->SetCurSel(0);
		}
		else
		if(this->GetCurSel()==4)
		{
			this->pComboMonth->AddString(L"-");
			this->pComboMonth->AddString(L"�������");
			this->pComboMonth->AddString(L"������");
			this->pComboMonth->AddString(L"�������");
			this->pComboMonth->SetCurSel(0);
		}
	}
	else
		if(this->GetCurSel()==0)
		{
			this->pComboMonth->ResetContent();
			this->pComboMonth->AddString(L"-");
			this->pComboMonth->SetCurSel(0);
		}
}


void CComboQuartal::OnCbnDropdown()
{
	//this->pComboMonth->ResetContent();
//	this->pComboMonth->AddString(L"-");
//	this->pComboMonth->SetCurSel(0);
	// TODO: �������� ���� ��� ����������� �����������
}
