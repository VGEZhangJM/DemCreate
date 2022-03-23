// SetCALCLSD_Dlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetCALCLSD_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetCALCLSD_Dlg dialog


CSetCALCLSD_Dlg::CSetCALCLSD_Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetCALCLSD_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetCALCLSD_Dlg)
	m_dD = 0.0;
	m_dValue = 0.0;
	m_dValue2 = 0.0;
	m_dValue3 = 0.0;
	m_dValue4 = 0.0;
	m_dValue5 = 0.0;
	//}}AFX_DATA_INIT
}


void CSetCALCLSD_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetCALCLSD_Dlg)
	DDX_Text(pDX, IDC_EDIT_D, m_dD);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_dValue);
	DDX_Text(pDX, IDC_EDIT_VALUE2, m_dValue2);
	DDX_Text(pDX, IDC_EDIT_VALUE3, m_dValue3);
	DDX_Text(pDX, IDC_EDIT_VALUE4, m_dValue4);
	DDX_Text(pDX, IDC_EDIT_VALUE5, m_dValue5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetCALCLSD_Dlg, CDialog)
	//{{AFX_MSG_MAP(CSetCALCLSD_Dlg)
	ON_EN_CHANGE(IDC_EDIT_D, OnChangeEditD)
	ON_BN_CLICKED(ID_CALC, OnCalc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetCALCLSD_Dlg message handlers

void CSetCALCLSD_Dlg::OnChangeEditD() 
{
	UpdateData(TRUE);
}

BOOL CSetCALCLSD_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetCALCLSD_Dlg::OnCalc() 
{
	m_dValue	= 1/(1+exp(1.5*(m_dD-4)));

	m_dValue2	= 1/(1+exp(0.3*(m_dD-20)));

	m_dValue3	= 1/(1+exp(0.15*(m_dD-40)));

	
	CString	szInfo;
	char	chInfo[32];
	szInfo.Format("%.2lf",m_dValue);
	strcpy(chInfo,szInfo);
	m_dValue	= atof(chInfo);

	szInfo.Format("%.2lf",m_dValue2);
	strcpy(chInfo,szInfo);
	m_dValue2	= atof(chInfo);

	szInfo.Format("%.2lf",m_dValue3);
	strcpy(chInfo,szInfo);
	m_dValue3	= atof(chInfo);
	

	UpdateData(FALSE);
}
