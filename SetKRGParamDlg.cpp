// SetKRGParamDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetKRGParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetKRGParamDlg dialog


CSetKRGParamDlg::CSetKRGParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetKRGParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetKRGParamDlg)
	m_iModelType = -1;
	m_dA = 0.0;
	m_dC = 0.0;
	m_dC0 = 0.0;
	//}}AFX_DATA_INIT
}


void CSetKRGParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetKRGParamDlg)
	DDX_Radio(pDX, IDC_RADIO_QX, m_iModelType);
	DDX_Text(pDX, IDC_EDIT_A, m_dA);
	DDX_Text(pDX, IDC_EDIT_C, m_dC);
	DDX_Text(pDX, IDC_EDIT_C0, m_dC0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetKRGParamDlg, CDialog)
	//{{AFX_MSG_MAP(CSetKRGParamDlg)
	ON_EN_CHANGE(IDC_EDIT_C0, OnChangeEditC0)
	ON_EN_CHANGE(IDC_EDIT_C, OnChangeEditC)
	ON_EN_CHANGE(IDC_EDIT_A, OnChangeEditA)
	ON_BN_CLICKED(IDC_RADIO_QX, OnRadioQx)
	ON_BN_CLICKED(IDC_RADIO_ZS, OnRadioZs)
	ON_BN_CLICKED(IDC_RADIO_GS, OnRadioGs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetKRGParamDlg message handlers

void CSetKRGParamDlg::OnChangeEditC0() 
{
	UpdateData(TRUE);
}

void CSetKRGParamDlg::OnChangeEditC() 
{
	UpdateData(TRUE);
}

void CSetKRGParamDlg::OnChangeEditA() 
{
	UpdateData(TRUE);
}

void CSetKRGParamDlg::OnRadioQx() 
{
	m_iModelType	= 0;	
}

void CSetKRGParamDlg::OnRadioZs() 
{
	m_iModelType	= 1;	
}

void CSetKRGParamDlg::OnRadioGs() 
{
	m_iModelType	= 2;	
}
