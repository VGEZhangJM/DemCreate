// SetTerrainCVDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetTerrainCVDlg.h"

#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetTerrainCVDlg dialog


CSetTerrainCVDlg::CSetTerrainCVDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetTerrainCVDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetTerrainCVDlg)
	m_szDemName = _T("");
	m_iDemNumX = 0;
	m_iDemNumY = 0;
	m_dDemSizeX = 0.0;
	m_dDemSizeY = 0.0;
	m_dWindowRange = 0.0;
	m_iWindowSize = 1;
	//}}AFX_DATA_INIT
}


void CSetTerrainCVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetTerrainCVDlg)
	DDX_Text(pDX, IDC_EDIT_DEMNAME, m_szDemName);
	DDX_Text(pDX, IDC_EDIT_NUM_X, m_iDemNumX);
	DDX_Text(pDX, IDC_EDIT_NUM_Y, m_iDemNumY);
	DDX_Text(pDX, IDC_EDIT_SIZE_X, m_dDemSizeX);
	DDX_Text(pDX, IDC_EDIT_SIZE_Y, m_dDemSizeY);
	DDX_Text(pDX, IDC_EDIT_WINDOWRANGE, m_dWindowRange);
	DDX_Text(pDX, IDC_EDIT_WINDOWSIZE, m_iWindowSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetTerrainCVDlg, CDialog)
	//{{AFX_MSG_MAP(CSetTerrainCVDlg)
	ON_EN_CHANGE(IDC_EDIT_WINDOWSIZE, OnChangeEditWindowsize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetTerrainCVDlg message handlers

BOOL CSetTerrainCVDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//
	m_dWindowRange	= m_dDemSizeX*m_dDemSizeY*(2*m_iWindowSize)*(2*m_iWindowSize);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetTerrainCVDlg::OnChangeEditWindowsize() 
{
	UpdateData(TRUE);	

	m_dWindowRange	= m_dDemSizeX*m_dDemSizeY*(2*m_iWindowSize)*(2*m_iWindowSize);

	UpdateData(FALSE);
}
