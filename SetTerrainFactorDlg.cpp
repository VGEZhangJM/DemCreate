// SetTerrainFactorDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetTerrainFactorDlg.h"

#include <direct.h>
#include "WzdDirDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetTerrainFactorDlg dialog


CSetTerrainFactorDlg::CSetTerrainFactorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetTerrainFactorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetTerrainFactorDlg)
	m_szPathName = _T("");
	//}}AFX_DATA_INIT

	m_szWindowName	= "地形因子批处理窗口";
}


void CSetTerrainFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetTerrainFactorDlg)
	DDX_Text(pDX, IDC_PATHNAME, m_szPathName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetTerrainFactorDlg, CDialog)
	//{{AFX_MSG_MAP(CSetTerrainFactorDlg)
	ON_BN_CLICKED(IDC_BROWN, OnBrown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetTerrainFactorDlg message handlers

void CSetTerrainFactorDlg::OnBrown() 
{
	CWzdDirDlg dlg;
	
	char chBuffer[MAX_PATH];
	_getcwd(chBuffer,MAX_PATH);
	CString szDir	= dlg.GetDirectory(this,chBuffer);		

	if(szDir.IsEmpty())		return;

	//int		nLocate;

	//nLocate	= -1;
	//nLocate	= szDir.ReverseFind('\\');
	
	//if(nLocate == -1)		return;

	//szFilePath	= szDir;
	//szFileName	= szDir.Right((szDir.GetLength() - nLocate - 1));
	
	m_szPathName		= szDir;

	UpdateData(FALSE);		
}

BOOL CSetTerrainFactorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//
	SetWindowText(m_szWindowName);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
