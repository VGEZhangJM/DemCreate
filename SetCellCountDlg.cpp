// SetCellCountDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetCellCountDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetCellCountDlg dialog


CSetCellCountDlg::CSetCellCountDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetCellCountDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetCellCountDlg)
	m_iNumOfX = 0;
	m_iNumOfY = 0;
	//}}AFX_DATA_INIT
}


void CSetCellCountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetCellCountDlg)
	DDX_Text(pDX, IDC_EDIT_X, m_iNumOfX);
	DDX_Text(pDX, IDC_EDIT_Y, m_iNumOfY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetCellCountDlg, CDialog)
	//{{AFX_MSG_MAP(CSetCellCountDlg)
	ON_EN_CHANGE(IDC_EDIT_X, OnChangeEditX)
	ON_EN_CHANGE(IDC_EDIT_Y, OnChangeEditY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetCellCountDlg message handlers

void CSetCellCountDlg::OnChangeEditX() 
{
	UpdateData(TRUE);	
}

void CSetCellCountDlg::OnChangeEditY() 
{
	UpdateData(TRUE);	
}
