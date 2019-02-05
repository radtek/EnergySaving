#pragma once
#include "afxwin.h"
class CEditRedistr :
	public CEdit
{
	DECLARE_DYNAMIC(CEditRedistr)
public:
	CEditRedistr(void);
	~CEditRedistr(void);
protected:
	DECLARE_MESSAGE_MAP()

public:
	double m_dCutValue;
	double GetCutValue(void);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

