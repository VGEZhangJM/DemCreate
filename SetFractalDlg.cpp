// SetFractalDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetFractalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetFractalDlg dialog


CSetFractalDlg::CSetFractalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetFractalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetFractalDlg)
	m_dSizeX = 0.0;
	m_dSizeY = 0.0;
	m_iNumX = 0;
	m_iNumY = 0;
	m_dZScale = 1.0;
	m_dScale = 0.0;
	m_szDemName = _T("");
	//}}AFX_DATA_INIT
}


void CSetFractalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetFractalDlg)
	DDX_Control(pDX, IDC_LIST_SCALE, m_pListScale);
	DDX_Text(pDX, IDC_EDIT_SIZE_X, m_dSizeX);
	DDX_Text(pDX, IDC_EDIT_SIZE_Y, m_dSizeY);
	DDX_Text(pDX, IDC_EDIT_NUM_X, m_iNumX);
	DDX_Text(pDX, IDC_EDIT_NUM_Y, m_iNumY);
	DDX_Text(pDX, IDC_EDIT_ZSCALE, m_dZScale);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_dScale);
	DDX_Text(pDX, IDC_EDIT_DEMNAME, m_szDemName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetFractalDlg, CDialog)
	//{{AFX_MSG_MAP(CSetFractalDlg)
	ON_EN_CHANGE(IDC_EDIT_ZSCALE, OnChangeEditZscale)
	ON_EN_CHANGE(IDC_EDIT_SCALE, OnChangeEditScale)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetFractalDlg message handlers

BOOL CSetFractalDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//码尺数目
	m_iScaleCount	= 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetFractalDlg::OnChangeEditZscale() 
{
	UpdateData(TRUE);	
}

void CSetFractalDlg::OnChangeEditScale() 
{
	UpdateData(TRUE);	
}

void CSetFractalDlg::OnButtonAdd() 
{
	for(int i=0;i<m_iScaleCount;i++)
	{
		if(m_dScaleList[i] == m_dScale)	
		{
			MessageBox("已经存在，请重新输入","提示信息",MB_OK);
			return;
		}
	}

	m_dScaleList[m_iScaleCount]	= m_dScale;
	m_iScaleCount++;

	m_pListScale.ResetContent();
	for(i=0;i<m_iScaleCount;i++)
	{
		CString	szInfo;
		szInfo.Format("%6ld 种码尺为 %.2lf",i+1,m_dScaleList[i]);

		m_pListScale.AddString(szInfo);
	}

	UpdateData(FALSE);
}

void CSetFractalDlg::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	
}
