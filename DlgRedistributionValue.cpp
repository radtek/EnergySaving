// DlgRedistributionValue.cpp: файл реализации
//

#include "stdafx.h"
#include "MFCApplication30.h"
#include "DlgRedistributionValue.h"
#include "afxdialogex.h"
#include "Data.h"
//#include "EditRedistr.h"

// диалоговое окно CDlgRedistributionValue
extern CData db;


IMPLEMENT_DYNAMIC(CDlgRedistributionValue, CDialogEx)

CDlgRedistributionValue::CDlgRedistributionValue(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgRedistributionValue::IDD, pParent)
{

}

CDlgRedistributionValue::CDlgRedistributionValue(CString szValue, CString szValue2, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgRedistributionValue::IDD, pParent), m_szValue(szValue), m_szValue1(szValue2)
{

}

CDlgRedistributionValue::~CDlgRedistributionValue()
{
}

void CDlgRedistributionValue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EDIT1, m_oEdit);
}


BEGIN_MESSAGE_MAP(CDlgRedistributionValue, CDialogEx)
	ON_STN_CLICKED(IDC_REMAIN, &CDlgRedistributionValue::OnStnClickedRemain)
	ON_BN_CLICKED(IDOK, &CDlgRedistributionValue::OnBnClickedOk)
END_MESSAGE_MAP()


// обработчики сообщений CDlgRedistributionValue
CDlgRedistributionValue::CDlgRedistributionValue(int dDirect,CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgRedistributionValue::IDD, pParent)
{
	
	
}

BOOL CDlgRedistributionValue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CStatic* pText=(CStatic*) malloc(sizeof(CStatic));
	CString szBuffer;
	CString szEU(L"NaN");
	//AfxMessageBox(db.m_szGenAdd);
	//ќтчеты
	if(StrCmp(db.m_szGenAdd,L"ќ—Ќќ¬Ќќ… ќ“„≈“")==0)
	{
		pText=(CStatic*) this->GetDlgItem(IDC_SOURCE_V);
		szBuffer.Format(L"ќсновной отчЄт");
		pText->SetWindowTextW(szBuffer);

		pText=(CStatic*) this->GetDlgItem(IDC_TARGET_V);
		szBuffer.Format(L"ƒополнительный отчЄт");
		pText->SetWindowTextW(szBuffer);
	}
	else
	{
		pText=(CStatic*) this->GetDlgItem(IDC_SOURCE_V);
		szBuffer.Format(L"ƒополнительный отчЄт");
		pText->SetWindowTextW(szBuffer);

		pText=(CStatic*) this->GetDlgItem(IDC_TARGET_V);
		szBuffer.Format(L"ќсновной отчЄт");
		pText->SetWindowTextW(szBuffer);
	}

	//≈диница измерени€
	pText=(CStatic*) this->GetDlgItem(IDC_AV);
	pText->SetWindowTextW(L"NAN");
	//AfxMessageBox(db.m_szTypeResource);
	if(StrCmp(db.m_szTypeResource,L"ѕ–»–ќƒЌџ… √ј«")==0)
	{
		szBuffer.Format(L"ƒоступно, тыс.м3");
		pText->SetWindowTextW(szBuffer);
		szEU.Format(L"тыс.м3");
	}
	else
		if(StrCmp(db.m_szTypeResource,L"ЁЋ≈ “–ќЁЌ≈–√»я")==0)
		{
			szBuffer.Format(L"ƒоступно, тыс.к¬т х ч");
			pText->SetWindowTextW(szBuffer);
			//pText->SetWindowTextW(L"тыс.к¬т х ч");
			szEU.Format(L"тыс.к¬т х ч");
		}
		else
	if(StrCmp(db.m_szTypeResource,L"“≈ѕЋќ¬јя ЁЌ≈–√»я")==0)
	{
		szBuffer.Format(L"ƒоступно, √кал");
		pText->SetWindowTextW(szBuffer);
		//pText->SetWindowTextW(L"√кал");
		szEU.Format(L"√кал");
	}

	//ƒоступно
	pText=(CStatic*) this->GetDlgItem(IDC_AVVALUE2);
	szBuffer.Format(L"%s",m_szValue);
	pText->SetWindowTextW(szBuffer);

	//≈сли доступное значение <=0 то контрол Edit становитс€ не активным
//	if(_wtof(m_szValue)<=0) m_oEdit.SetReadOnly(1);
	
	pText=(CStatic*) this->GetDlgItem(IDC_REMAIN21);
	szBuffer.Format(L"0.0");
	pText->SetWindowTextW(szBuffer);

	pText=(CStatic*) this->GetDlgItem(IDC_STATICTEXT);
	szBuffer.Format(L"ќбъем перераспределени€, %s:",szEU);
	pText->SetWindowTextW(szBuffer);

	pText=(CStatic*) this->GetDlgItem(IDC_REMAIN);
	szBuffer.Format(L"ќстаток, %s:",szEU);
	pText->SetWindowTextW(szBuffer);

	pText=(CStatic*) this->GetDlgItem(IDC_PERIODVALUE);
	szBuffer.Format(L"%s %s",db.m_szMonth,db.m_szYear);
	pText->SetWindowTextW(szBuffer);

	pText=(CStatic*) this->GetDlgItem(IDC_OBJVALUE);
	szBuffer.Format(L"%s/%s",db.m_szPlace,m_szValue1);
	pText->SetWindowTextW(szBuffer);

	CStatic* pListB=(CStatic*) malloc(sizeof(CStatic));
	pListB=(CStatic*) this->GetDlgItem(IDC_STATIC);
	pListB->SetWindowTextW(db.m_szEventName);

	CreateEditConrol();

	pListB=NULL;
	pText=NULL;
	delete pText,pListB;
	return TRUE;
}


void CDlgRedistributionValue::OnStnClickedRemain()
{
	// TODO: добавьте свой код обработчика уведомлений
}


void CDlgRedistributionValue::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	//CDialogEx::OnOK();

}



void CDlgRedistributionValue::CreateEditConrol(void)
{
		//Ўирина окна
	int l;//ширина окна
	int b;//низ
	int y;//верх
	int x,r;//начальна€ переменна€
	CString szBuf;//просто строковый буфер
	RECT rect;//пр€моугольник
	CStatic* pText;//указатель на текстовый элемент

	this->GetClientRect(&rect);
	l=rect.right-rect.left;

	pText = (CStatic*) this->GetDlgItem(IDC_STATICTEXT);
	pText->GetWindowRect(&rect);

	//ClientToScreen(&rect);
	this->ScreenToClient(&rect);

	x=rect.left;
	y=rect.bottom;
	r=rect.right;
	b=y+30;

	pEdit = new CEditRedistr();
	pEdit->Create(WS_BORDER|WS_CHILD|WS_VISIBLE,CRect(x,y,r,b),this,IDC_EDITREDISTR);	
}
