// SetStatDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "SetStatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetStatDlg dialog


CSetStatDlg::CSetStatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetStatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetStatDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//地理范围
	dLeft	= dRight	= dBottom	= dTop	= 0;
	//高程范围
	dMinH	= dMaxH		= 0;
	//样本均值
	dXMean	= dYMean	= dZMean	= 0;
	//样本方差
	dXVar	= dYVar		= dZVar		= 0;
	//样本标准方差
	dXStdVar= dYStdVar	= dZStdVar	= 0;
	//样本变异系数
	dZCV	= 0;

	//
	m_pImageList		= NULL;
}


void CSetStatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetStatDlg)
	DDX_Control(pDX, IDC_LIST_CTRL, m_pListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetStatDlg, CDialog)
	//{{AFX_MSG_MAP(CSetStatDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetStatDlg message handlers

BOOL CSetStatDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//////////////////////////////////////////////////////////////////////////
	/////////////				Insert Image Lists		//////////////////////
	//////////////////////////////////////////////////////////////////////////
	//{
		if(m_pImageList != NULL)
		{
			delete	m_pImageList;
			m_pImageList	= NULL;
		}
		m_pImageList = new CImageList();
		m_pImageList->Create( IDB_BACKGROUND, 16, 1, RGB(255,255,255) );
		m_pListCtrl.SetImageList( m_pImageList, LVSIL_SMALL );

		m_pListCtrl.SetTextColor(RGB(0,0,255));

		LV_COLUMN	lvcolumn;
		CRect		rect;
		m_pListCtrl.GetWindowRect( &rect );
		TCHAR rgtsz[2][20] = {_T("地形统计量"), _T("统计值")};
		
		int				i;

		for(i = 0; i < 2; i++)  
		{
			lvcolumn.mask		= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
			lvcolumn.fmt		= LVCFMT_LEFT;
			lvcolumn.pszText	= rgtsz[i];
			lvcolumn.iSubItem	= i;
			lvcolumn.iOrder		= i;

			if(i==0)	lvcolumn.cx	= 130;
			if(i==1)	lvcolumn.cx	= rect.Width()-130;

			m_pListCtrl.InsertColumn(i, &lvcolumn);  
		}	
	//}
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	
	FillStatContent();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//填充统计量的值
void CSetStatDlg::FillStatContent()
{
	//首先清空
	m_pListCtrl.DeleteAllItems();

	//统计多少个统计量 
	for(int iMainItem=0;iMainItem<8;iMainItem++)
	{
		LV_ITEM		lvitem;
		TCHAR		chBuffer[30];
		
		for(int iSubItem=0;iSubItem<2;iSubItem++)
		{
			lvitem.mask		= LVIF_TEXT | (iSubItem == 0 ? LVIF_IMAGE : 0);
			lvitem.iItem	= iMainItem;
			lvitem.iSubItem = iSubItem;
			lvitem.iImage	= 0;

			//第一列
			if(iSubItem == 0)
			{
				if(iMainItem==0)	lvitem.pszText	= "X方向范围";
				if(iMainItem==1)	lvitem.pszText	= "Y方向范围";
				if(iMainItem==2)	lvitem.pszText	= "最高高程值";
				if(iMainItem==3)	lvitem.pszText	= "最低高程值";
				if(iMainItem==4)	lvitem.pszText	= "样本均值(Z)";
				if(iMainItem==5)	lvitem.pszText	= "样本方差(Z)";
				if(iMainItem==6)	lvitem.pszText	= "样本标准方差(Z)";
				if(iMainItem==7)	lvitem.pszText	= "样本变异系数(Z)";	
			}

			//第二列
			if(iSubItem == 1)
			{
				if(iMainItem==0)	sprintf( chBuffer, "%.2lf-%.2lf", dLeft,dRight);	
				if(iMainItem==1)	sprintf( chBuffer, "%.2lf-%.2lf", dBottom,dTop);			
				if(iMainItem==2)	sprintf( chBuffer, "%.2lf", dMinH);					
				if(iMainItem==3)	sprintf( chBuffer, "%.2lf", dMaxH);					
				if(iMainItem==4)	sprintf( chBuffer, "%.2lf", dZMean);				
				if(iMainItem==5)	sprintf( chBuffer, "%.2lf", dZVar);				
				if(iMainItem==6)	sprintf( chBuffer, "%.2lf", dZStdVar);			
				if(iMainItem==7)	sprintf( chBuffer, "%.2lf", dZCV);				

				lvitem.pszText	= chBuffer;
			}

			if (iSubItem == 0)
				m_pListCtrl.InsertItem(&lvitem);
			else
				m_pListCtrl.SetItem(&lvitem);
		}
	}

	//UpdateData(FALSE);
}

void CSetStatDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	//
	if(m_pImageList != NULL)
	{
		delete	m_pImageList;
		m_pImageList	= NULL;
	}	
}
