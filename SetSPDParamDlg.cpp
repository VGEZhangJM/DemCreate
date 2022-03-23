// SetSPDParamDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetSPDParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetSPDParamDlg dialog


CSetSPDParamDlg::CSetSPDParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetSPDParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetSPDParamDlg)
	m_dT = 0.0;
	m_iU = 0;
	//}}AFX_DATA_INIT
}


void CSetSPDParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetSPDParamDlg)
	DDX_Text(pDX, IDC_EDIT_T, m_dT);
	DDX_Text(pDX, IDC_EDIT_U, m_iU);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetSPDParamDlg, CDialog)
	//{{AFX_MSG_MAP(CSetSPDParamDlg)
	ON_EN_CHANGE(IDC_EDIT_U, OnChangeEditU)
	ON_EN_CHANGE(IDC_EDIT_T, OnChangeEditT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetSPDParamDlg message handlers

void CSetSPDParamDlg::OnChangeEditU() 
{
	UpdateData(TRUE);
}

void CSetSPDParamDlg::OnChangeEditT() 
{
	UpdateData(TRUE);
}
