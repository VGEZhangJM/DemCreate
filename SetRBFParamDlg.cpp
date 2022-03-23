// SetRBFParamDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetRBFParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetRBFParamDlg dialog


CSetRBFParamDlg::CSetRBFParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetRBFParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetRBFParamDlg)
	m_iModelType = -1;
	m_dC = 0.0;
	//}}AFX_DATA_INIT
}


void CSetRBFParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetRBFParamDlg)
	DDX_Radio(pDX, IDC_RADIO_MQF, m_iModelType);
	DDX_Text(pDX, IDC_EDIT_C, m_dC);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetRBFParamDlg, CDialog)
	//{{AFX_MSG_MAP(CSetRBFParamDlg)
	ON_EN_CHANGE(IDC_EDIT_C, OnChangeEditC)
	ON_BN_CLICKED(IDC_RADIO_MQF, OnRadioMqf)
	ON_BN_CLICKED(IDC_RADIO_IQMF, OnRadioIqmf)
	ON_BN_CLICKED(IDC_RADIO_TPSF, OnRadioTpsf)
	ON_BN_CLICKED(IDC_RADIO_MLF, OnRadioMlf)
	ON_BN_CLICKED(IDC_RADIO_NCSF, OnRadioNcsf)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetRBFParamDlg message handlers

void CSetRBFParamDlg::OnChangeEditC() 
{
	UpdateData(TRUE);
}

void CSetRBFParamDlg::OnRadioMqf() 
{
	m_iModelType	= 0;	
}

void CSetRBFParamDlg::OnRadioIqmf() 
{
	m_iModelType	= 1;	
}

void CSetRBFParamDlg::OnRadioTpsf() 
{
	m_iModelType	= 2;	
}

void CSetRBFParamDlg::OnRadioMlf() 
{
	m_iModelType	= 3;	
}

void CSetRBFParamDlg::OnRadioNcsf() 
{
	m_iModelType	= 4;	
}

void CSetRBFParamDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
