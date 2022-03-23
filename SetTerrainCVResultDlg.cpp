// SetTerrainCVResultDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetTerrainCVResultDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetTerrainCVResultDlg dialog


CSetTerrainCVResultDlg::CSetTerrainCVResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetTerrainCVResultDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetTerrainCVResultDlg)
	m_dMaxValue = 0.0;
	m_dMeanValue = 0.0;
	m_dMinValue = 0.0;
	m_dWindowRange = 0.0;
	m_iWindowSize = 0;
	//}}AFX_DATA_INIT
}


void CSetTerrainCVResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetTerrainCVResultDlg)
	DDX_Text(pDX, IDC_EDIT_MAX, m_dMaxValue);
	DDX_Text(pDX, IDC_EDIT_MEAN, m_dMeanValue);
	DDX_Text(pDX, IDC_EDIT_MIN, m_dMinValue);
	DDX_Text(pDX, IDC_EDIT_WINDOWRANGE, m_dWindowRange);
	DDX_Text(pDX, IDC_EDIT_WINDOWSIZE, m_iWindowSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetTerrainCVResultDlg, CDialog)
	//{{AFX_MSG_MAP(CSetTerrainCVResultDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetTerrainCVResultDlg message handlers
