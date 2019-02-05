#pragma once


// CComboQuartal

class CComboQuartal : public CComboBox
{
	DECLARE_DYNAMIC(CComboQuartal)

public:
	CComboQuartal();
	virtual ~CComboQuartal();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchange();
	CComboBox* pComboMonth;
	afx_msg void OnCbnDropdown();
};


