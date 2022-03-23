// SetLimitDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "SetLimitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetLimitDlg dialog


CSetLimitDlg::CSetLimitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetLimitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetLimitDlg)
	m_dLimit = 0.0;
	//}}AFX_DATA_INIT

	m_dLimit	= 0.5;
}


void CSetLimitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetLimitDlg)
	DDX_Text(pDX, IDC_EDIT_LIMIT, m_dLimit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetLimitDlg, CDialog)
	//{{AFX_MSG_MAP(CSetLimitDlg)
	ON_EN_CHANGE(IDC_EDIT_LIMIT, OnChangeEditLimit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetLimitDlg message handlers

void CSetLimitDlg::OnChangeEditLimit() 
{
	UpdateData(TRUE);	
}
