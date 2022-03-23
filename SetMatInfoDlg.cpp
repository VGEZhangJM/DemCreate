// SetMatInfoDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetMatInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetMatInfoDlg dialog


CSetMatInfoDlg::CSetMatInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetMatInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetMatInfoDlg)
	m_dLeft = 0.0;
	m_dRight = 0.0;
	m_dBottom = 0.0;
	m_dTop = 0.0;
	m_iMatWidth = 0;
	m_iMatHeight = 0;
	m_iWidth = 0;
	m_iHeight = 0;
	//}}AFX_DATA_INIT
}


void CSetMatInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetMatInfoDlg)
	DDX_Text(pDX, IDC_EDIT_LEFT, m_dLeft);
	DDX_Text(pDX, IDC_EDIT_RIGHT, m_dRight);
	DDX_Text(pDX, IDC_EDIT_BOTTOM, m_dBottom);
	DDX_Text(pDX, IDC_EDIT_TOP, m_dTop);
	DDX_Text(pDX, IDC_EDIT_MATWIDTH, m_iMatWidth);
	DDX_Text(pDX, IDC_EDIT_MATHEIGHT, m_iMatHeight);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_iWidth);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_iHeight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetMatInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CSetMatInfoDlg)
	ON_EN_CHANGE(IDC_EDIT_LEFT, OnChangeEditLeft)
	ON_EN_CHANGE(IDC_EDIT_RIGHT, OnChangeEditRight)
	ON_EN_CHANGE(IDC_EDIT_BOTTOM, OnChangeEditBottom)
	ON_EN_CHANGE(IDC_EDIT_TOP, OnChangeEditTop)
	ON_EN_CHANGE(IDC_EDIT_MATWIDTH, OnChangeEditMatwidth)
	ON_EN_CHANGE(IDC_EDIT_MATHEIGHT, OnChangeEditMatheight)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, OnChangeEditWidth)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, OnChangeEditHeight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetMatInfoDlg message handlers

void CSetMatInfoDlg::OnChangeEditLeft() 
{
	UpdateData(TRUE);	
}

void CSetMatInfoDlg::OnChangeEditRight() 
{
	UpdateData(TRUE);	
}

void CSetMatInfoDlg::OnChangeEditBottom() 
{
	UpdateData(TRUE);	
}

void CSetMatInfoDlg::OnChangeEditTop() 
{
	UpdateData(TRUE);	
}

void CSetMatInfoDlg::OnChangeEditMatwidth() 
{
	UpdateData(TRUE);	
}

void CSetMatInfoDlg::OnChangeEditMatheight() 
{
	UpdateData(TRUE);	
}

void CSetMatInfoDlg::OnChangeEditWidth() 
{
	UpdateData(TRUE);	
}

void CSetMatInfoDlg::OnChangeEditHeight() 
{
	UpdateData(TRUE);	
}
