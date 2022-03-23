// SetFractalResultDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetFractalResultDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetFractalResultDlg dialog


CSetFractalResultDlg::CSetFractalResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetFractalResultDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetFractalResultDlg)
	m_szPathName = _T("");
	m_dResult1 = 0.0;
	m_dResult2 = 0.0;
	//}}AFX_DATA_INIT
}


void CSetFractalResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetFractalResultDlg)
	DDX_Text(pDX, IDC_EDIT_PATHNAME, m_szPathName);
	DDX_Text(pDX, IDC_EDIT_RESULT_1, m_dResult1);
	DDX_Text(pDX, IDC_EDIT_RESULT_2, m_dResult2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetFractalResultDlg, CDialog)
	//{{AFX_MSG_MAP(CSetFractalResultDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetFractalResultDlg message handlers
