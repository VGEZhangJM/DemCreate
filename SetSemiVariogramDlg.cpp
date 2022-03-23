// SetSemiVariogramDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "SetSemiVariogramDlg.h"

#include "matlib.h"
#include "DrawSemiVariogram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetSemiVariogramDlg dialog


CSetSemiVariogramDlg::CSetSemiVariogramDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetSemiVariogramDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetSemiVariogramDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSetSemiVariogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetSemiVariogramDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetSemiVariogramDlg, CDialog)
	//{{AFX_MSG_MAP(CSetSemiVariogramDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetSemiVariogramDlg message handlers

BOOL CSetSemiVariogramDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//进入MatCom
	initM(MATCOM_VERSION);

	//用图表显示	
	Mm maxtixX,maxtixY;

	maxtixX	= zeros(1,m_iLagNum);
	maxtixY	= zeros(1,m_iLagNum);

	for(int i=0;i<m_iLagNum;i++)
	{	
		maxtixX.r(1+i)	= m_dLagDistance[i];
		maxtixY.r(1+i)	= m_dRPreLag[i];
	}
	
	//
	CWnd	*pWnd=NULL;
	pWnd	= (CWnd*)GetDlgItem(IDC_STATIC_PIC);
	
	Mm plothandle	= winaxes(pWnd->m_hWnd);

	Mm	len,isH;
	isH	= ishandle(plothandle);
	len	= length(isH);

	if( ((int)(len.r(1))) && ((int)(isH.r(1))) )
	{
		Mm		color;
		color	= zeros(1,3);
		color.r(1)	= 1;
		color.r(2)	= 1;
		color.r(3)	= 1;

		set(plothandle,TM("color"),color);
	}

	//绘制
	DrawSemiVariogram(maxtixX,maxtixY);

	//退出MatCom
	exitM();	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
