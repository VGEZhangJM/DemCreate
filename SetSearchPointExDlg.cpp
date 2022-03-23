// SetSearchPointExDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetSearchPointExDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetSearchPointExDlg dialog


CSetSearchPointExDlg::CSetSearchPointExDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetSearchPointExDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetSearchPointExDlg)
	m_dX = 0.0;
	m_dY = 0.0;
	//}}AFX_DATA_INIT
}


void CSetSearchPointExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetSearchPointExDlg)
	DDX_Text(pDX, IDC_EDIT_X, m_dX);
	DDX_Text(pDX, IDC_EDIT_Y, m_dY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetSearchPointExDlg, CDialog)
	//{{AFX_MSG_MAP(CSetSearchPointExDlg)
	ON_EN_CHANGE(IDC_EDIT_X, OnChangeEditX)
	ON_EN_CHANGE(IDC_EDIT_Y, OnChangeEditY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetSearchPointExDlg message handlers

void CSetSearchPointExDlg::OnChangeEditX() 
{
	UpdateData(TRUE);
}

void CSetSearchPointExDlg::OnChangeEditY() 
{
	UpdateData(TRUE);
}
