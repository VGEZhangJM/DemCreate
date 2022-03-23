// SetSearchPointDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetSearchPointDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetSearchPointDlg dialog

BOOL CSetSearchPointDlg::Create(CWnd* pView)
{
	m_pMainView	= pView;

	return CDialog::Create(CSetSearchPointDlg::IDD);
}

//
void CSetSearchPointDlg::MoveWindowPos()
{
	if(m_pMainView != NULL)
	{
		RECT	rcRect;
		m_pMainView->GetClientRect(&rcRect);
		
		RECT	winRect;
		GetWindowRect(&winRect);

		m_pMainView->ClientToScreen(&rcRect);

		MoveWindow(rcRect.right-(winRect.right-winRect.left), (rcRect.bottom)-(winRect.bottom-winRect.top), winRect.right-winRect.left, winRect.bottom-winRect.top);
	}
}

CSetSearchPointDlg::CSetSearchPointDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetSearchPointDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetSearchPointDlg)
	m_dX = 0.0;
	m_dY = 0.0;
	m_dPP = 0.0;
	m_dMS = 0.0;
	m_dDH = 0.0;
	m_iNum = 5;
	//}}AFX_DATA_INIT

	m_pLocalTerrain	= NULL;

	m_pWorkspace	= NULL;
}


void CSetSearchPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetSearchPointDlg)
	DDX_Control(pDX, IDC_LIST_COORD, m_pCoord);
	DDX_Text(pDX, IDC_EDIT_X, m_dX);
	DDX_Text(pDX, IDC_EDIT_Y, m_dY);
	DDX_Text(pDX, IDC_EDIT_POINTPATTERN, m_dPP);
	DDX_Text(pDX, IDC_EDIT_SLOPE, m_dMS);
	DDX_Text(pDX, IDC_EDIT_DH, m_dDH);
	DDX_Text(pDX, IDC_EDIT_NUM, m_iNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetSearchPointDlg, CDialog)
	//{{AFX_MSG_MAP(CSetSearchPointDlg)
	ON_EN_CHANGE(IDC_EDIT_X, OnChangeEditX)
	ON_EN_CHANGE(IDC_EDIT_Y, OnChangeEditY)
	ON_EN_CHANGE(IDC_EDIT_POINTPATTERN, OnChangeEditPointpattern)
	ON_EN_CHANGE(IDC_EDIT_SLOPE, OnChangeEditSlope)
	ON_BN_CLICKED(IDSEARCH, OnSearch)
	ON_LBN_SELCHANGE(IDC_LIST_COORD, OnSelchangeListCoord)
	ON_EN_CHANGE(IDC_EDIT_DH, OnChangeEditDh)
	ON_BN_CLICKED(IDGO, OnGo)
	ON_EN_CHANGE(IDC_EDIT_NUM, OnChangeEditNum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetSearchPointDlg message handlers

void CSetSearchPointDlg::OnChangeEditX() 
{
	UpdateData(TRUE);	
}

void CSetSearchPointDlg::OnChangeEditY() 
{
	UpdateData(TRUE);		
}

BOOL CSetSearchPointDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString	szInfo;
	char	chInfo[32];

	szInfo.Format("%.4lf",m_dX);
	strcpy(chInfo,szInfo);
	m_dX	= atof(chInfo);

	szInfo.Format("%.4lf",m_dY);
	strcpy(chInfo,szInfo);
	m_dY	= atof(chInfo);
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetSearchPointDlg::OnChangeEditPointpattern() 
{
	UpdateData(TRUE);	
}

void CSetSearchPointDlg::OnChangeEditSlope() 
{
	UpdateData(TRUE);
}

void CSetSearchPointDlg::OnSearch() 
{
	// TODO: Add your control notification handler code here
	if(m_pLocalTerrain != NULL)
	{
		//
		m_pCoord.ResetContent();

		//
		if(m_pLocalTerrain->OnSearchPoint(m_dPP,m_dMS,m_dDH,m_iNum))
		{	
			int	iNum;
			iNum	= m_pLocalTerrain->GetSelectNum();

			for(int i=0;i<iNum;i++)
			{
				double	dX,dY,dDH,dMaxDistance,dMinDistance;
				m_pLocalTerrain->GetSelectPoint(i,dX,dY,dDH,dMaxDistance,dMinDistance);
				
				CString	szInfo;
				szInfo.Format("%.2lf %.2lf %.2lf %.2lf %.2lf",dX,dY,dDH,dMaxDistance,dMinDistance);

				m_pCoord.AddString(szInfo);
			}
		}
	}
}

void CSetSearchPointDlg::OnSelchangeListCoord() 
{
	int	iItem;
	iItem	= m_pCoord.GetCurSel();

	if(iItem < 0)	return;

	if(m_pLocalTerrain != NULL)
	{
		double	dH;
		double	dMaxDistance,dMinDistance;
		if(m_pLocalTerrain->GetSelectPoint(iItem,m_dX,m_dY,dH,dMaxDistance,dMinDistance))
		{
			CString	szInfo;
			char	chInfo[32];

			szInfo.Format("%.4lf",m_dX);
			strcpy(chInfo,szInfo);
			m_dX	= atof(chInfo);

			szInfo.Format("%.4lf",m_dY);
			strcpy(chInfo,szInfo);
			m_dY	= atof(chInfo);	
			
			UpdateData(FALSE);
		}		
	}
}

void CSetSearchPointDlg::OnChangeEditDh() 
{
	UpdateData(TRUE);
}

void CSetSearchPointDlg::OnGo() 
{
	
	if(m_pWorkspace != NULL)
	{
		m_pWorkspace->SelectPoint(m_dX,m_dY);

		double	dLeft,dRight,dBottom,dTop;
		dLeft	= m_dX-m_dWidth/2;
		dRight	= dLeft + m_dWidth;
		dBottom	= m_dY-m_dHeight/2;
		dTop	= dBottom+ m_dHeight;
		
		static CRect	rect;
		rect.left	= (long)(dLeft+0.5);
		rect.right	= (long)(dRight+0.5);
		rect.bottom	= (long)(dBottom+0.5);
		rect.top	= (long)(dTop+0.5);

		if(m_pMainView != NULL)
			m_pMainView->SendMessage(WM_DEMCREATEFLASH,0,(LPARAM)&rect);
	}
}

void CSetSearchPointDlg::OnChangeEditNum() 
{
	UpdateData(TRUE);	
}
