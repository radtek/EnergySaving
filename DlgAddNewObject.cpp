// DlgAddNewObject.cpp: файл реализации
//

#include "stdafx.h"
#include "MFCApplication30.h"
#include "DlgAddNewObject.h"
#include "afxdialogex.h"
#include "Data.h"

extern CData db;


// диалоговое окно CDlgAddNewObject

IMPLEMENT_DYNAMIC(CDlgAddNewObject, CDialog)

CDlgAddNewObject::CDlgAddNewObject(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddNewObject::IDD, pParent)
{

}

CDlgAddNewObject::~CDlgAddNewObject()
{
}

void CDlgAddNewObject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
}


BEGIN_MESSAGE_MAP(CDlgAddNewObject, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAddNewObject::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgAddNewObject::OnBnClickedCancel)
END_MESSAGE_MAP()


// обработчики сообщений CDlgAddNewObject


void CDlgAddNewObject::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	CString szBuffer1;
	CString szBuffer2;
	CRetQuery crq;

	m_edit1.GetWindowTextW(szBuffer1);

	if(szBuffer1.IsEmpty())
	{
		AfxMessageBox(L"Данные не сохранены");
	}

#ifdef CONVERTING_ALIAS
	szBuffer2.Format(
	L"INSERT INTO OBJECTS_PLANS(OBJECT_NAME, ID_PLACE) "
	L"SELECT '%s', PLUSE.ID_PLACE FROM PLAN_EVENT AS PE INNER JOIN PLACEOFUSEPLAN AS PLUSE "
	L"ON PE.ID_EVENTPLAN=PLUSE.ID_EVENTPLAN "
	L"WHERE PE.NYEAR=%s AND PE.EVENT='%s' "
	L"AND PLUSE.NAME_PLACE='%s'",
	szBuffer1,
	db.m_szYear,
	db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),
	db.m_szPlace);
#else
	szBuffer2.Format(
	L"INSERT INTO OBJECTS_PLANS(OBJECT_NAME, ID_PLACE) "
	L"SELECT '%s', PLUSE.ID_PLACE FROM PLAN_EVENT AS PE INNER JOIN PLACEOFUSEPLAN AS PLUSE "
	L"ON PE.ID_EVENTPLAN=PLUSE.ID_EVENTPLAN "
	L"WHERE PE.NYEAR=%s AND PE.EVENT='%s' "
	L"AND PLUSE.NAME_PLACE='%s'",
	szBuffer1,
	db.m_szYear,
	db.m_szEventName,
	db.m_szPlace);
#endif
	

	db.ExeSql(szBuffer2);
	//AfxMessageBox(szBuffer2);
	szBuffer2.Format(L"SELECT MAX(ID_OBJECT) FROM OBJECTS_PLANS");
	

	db.fQueryRead(szBuffer2,crq);
	
	szBuffer1=crq.m_RetValues[0][0];

	
	//Заполняем таблицу с плановыми сводными показателями
	for(int i=0;i<4;i++)
	{
#ifdef CONVERTING_ALIAS
		szBuffer2.Format(
			L"INSERT INTO PLAN_INFO(NYEAR,NQUART,EVENT,MESUAR,SAVEDS,ECONOMY,CURCOSTS,CURTIME,ID_OBJECT) "
			L"VALUES(%s,%d,'%s','%s',%d,%d,%d,%d,%s)",
			db.m_szYear,
			i+1,
			db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),
			((StrCmp(db.m_szTypeResource,L"ПРИРОДНЫЙ ГАЗ")==0)?L"тыс.м3":(StrCmp(db.m_szTypeResource,L"ЭЛЕКТРОЭНЕРГИЯ")==0)?L"тыс.кВт х ч":L"Гкал"),
			0,
			0,
			0,
			0,
			szBuffer1);
#else
		szBuffer2.Format(
			L"INSERT INTO PLAN_INFO(NYEAR,NQUART,EVENT,MESUAR,SAVEDS,ECONOMY,CURCOSTS,CURTIME,ID_OBJECT) "
			L"VALUES(%s,%d,'%s','%s',%d,%d,%d,%d,%s)",
			db.m_szYear,
			i+1,
			db.m_szEventName,
			((StrCmp(db.m_szTypeResource,L"ПРИРОДНЫЙ ГАЗ")==0)?L"тыс.м3":(StrCmp(db.m_szTypeResource,L"ЭЛЕКТРОЭНЕРГИЯ")==0)?L"тыс.кВт х ч":L"Гкал"),
			0,
			0,
			0,
			0,
			szBuffer1);
#endif
		db.ExeSql(szBuffer2);
	}

	//А также с фактическими показателями
	int quart=1;
	int iter=0;
	for(int i=0;i<12;i++)
	{
		if((i%3==0)&&(i!=0))
		{
			quart++;
		}

#ifdef CONVERTING_ALIAS
		szBuffer2.Format(
			L"INSERT INTO FACT_INFO(NYEAR,NQUART,NMONTH,EVENT,MESUAR,SAVEDS,ECONOMY,CURCOSTS,CURTIME,ID_OBJECT) "
			L"VALUES(%s,%d,%d,'%s','%s',%d,%d,%d,%d,%s)",
			db.m_szYear,
			quart,
			i+1,
			db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),
			((StrCmp(db.m_szTypeResource,L"ПРИРОДНЫЙ ГАЗ")==0)?L"тыс.м3":(StrCmp(db.m_szTypeResource,L"ЭЛЕКТРОЭНЕРГИЯ")==0)?L"тыс.кВт х ч":L"Гкал"),
			0,
			0,
			0,
			0,
			szBuffer1);
#else
		szBuffer2.Format(
			L"INSERT INTO FACT_INFO(NYEAR,NQUART,NMONTH,EVENT,MESUAR,SAVEDS,ECONOMY,CURCOSTS,CURTIME,ID_OBJECT) "
			L"VALUES(%s,%d,%d,'%s','%s',%d,%d,%d,%d,%s)",
			db.m_szYear,
			quart,
			i+1,
			db.m_szEventName,
			((StrCmp(db.m_szTypeResource,L"ПРИРОДНЫЙ ГАЗ")==0)?L"тыс.м3":(StrCmp(db.m_szTypeResource,L"ЭЛЕКТРОЭНЕРГИЯ")==0)?L"тыс.кВт х ч":L"Гкал"),
			0,
			0,
			0,
			0,
			szBuffer1);
#endif
		
		db.ExeSql(szBuffer2);
	}
	CString MAXIDPLAN;
	CString MAXIDFACT;

	szBuffer2.Format(L"SELECT MAX(ID_PLAN_INFO) FROM PLAN_INFO");
	db.fQueryRead(szBuffer2,crq);
	MAXIDPLAN=crq.m_RetValues[0][0];

	szBuffer2.Format(L"SELECT MAX(ID_FACT_INFO) FROM FACT_INFO");
	db.fQueryRead(szBuffer2,crq);
	MAXIDFACT=crq.m_RetValues[0][0];

	//AddToEvData(szBuffer1, crq.m_RetValues[0][0]);
	
	AddToEvData(szBuffer1, MAXIDPLAN, MAXIDFACT);

	//деструктор для crq
	for (int i = 0; i < crq.m_lRecCount; i++)	
	{
		delete [] crq.m_RetValues[i];
	}
	if(crq.m_lRecCount>0)
	delete [] crq.m_RetValues;

	CDialog::OnOK();
}


bool CDlgAddNewObject::UpdateList(void)
{
	

	return false;
}


void CDlgAddNewObject::OnBnClickedCancel()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialog::OnCancel();
}


int CDlgAddNewObject::defineNumberEvent(void)
{
#ifdef CONVERTING_ALIAS
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Химическая промывка трубных пучков утилизационных теплообменников ГПА"														   )==0) return 100;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Оптимизация режимов работы компрессорных станций и газопроводов"														   )==0) return 1;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Выработка газа потребителям перед выполнением ремонтных работ"								       )==0) return 2;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Выработка (оттягивание) газа ГПА КС из ремонтируемых участков газопроводов"											   )==0) return 4;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Очистка (промывка) проточной части осевых компрессоров"															   )==0) return 7;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Ремонт газоперекачивающих агрегатов"																	   )==0) return 8;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Применение безрасходной системы продувки пылеуловителей ГРС"												   )==0) return 12;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Оптимизация количества и времени продувок пылеуловителей"																   )==0) return 15;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Режимно-наладочные испытания котельных"																					       )==0) return 17;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Перевод освещения промышленных и бытовых помещений на осветительное оборудование с использованием светодиодов"   	       )==0) return 20;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Оптимизация схемы электроснабжения путем вывода в резерв ТП и отключения силовых трансформаторов, работающих на холостом ходу")==0) return 23;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Оптимизация режимов работы вентиляторов АВО газа и АВО масла"																		   )==0) return 25;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Очистка поверхностей теплообмена трубных пучков АВО газа и АВО масла"         		       )==0) return 26;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Использование устройств плавного пуска и частотно регулируемого электропривода АВО масла, АВО газа, насосов водоснабжения2")==0) return 261;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Использование устройств плавного пуска и частотно регулируемого электропривода АВО масла, АВО газа, насосов водоснабжения")==0) return 27;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Установка конденсаторных батарей с автоматическим управлением"															   )==0) return 38;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Проведение тепловизионного обследования зданий и сооружений на выявление теплопотерь с их последующим устранением"		   )==0) return 42;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Использование утилизаторов тепла работающих газоперекачивающих агрегатов"												   )==0) return 43;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Регулирование температуры в тепловой сети отопления в зависимости от наружной температуры"								   )==0) return 48;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Регулирование температуры в тепловой сети отопления в зависимости от наружной температуры1"							   )==0) return 481;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Использование утилизаторов тепла работающих газоперекачивающих агрегатов1"												   )==0) return 431;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Выработка (оттягивание) газа мобильными компрессорными установками из ремонтируемых участков газопроводов"				   )==0) return 101;
	if(StrCmp(db.Aliases(db.m_szEventName,ALIAS_TO_NAMES),L"Ремонт газоперекачивающих агрегатов1"																	   )==0) return 81;
#else

#endif
	return 0;
}

void CDlgAddNewObject::AddToEvData(CString ID_OBJECT, CString MAXIDPLANINFO)
{
	CString szBuffer;
	
	int max=_wtoi(MAXIDPLANINFO);

	szBuffer.Format(L"%f",max);

	switch(defineNumberEvent())
	{
	case 1:
#pragma region 1
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 1"
				L"(ID_OBJECT,Q1b,Q2b,Q3b,Q4b,Q5b,Q6b,Q7b,Q8b,"
				L"Q1a,Q2a,Q3a,Q4a,Q5a,Q6a,Q7a,Q8a,"
				L"T1b,T2b,T3b,T4b,T5b,T6b,T7b,T8b,"
				L"T1a,T2a,T3a,T4a,T5a,T6a,T7a,T8a,"
				L"PRICE,COSTS,dQ0,dQ1,RESULT,ECONOMY,CURTIME,ID_PLAN_INFO) "
				L"VALUES(%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,
				max-i);

			db.ExeSql(szBuffer);
		}
#pragma endregion 1
		break;
	case 2:
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 2"
				L"(ID_OBJECT,PINIT, PLIM, TINIT, TLIM,"
				L"P0, DIN, L, COSTS, PRICE,"
				L"DV, VGP, ZINIT, ZLIM, RESULT," 
				L"CURTIME, ECONOMY, ID_PLAN_INFO) "
				L"VALUES(%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,
				max-i);

			db.ExeSql(szBuffer);
		}
	case 4:
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 4"
				L"(ID_OBJECT,P0, P1, T0, T1,"
				L"Q0,PRICE,DIN,L,COSTS,"
				L"TIME,DQ,V,Z0,Z1,RESULT,"
				L"ECONOMY,ID_PLAN_INFO) "
				L"VALUES(%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,
				max-i);

			db.ExeSql(szBuffer);
		}
		break;
	case 7:
#pragma region 7
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 7"
				L"(ID_OBJECT, TMONTH, NZ, q0, Q, NEFF0, NEFF1, KEFF, PRICE, "
				L"COSTS, QUD0, QUD1, DQ, RESULT, ECONOMY, CURTIME, ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,-1,0,-1,
				-1,0,0,0,0,
				0,0,0,0,0,
				max-i);
	
			db.ExeSql(szBuffer);
		}
#pragma endregion 7
		break;
	case 8:
#pragma region 8
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 8"
				L"(ID_OBJECT, Q, NOM, NFACTBEF, NFACTAFTER, KEC, PRICE, COSTS, KTS_B,"
				L"KTS_A,KEFF,RESULT,ECONOMY,CURTIME,ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d)",
				ID_OBJECT,
				0,-1,0,0,0,
				0,0,0,-1,0,
				0,0,0,
				max-i);
	
			db.ExeSql(szBuffer);
		}
#pragma endregion 8
		break;
	case 12:
#pragma region 12
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 12"
				L"(ID_OBJECT,"
				L"T,P,P0,P1,T0,"
				L"T1,QP,V1,V2,V3,"
				L"DQP,TSR0,TSR1,PSR0,PSR1,"
				L"Z0,Z1,Q0,Q1,n,"
				L"Q01,Q11,RESULT,PRICE,COSTS,"
				L"ECONOMY,CURTIME,ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,max-i);
	
			db.ExeSql(szBuffer);
		}
#pragma endregion 12
		break;
	case 15:
#pragma region 15
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 15"
				L"(ID_OBJECT,"
				L"P0,D0,L0,T0,G,"
				L"KRESIST,SHEROH,PATM,PPG,TPROD,"
				L"NEMK,NOMNPROD,FACTNPROD,PRICE,COSTS, "
				L"CURTIME,POPSECH,KGIDR,QSTD,K_DP1P0, "
				L"K_DT1T0,K_DL1L0,KDD1D0,RESULT,ECONOMY, "
				L"ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				max-i);
			db.ExeSql(szBuffer);
		}
#pragma endregion 15
		break;
	case 25:
#pragma region 25
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 25"
				L"(ID_OBJECT,"
				L"N0,NSD,KPD,PRICE,COSTS,"
				L"DW1,RESULT,ECONOMY,CURTIME,"
				L"ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				max-i);
			db.ExeSql(szBuffer);
		}
#pragma endregion 25
		break;
	case 27:
#pragma region 27
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 27"
				L"(ID_OBJECT,"
				L"NAVO,NAVOMAX,DTINOUT,DTMAX,b,"
				L"N,T,PRICE,COSTS,CURTIME,"
				L"KLOADNH,KLOADND,DRES,RESULT,ECONOMY,"
				L"ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				max-i);
			//AfxMessageBox(szBuffer);

			db.ExeSql(szBuffer);
		}
#pragma endregion 27
		break;
	case 38:
#pragma region 38
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 38"
				L"(ID_OBJECT,"
				L"EKRN,WKC,TGANGLEF1,TGANGLEF2,PRICE,"
				L"COSTS,RESULT,ECONOMY,CURTIME,"
				L"ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,
				max-i);
			//AfxMessageBox(szBuffer);
			db.ExeSql(szBuffer);
		}
#pragma endregion 38
		break;
	case 42:
#pragma region 42
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 42 "
				L"(ID_OBJECT,"
				L"KT,S,TAIR,TMAT,NTEP,"
				L"PRICE,COSTS,CURTIME,RESULT,ECONOMY,"
				L"ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				max-i);
			db.ExeSql(szBuffer);
		}
#pragma endregion 42
		break;
	case 48:
#pragma region 48
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 48 "
				L"(ID_OBJECT,"
				L"QFYEAR,TINT,THT,TEXTS,QRN,"
				L"KPD,PRICE,COSTS,RESULT,ECONOMY,"
				L"CURTIME,"				
				L"ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,
				max-i);
			db.ExeSql(szBuffer);
		}
#pragma endregion 48
		break;
		case 481:
#pragma region 481
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 481 "
				L"(ID_OBJECT,"
				L"QFYEAR,TINT,THT,TEXTS,QRN,"
				L"KPD,PRICE,COSTS,RESULT,ECONOMY,"
				L"CURTIME,"				
				L"ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,
				max-i);
			db.ExeSql(szBuffer);
		}
#pragma endregion 481
		break;
	case 101:
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 101"
				L"(ID_OBJECT,P0, P1, T0, T1,"
				L"Q0,PRICE,DIN,L,COSTS,"
				L"TIME,DQ,V,Z0,Z1,RESULT,"
				L"ECONOMY,ID_PLAN_INFO) "
				L"VALUES(%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,
				max-i);

			db.ExeSql(szBuffer);
		}
		break;
	case 81:
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 81"
				L"(ID_OBJECT,T, NNOM, NNOM0, NNOM1,"
				L"QRN,KPD,KEF,KNE,KNE1,"
				L"KKR,Q,PRICE,COSTS,CURTIME,RESULT,"
				L"ECONOMY,ID_PLAN_INFO) "
				L"VALUES(%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,0,
				0,
				max-i);

			db.ExeSql(szBuffer);
		}
		break;
	default:
		break;
	}
}

void CDlgAddNewObject::AddToEvData(CString ID_OBJECT, CString MAXIDPLANINFO, CString MAXIDFACTINFO)
{
	CString szBuffer;
	
	int max=_wtoi(MAXIDPLANINFO);

	szBuffer.Format(L"Number event = %d",defineNumberEvent());
	//AfxMessageBox(szBuffer);

	szBuffer.Format(L"%f",max);

	switch(defineNumberEvent())
	{
	case 100:
#pragma region 0
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 100"
				L"(ID_OBJECT, KUTE, PUT, N, PRICE, COSTS, CURTIME, RESULT, ECONOMY, ID_PLAN_INFO) "
				L"VALUES(%s,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,0,0,0,
				max-i);
			//AfxMessageBox(szBuffer);
			db.ExeSql(szBuffer);
		}

		max=_wtoi(MAXIDFACTINFO);

		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 100F"
				L"(ID_OBJECT, KUTE, PUT, N, PRICE, COSTS, CURTIME, RESULT, ECONOMY, ID_FACT_INFO) "
				L"VALUES(%s,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,0,0,0,
				max-i);
			//AfxMessageBox(szBuffer);
			db.ExeSql(szBuffer);
		}
#pragma endregion 0
		break;
	case 1:
#pragma region 1
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 1"
				L"(ID_OBJECT,Q1b,Q2b,Q3b,Q4b,Q5b,Q6b,Q7b,Q8b,"
				L"Q1a,Q2a,Q3a,Q4a,Q5a,Q6a,Q7a,Q8a,"
				L"T1b,T2b,T3b,T4b,T5b,T6b,T7b,T8b,"
				L"T1a,T2a,T3a,T4a,T5a,T6a,T7a,T8a,"
				L"PRICE,COSTS,dQ0,dQ1,RESULT,ECONOMY,CURTIME,ID_PLAN_INFO) "
				L"VALUES(%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,
				max-i);

			db.ExeSql(szBuffer);
		}

		max=_wtoi(MAXIDFACTINFO);

		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 1F"
				L"(ID_OBJECT,Q1b,Q2b,Q3b,Q4b,Q5b,Q6b,Q7b,Q8b,"
				L"Q1a,Q2a,Q3a,Q4a,Q5a,Q6a,Q7a,Q8a,"
				L"T1b,T2b,T3b,T4b,T5b,T6b,T7b,T8b,"
				L"T1a,T2a,T3a,T4a,T5a,T6a,T7a,T8a,"
				L"PRICE,COSTS,dQ0,dQ1,RESULT,ECONOMY,CURTIME,ID_FACT_INFO) "
				L"VALUES(%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,
				max-i);
			db.ExeSql(szBuffer);
		}
#pragma endregion 1
		break;
		case 2:
#pragma region 2

			//AfxMessageBox(L"case 2 1:");
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 2"
				L"(ID_OBJECT,PINIT, PLIM, TINIT, TLIM,"
				L"P0, DIN, L, COSTS, PRICE,"
				L"DV, VGP, ZINIT, ZLIM, RESULT," 
				L"CURTIME, ECONOMY, ID_PLAN_INFO) "
				L"VALUES(%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,
				max-i);

			db.ExeSql(szBuffer);
		}

		max=_wtoi(MAXIDFACTINFO);
		//AfxMessageBox(L"case 2 2:");
		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 2F"
				L"(ID_OBJECT,PINIT, PLIM, TINIT, TLIM,"
				L"P0, DIN, L, COSTS, PRICE,"
				L"DV, VGP, ZINIT, ZLIM, RESULT," 
				L"CURTIME, ECONOMY, ID_FACT_INFO) "
				L"VALUES(%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,
				max-i);

			db.ExeSql(szBuffer);
		}
		//AfxMessageBox(L"!!!");
		break;
#pragma endregion 2
	case 4:
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 4"
				L"(ID_OBJECT,P0, P1, T0, T1,"
				L"Q0,PRICE,DIN,L,COSTS,"
				L"CURTIME,DQ,V,Z0,Z1,RESULT,"
				L"ECONOMY,ID_PLAN_INFO) "
				L"VALUES(%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,
				max-i);
			//AfxMessageBox(szBuffer);
			db.ExeSql(szBuffer);
		}
		
		max=_wtoi(MAXIDFACTINFO);
		//AfxMessageBox(L"case 2 2:");
		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 4F"
				L"(ID_OBJECT,P0, P1, T0, T1,"
				L"Q0,PRICE,DIN,L,COSTS,"
				L"CURTIME,DQ,V,Z0,Z1,RESULT,"
				L"ECONOMY,ID_FACT_INFO) "
				L"VALUES(%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,
				max-i);

			db.ExeSql(szBuffer);
		}
		//AfxMessageBox(szBuffer);
		break;
	case 7:
#pragma region 7
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 7"
				L"(ID_OBJECT,"
				L"NMONTH,NPROM,KECTG,KLOAD,N,Kdepression,Q,COUNT,PRICE,COSTS,CURTIME,L,RESULT,ECONOMY,ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,
				max-i);
	
			db.ExeSql(szBuffer);
		}

		max=_wtoi(MAXIDFACTINFO);

		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 7F"
				L"(ID_OBJECT,"
				L"NMONTH,NPROM,KECTG,KLOAD,N,Kdepression,Q,COUNT,PRICE,COSTS,CURTIME,L,RESULT,ECONOMY,ID_FACT_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,
				max-i);
	
			db.ExeSql(szBuffer);
		}

#pragma endregion 7
		break;
	case 8:
#pragma region 8
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 8"
				L"(ID_OBJECT," 
				L"NMONTH, NREPAIR, KLOAD, NOM, Kdepression, QP, PRICE, COSTS, CURTIME, KECTG, LOAD, RESULT, ECONOMY, ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,
				max-i);
	
			db.ExeSql(szBuffer);
		}

		max=_wtoi(MAXIDFACTINFO);

		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 8F"
				L"(ID_OBJECT," 
				L"NMONTH, NREPAIR, KLOAD, NOM, Kdepression, QP, PRICE, COSTS, CURTIME, KECTG, LOAD, RESULT, ECONOMY, ID_FACT_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,
				max-i);
	
			db.ExeSql(szBuffer);
		}

#pragma endregion 8
		break;
	case 12:
#pragma region 12
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 12"
				L"(ID_OBJECT,"
				L"T,P,P0,P1,T0,"
				L"T1,QP,V1,V2,V3,"
				L"DQP,TSR0,TSR1,PSR0,PSR1,"
				L"Z0,Z1,Q0,Q1,n,"
				L"Q01,Q11,RESULT,PRICE,COSTS,"
				L"ECONOMY,CURTIME,ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,max-i);
	
			db.ExeSql(szBuffer);
		}

		max=_wtoi(MAXIDFACTINFO);

		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 12F"
				L"(ID_OBJECT,"
				L"T,P,P0,P1,T0,"
				L"T1,QP,V1,V2,V3,"
				L"DQP,TSR0,TSR1,PSR0,PSR1,"
				L"Z0,Z1,Q0,Q1,n,"
				L"Q01,Q11,RESULT,PRICE,COSTS,"
				L"ECONOMY,CURTIME,ID_FACT_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,max-i);
	
			db.ExeSql(szBuffer);
		}
#pragma endregion 12
		break;
	case 15:
#pragma region 15
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 15"
				L"(ID_OBJECT,"
				L"P0,D0,L0,T0,G,"
				L"KRESIST,SHEROH,PATM,PPG,TPROD,"
				L"NEMK,NOMNPROD,FACTNPROD,PRICE,COSTS, "
				L"CURTIME,POPSECH,KGIDR,QSTD,K_DP1P0, "
				L"K_DT1T0,K_DL1L0,KDD1D0,RESULT,ECONOMY, "
				L"ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				max-i);
			db.ExeSql(szBuffer);
		}

		max=_wtoi(MAXIDFACTINFO);

		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 15F"
				L"(ID_OBJECT,"
				L"P0,D0,L0,T0,G,"
				L"KRESIST,SHEROH,PATM,PPG,TPROD,"
				L"NEMK,NOMNPROD,FACTNPROD,PRICE,COSTS, "
				L"CURTIME,POPSECH,KGIDR,QSTD,K_DP1P0, "
				L"K_DT1T0,K_DL1L0,KDD1D0,RESULT,ECONOMY, "
				L"ID_FACT_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				max-i);
			db.ExeSql(szBuffer);
		}
		break;
#pragma endregion 15
	case 17:
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 17"
				L"(ID_OBJECT,"
				L"BTMG, QNMG, NFMG, NMG, TMG, BTBG, QNBG, NFBG, NBG, TBG, PRICE, COSTS, CURTIME, VN0, VN1, VE, ECONOMY, ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,0,0,
				max-i);
	
			db.ExeSql(szBuffer);
		}

		max=_wtoi(MAXIDFACTINFO);

		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 17F"
				L"(ID_OBJECT,"
				L"BTMG, QNMG, NFMG, NMG, TMG, BTBG, QNBG, NFBG, NBG, TBG, PRICE, COSTS, CURTIME, VN0, VN1, VE, ECONOMY, ID_FACT_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,0,0,
				max-i);
	
			db.ExeSql(szBuffer);
		}
		break;
	case 20:
		
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 20"
				L"(ID_OBJECT,"
				L"NZAM,NUST,T,PRICE,COSTS,CURTIME,DW0,N,RESULT,ECONOMY,"
				L"ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				max-i);
	
			//AfxMessageBox(szBuffer);

			db.ExeSql(szBuffer);
		}

		//AfxMessageBox(L"20");

		max=_wtoi(MAXIDFACTINFO);

		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 20F"
				L"(ID_OBJECT,"
				L"NZAM,NUST,T,PRICE,COSTS,CURTIME,DW0,N,RESULT,ECONOMY,"
				L"ID_FACT_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				max-i);
	
			db.ExeSql(szBuffer);
		}
		break;
	case 23:
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 23"
				L"(ID_OBJECT,"
				L"DPIDLE,T,U,UNOM,N,PRICE,CURCOSTS,CURTIME,RESULT,ECONOMY,"
				L"ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,%d,%d,"
				L"%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,
				max-i);
	
			//AfxMessageBox(szBuffer);

			db.ExeSql(szBuffer);
		}

		//AfxMessageBox(L"20");

		max=_wtoi(MAXIDFACTINFO);

		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 23F"
				L"(ID_OBJECT,"
				L"DPIDLE,T,U,UNOM,N,PRICE,CURCOSTS,CURTIME,RESULT,ECONOMY,"
				L"ID_FACT_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,%d,%d,"
				L"%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,
				max-i);
	
			db.ExeSql(szBuffer);
		}
		break;
	case 25:
#pragma region 25
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 25"
				L"(ID_OBJECT,"
				L"N0,NSD,KPD,PRICE,COSTS,"
				L"DW1,RESULT,ECONOMY,CURTIME,"
				L"ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,
				max-i);
			db.ExeSql(szBuffer);
		}

		max=_wtoi(MAXIDFACTINFO);

		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 25F"
				L"(ID_OBJECT,"
				L"N0,NSD,KPD,PRICE,COSTS,"
				L"DW1,RESULT,ECONOMY,CURTIME,"
				L"ID_FACT_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,
				max-i);
			db.ExeSql(szBuffer);
		}
#pragma endregion 25
		break;
	case 26:
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 26"
				L"(ID_OBJECT,"
				L"N0,T,KPD,K,PRICE,COSTS,"
				L"CURTIME,RESULT,ECONOMY,"
				L"ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,%d,"
				L"%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,0,
				0,0,0,
				max-i);
			db.ExeSql(szBuffer);
		}

		max=_wtoi(MAXIDFACTINFO);

		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 26F"
				L"(ID_OBJECT,"
				L"N0,T,KPD,K,PRICE,COSTS,"
				L"CURTIME,RESULT,ECONOMY,"
				L"ID_FACT_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,%d,"
				L"%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,0,
				0,0,0,
				max-i);
			db.ExeSql(szBuffer);
		}
		break;
	case 27:
#pragma region 27
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 27"
				L"(ID_OBJECT,"
				L"NAVO,NAVOMAX,DTINOUT,DTMAX,b,"
				L"N,T,PRICE,COSTS,CURTIME,"
				L"KLOADNH,KLOADND,DRES,RESULT,ECONOMY,"
				L"ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				max-i);
			//AfxMessageBox(szBuffer);

			db.ExeSql(szBuffer);
		}

		max=_wtoi(MAXIDFACTINFO);

		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 27F"
				L"(ID_OBJECT,"
				L"NAVO,NAVOMAX,DTINOUT,DTMAX,b,"
				L"N,T,PRICE,COSTS,CURTIME,"
				L"KLOADNH,KLOADND,DRES,RESULT,ECONOMY,"
				L"ID_FACT_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				max-i);
			db.ExeSql(szBuffer);
		}
#pragma endregion 27
		break;
	case 38:
#pragma region 38
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 38"
				L"(ID_OBJECT,"
				L"EKRN,WKC,TGANGLEF1,TGANGLEF2,PRICE,"
				L"COSTS,RESULT,ECONOMY,CURTIME,"
				L"ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,
				max-i);
			//AfxMessageBox(szBuffer);
			db.ExeSql(szBuffer);
		}

		max=_wtoi(MAXIDFACTINFO);

		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 38F"
				L"(ID_OBJECT,"
				L"EKRN,WKC,TGANGLEF1,TGANGLEF2,PRICE,"
				L"COSTS,RESULT,ECONOMY,CURTIME,"
				L"ID_FACT_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,
				max-i);
			//AfxMessageBox(szBuffer);
			db.ExeSql(szBuffer);
		}
#pragma endregion 38
		break;
	case 42:
#pragma region 42
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 42"
				L"(ID_OBJECT,"
				L"KT,S,TAIR,TMAT,NTEP,"
				L"PRICE,COSTS,CURTIME,RESULT,ECONOMY,"
				L"ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				max-i);
			db.ExeSql(szBuffer);
		}

		max=_wtoi(MAXIDFACTINFO);

		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 42F"
				L"(ID_OBJECT,"
				L"KT,S,TAIR,TMAT,NTEP,"
				L"PRICE,COSTS,CURTIME,RESULT,ECONOMY,"
				L"ID_FACT_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				max-i);
			db.ExeSql(szBuffer);
		}
#pragma endregion 42
		break;
	case 43:
		#pragma region 43
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 43"
				L"(ID_OBJECT,"
				L"QUT,QRN,KPDK,NADD,KPDGTA,TGTA,KLOAD,DPUT,G,TEMP,PRICE,CURCOSTS,CURTIME,Q,RASH,NADD2,QGTAI,QGTA,QK,RESULT,ECONOMY,"
				L"ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,0,
				max-i);
			db.ExeSql(szBuffer);
		}

		max=_wtoi(MAXIDFACTINFO);

		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 43F"
				L"(ID_OBJECT,"
				L"QUT,QRN,KPDK,NADD,KPDGTA,TGTA,KLOAD,DPUT,G,TEMP,PRICE,CURCOSTS,CURTIME,Q,RASH,NADD2,QGTAI,QGTA,QK,RESULT,ECONOMY,"
				L"ID_FACT_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,0,
				max-i);
			db.ExeSql(szBuffer);
		}
#pragma endregion 43
		break;
	case 48:
#pragma region 48
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 48"
				L"(ID_OBJECT,"
				L"QFYEAR,TINT,THT,TEXTS,QRN,"
				L"KPD,PRICE,COSTS,RESULT,ECONOMY,"
				L"CURTIME,"				
				L"ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,
				max-i);
			db.ExeSql(szBuffer);
		}

		max=_wtoi(MAXIDFACTINFO);

		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 48F"
				L"(ID_OBJECT,"
				L"QFYEAR,TINT,THT,TEXTS,QRN,"
				L"KPD,PRICE,COSTS,RESULT,ECONOMY,"
				L"CURTIME,"				
				L"ID_FACT_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,
				max-i);
			db.ExeSql(szBuffer);
		}

#pragma endregion 48
		break;
		case 481:
#pragma region 481
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 481"
				L"(ID_OBJECT,"
				L"QFYEAR,TINT,THT,TEXTS,QRN,"
				L"KPD,PRICE,COSTS,RESULT,ECONOMY,"
				L"CURTIME,"				
				L"ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,
				max-i);
			db.ExeSql(szBuffer);
		}

		max=_wtoi(MAXIDFACTINFO);

		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 481F"
				L"(ID_OBJECT,"
				L"QFYEAR,TINT,THT,TEXTS,QRN,"
				L"KPD,PRICE,COSTS,RESULT,ECONOMY,"
				L"CURTIME,"				
				L"ID_FACT_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,
				max-i);
			db.ExeSql(szBuffer);
		}

#pragma endregion 481
		break;
		case 431:
		#pragma region 431
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 431"
				L"(ID_OBJECT,"
				L"QUT,QRN,KPDK,NADD,KPDGTA,TGTA,KLOAD,DPUT,G,TEMP,PRICE,CURCOSTS,CURTIME,Q,RASH,NADD2,QGTAI,QGTA,QK,RESULT,ECONOMY,"
				L"ID_PLAN_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,0,
				max-i);
			db.ExeSql(szBuffer);
		}

		max=_wtoi(MAXIDFACTINFO);

		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 431F"
				L"(ID_OBJECT,"
				L"QUT,QRN,KPDK,NADD,KPDGTA,TGTA,KLOAD,DPUT,G,TEMP,PRICE,CURCOSTS,CURTIME,Q,RASH,NADD2,QGTAI,QGTA,QK,RESULT,ECONOMY,"
				L"ID_FACT_INFO) "
				L"VALUES(%s,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,"
				L"%d,%d,%d,%d,%d,%d,"
				L"%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,0,
				max-i);
			db.ExeSql(szBuffer);
		}
#pragma endregion 431
		break;
	case 101:
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 101"
				L"(ID_OBJECT,P0, P1, T0, T1,"
				L"Q0,PRICE,DIN,L,COSTS,"
				L"CURTIME,DQ,V,Z0,Z1,RESULT,"
				L"ECONOMY,ID_PLAN_INFO) "
				L"VALUES(%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,
				max-i);
			//AfxMessageBox(szBuffer);
			db.ExeSql(szBuffer);
		}
		
		max=_wtoi(MAXIDFACTINFO);
		//AfxMessageBox(L"case 2 2:");
		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 101F"
				L"(ID_OBJECT,P0, P1, T0, T1,"
				L"Q0,PRICE,DIN,L,COSTS,"
				L"CURTIME,DQ,V,Z0,Z1,RESULT,"
				L"ECONOMY,ID_FACT_INFO) "
				L"VALUES(%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,
				0,
				max-i);

			db.ExeSql(szBuffer);
		}
		//AfxMessageBox(szBuffer);
		break;

	case 81:
		for(int i=3;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 81"
				L"(ID_OBJECT,T, NNOM, NNOM0, NNOM1,"
				L"QRN,KPD,KEF,KNE,KNE1,"
				L"KKR,Q,PRICE,COSTS,CURTIME,RESULT,"
				L"ECONOMY,ID_PLAN_INFO) "
				L"VALUES(%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,
				0,0,0,0,0,
				0,0,0,0,0,0,
				0,
				max-i);
			//AfxMessageBox(szBuffer);
			db.ExeSql(szBuffer);
		}
		
		max=_wtoi(MAXIDFACTINFO);
		//AfxMessageBox(L"case 2 2:");
		for(int i=11;i>-1;i--)
		{
			szBuffer.Format(
				L"INSERT INTO 81F"
				L"(ID_OBJECT,NE0, NEF0, NEF1, KNE,"
				L"KNE1,QTG,KKR,PRICE,COSTS,"
				L"CURTIME,RESULT,ECONOMY,ID_FACT_INFO) "
				L"VALUES(%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
				ID_OBJECT,
				0,0,0,0,
				0,0,0,0,0,
				0,0,0,
				max-i);

			db.ExeSql(szBuffer);
		}
		break;

	default:
		break;
	}
}
