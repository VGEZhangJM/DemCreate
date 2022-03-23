// SetTerrainRelifResultDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetTerrainRelifResultDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetTerrainRelifResultDlg dialog


CSetTerrainRelifResultDlg::CSetTerrainRelifResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetTerrainRelifResultDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetTerrainRelifResultDlg)
	m_iWindowSize = 0;
	m_dWindowRange = 0.0;
	m_dMaxValue = 0.0;
	m_dMinValue = 0.0;
	m_dMeanValue = 0.0;
	//}}AFX_DATA_INIT
}


void CSetTerrainRelifResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetTerrainRelifResultDlg)
	DDX_Text(pDX, IDC_EDIT_WINDOWSIZE, m_iWindowSize);
	DDX_Text(pDX, IDC_EDIT_WINDOWRANGE, m_dWindowRange);
	DDX_Text(pDX, IDC_EDIT_MAX, m_dMaxValue);
	DDX_Text(pDX, IDC_EDIT_MIN, m_dMinValue);
	DDX_Text(pDX, IDC_EDIT_MEAN, m_dMeanValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetTerrainRelifResultDlg, CDialog)
	//{{AFX_MSG_MAP(CSetTerrainRelifResultDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetTerrainRelifResultDlg message handlers
