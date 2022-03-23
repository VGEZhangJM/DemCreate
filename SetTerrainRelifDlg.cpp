// SetTerrainRelifDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetTerrainRelifDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetTerrainRelifDlg dialog


CSetTerrainRelifDlg::CSetTerrainRelifDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetTerrainRelifDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetTerrainRelifDlg)
	m_szDemName = _T("");
	m_iDemNumX = 0;
	m_iDemNumY = 0;
	m_dDemSizeX = 0.0;
	m_dDemSizeY = 0.0;
	m_iWindowSize = 1;
	m_dWindowRange = 0.0;
	//}}AFX_DATA_INIT
}


void CSetTerrainRelifDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetTerrainRelifDlg)
	DDX_Text(pDX, IDC_EDIT_DEMNAME, m_szDemName);
	DDX_Text(pDX, IDC_EDIT_NUM_X, m_iDemNumX);
	DDX_Text(pDX, IDC_EDIT_NUM_Y, m_iDemNumY);
	DDX_Text(pDX, IDC_EDIT_SIZE_X, m_dDemSizeX);
	DDX_Text(pDX, IDC_EDIT_SIZE_Y, m_dDemSizeY);
	DDX_Text(pDX, IDC_EDIT_WINDOWSIZE, m_iWindowSize);
	DDX_Text(pDX, IDC_EDIT_WINDOWRANGE, m_dWindowRange);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetTerrainRelifDlg, CDialog)
	//{{AFX_MSG_MAP(CSetTerrainRelifDlg)
	ON_EN_CHANGE(IDC_EDIT_WINDOWSIZE, OnChangeEditWindowsize)
	ON_EN_CHANGE(IDC_EDIT_WINDOWRANGE, OnChangeEditWindowrange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetTerrainRelifDlg message handlers

void CSetTerrainRelifDlg::OnChangeEditWindowsize() 
{
	UpdateData(TRUE);	

	m_dWindowRange	= m_dDemSizeX*m_dDemSizeY*(2*m_iWindowSize)*(2*m_iWindowSize);

	UpdateData(FALSE);
}

void CSetTerrainRelifDlg::OnChangeEditWindowrange() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

BOOL CSetTerrainRelifDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//
	m_dWindowRange	= m_dDemSizeX*m_dDemSizeY*(2*m_iWindowSize)*(2*m_iWindowSize);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
