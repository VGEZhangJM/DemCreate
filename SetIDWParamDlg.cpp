// SetIDWParamDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetIDWParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetIDWParamDlg dialog


CSetIDWParamDlg::CSetIDWParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetIDWParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetIDWParamDlg)
	m_dT = 0.0;
	m_iU = 1;
	//}}AFX_DATA_INIT
}


void CSetIDWParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetIDWParamDlg)
	DDX_Text(pDX, IDC_EDIT_T, m_dT);
	DDX_Text(pDX, IDC_EDIT_U, m_iU);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetIDWParamDlg, CDialog)
	//{{AFX_MSG_MAP(CSetIDWParamDlg)
	ON_EN_CHANGE(IDC_EDIT_U, OnChangeEditU)
	ON_EN_CHANGE(IDC_EDIT_T, OnChangeEditT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetIDWParamDlg message handlers

void CSetIDWParamDlg::OnChangeEditU() 
{
	UpdateData(TRUE);	
}

void CSetIDWParamDlg::OnChangeEditT() 
{
	UpdateData(TRUE);	
}

BOOL CSetIDWParamDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
