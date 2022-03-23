// SetDimensionAnalyseDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetDimensionAnalyseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetDimensionAnalyseDlg dialog


CSetDimensionAnalyseDlg::CSetDimensionAnalyseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDimensionAnalyseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetDimensionAnalyseDlg)
	m_szPathName = _T("");
	m_iMode = 0;
	m_iFieldNum = 38;
	m_iDelims = 0;
	//}}AFX_DATA_INIT
}


void CSetDimensionAnalyseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetDimensionAnalyseDlg)
	DDX_Text(pDX, IDC_EDIT_PATHNAME, m_szPathName);
	DDX_Radio(pDX, IDC_RADIO_MODE_1, m_iMode);
	DDX_Text(pDX, IDC_EDIT_FIELDNUM, m_iFieldNum);
	DDX_Radio(pDX, IDC_RADIO_DELIMS_1, m_iDelims);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetDimensionAnalyseDlg, CDialog)
	//{{AFX_MSG_MAP(CSetDimensionAnalyseDlg)
	ON_BN_CLICKED(IDC_BUTTON_PATHNAME, OnButtonPathname)
	ON_EN_CHANGE(IDC_EDIT_FIELDNUM, OnChangeEditFieldnum)
	ON_BN_CLICKED(IDC_RADIO_MODE_1, OnRadioMode1)
	ON_BN_CLICKED(IDC_RADIO_MODE_2, OnRadioMode2)
	ON_BN_CLICKED(IDC_RADIO_MODE_3, OnRadioMode3)
	ON_BN_CLICKED(IDC_RADIO_MODE_4, OnRadioMode4)
	ON_BN_CLICKED(IDC_RADIO_DELIMS_1, OnRadioDelims1)
	ON_BN_CLICKED(IDC_RADIO_DELIMS_2, OnRadioDelims2)
	ON_BN_CLICKED(IDC_RADIO_DELIMS_3, OnRadioDelims3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetDimensionAnalyseDlg message handlers

void CSetDimensionAnalyseDlg::OnButtonPathname() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"文本数据文件(*.txt)|*.txt|所有数据文件(*.*)|*.*||");
	dlg.m_ofn.lpstrTitle="打开文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szPathName	= dlg.GetPathName();
	}
	else
		return;	

	UpdateData(FALSE);
}

void CSetDimensionAnalyseDlg::OnChangeEditFieldnum() 
{
	UpdateData(TRUE);
}

void CSetDimensionAnalyseDlg::OnRadioMode1() 
{
	m_iMode	= 0;
}

void CSetDimensionAnalyseDlg::OnRadioMode2() 
{
	m_iMode	= 1;
}

void CSetDimensionAnalyseDlg::OnRadioMode3() 
{
	m_iMode	= 2;
}

void CSetDimensionAnalyseDlg::OnRadioMode4() 
{
	m_iMode	= 3;
}

void CSetDimensionAnalyseDlg::OnOK() 
{
	if(m_szPathName.IsEmpty())	return;
	
	CDialog::OnOK();
}

void CSetDimensionAnalyseDlg::OnRadioDelims1() 
{
	m_iDelims	= 0;	
}

void CSetDimensionAnalyseDlg::OnRadioDelims2() 
{
	m_iDelims	= 1;	
}

void CSetDimensionAnalyseDlg::OnRadioDelims3() 
{
	m_iDelims	= 2;	
}
