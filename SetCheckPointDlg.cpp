// SetCheckPointDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetCheckPointDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetCheckPointDlg dialog


CSetCheckPointDlg::CSetCheckPointDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetCheckPointDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetCheckPointDlg)
	m_iPointNum = 0;
	//}}AFX_DATA_INIT
}


void CSetCheckPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetCheckPointDlg)
	DDX_Text(pDX, IDC_EDIT_NUM, m_iPointNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetCheckPointDlg, CDialog)
	//{{AFX_MSG_MAP(CSetCheckPointDlg)
	ON_EN_CHANGE(IDC_EDIT_NUM, OnChangeEditNum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetCheckPointDlg message handlers

void CSetCheckPointDlg::OnChangeEditNum() 
{
	UpdateData(TRUE);	
}

void CSetCheckPointDlg::OnOK() 
{
	if(m_iPointNum <= 0)
		return;
	
	CDialog::OnOK();
}
