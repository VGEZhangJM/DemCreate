// SetImportTxtDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetImportTxtDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetImportTxtDlg dialog


CSetImportTxtDlg::CSetImportTxtDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetImportTxtDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetImportTxtDlg)
	m_szTxtPathName = _T("");
	m_iX = 1;
	m_iY = 2;
	m_iZ = 3;
	m_iNumOfAttrib = 0;
	m_bFirstLine = FALSE;
	m_iSeparator = 0;
	//}}AFX_DATA_INIT
}


void CSetImportTxtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetImportTxtDlg)
	DDX_Text(pDX, IDC_EDIT_TXT_PATHNAME, m_szTxtPathName);
	DDX_Text(pDX, IDC_EDIT_X, m_iX);
	DDX_Text(pDX, IDC_EDIT_Y, m_iY);
	DDX_Text(pDX, IDC_EDIT_Z, m_iZ);
	DDX_Text(pDX, IDC_EDIT_NUMATTRIB, m_iNumOfAttrib);
	DDX_Check(pDX, IDC_CHECK_FIRSTLINE, m_bFirstLine);
	DDX_Radio(pDX, IDC_RADIO_1, m_iSeparator);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetImportTxtDlg, CDialog)
	//{{AFX_MSG_MAP(CSetImportTxtDlg)
	ON_EN_CHANGE(IDC_EDIT_X, OnChangeEditX)
	ON_EN_CHANGE(IDC_EDIT_Y, OnChangeEditY)
	ON_EN_CHANGE(IDC_EDIT_Z, OnChangeEditZ)
	ON_EN_CHANGE(IDC_EDIT_NUMATTRIB, OnChangeEditNumattrib)
	ON_BN_CLICKED(IDC_BUTTON_TXT, OnButtonTxt)
	ON_BN_CLICKED(IDC_CHECK_FIRSTLINE, OnCheckFirstline)
	ON_BN_CLICKED(IDC_RADIO_1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO_2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO_3, OnRadio3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetImportTxtDlg message handlers
BOOL CSetImportTxtDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetImportTxtDlg::OnChangeEditX() 
{
	UpdateData(TRUE);	
}

void CSetImportTxtDlg::OnChangeEditY() 
{
	UpdateData(TRUE);	
}

void CSetImportTxtDlg::OnChangeEditZ() 
{
	UpdateData(TRUE);	
}

void CSetImportTxtDlg::OnChangeEditNumattrib() 
{
	UpdateData(TRUE);	
}

void CSetImportTxtDlg::OnButtonTxt() 
{
	// TODO: Add your control notification handler code here
	
}

void CSetImportTxtDlg::OnCheckFirstline() 
{
	// TODO: Add your control notification handler code here
	
}

void CSetImportTxtDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	
}

void CSetImportTxtDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	
}

void CSetImportTxtDlg::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	
}

