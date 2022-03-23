// SetSlopeDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "SetSlopeDlg.h"

#include "matlib.h"

#include "DrawHistogram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetSlopeDlg dialog


CSetSlopeDlg::CSetSlopeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetSlopeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetSlopeDlg)
	//}}AFX_DATA_INIT

	iNum	= 0;
	pData	= NULL;

	//
	m_pImageList		= NULL;
}


void CSetSlopeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetSlopeDlg)
	DDX_Control(pDX, IDC_LIST_CTRL, m_pListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetSlopeDlg, CDialog)
	//{{AFX_MSG_MAP(CSetSlopeDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetSlopeDlg message handlers

BOOL CSetSlopeDlg::OnInitDialog() 
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
		TCHAR rgtsz[2][20] = {_T("�¶�ͳ����"), _T("ͳ��ֵ")};
		
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

	if(pData != NULL)
	{
		//����MatCom
		initM(MATCOM_VERSION);

		//��ͼ����ʾ	
		Mm maxtixX;

		maxtixX	= zeros(1,iNum);

		for(int i=0;i<iNum;i++)
		{	
			maxtixX.r(1+i)	= pData[i];
		}
		
		//
		CWnd	*pWnd=NULL;
		pWnd	= (CWnd*)GetDlgItem(IDC_STATIC_HIST);
		
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
		
		//����
		DrawHistogram(maxtixX);

		//�˳�MatCom
		exitM();	

	}
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//���ͳ������ֵ
void CSetSlopeDlg::FillStatContent()
{
	//�������
	m_pListCtrl.DeleteAllItems();

	//ͳ�ƶ��ٸ�ͳ���� 
	for(int iMainItem=0;iMainItem<9;iMainItem++)
	{
		LV_ITEM		lvitem;
		TCHAR		chBuffer[30];
		
		for(int iSubItem=0;iSubItem<2;iSubItem++)
		{
			lvitem.mask		= LVIF_TEXT | (iSubItem == 0 ? LVIF_IMAGE : 0);
			lvitem.iItem	= iMainItem;
			lvitem.iSubItem = iSubItem;
			lvitem.iImage	= 0;

			//��һ��
			if(iSubItem == 0)
			{
				if(iMainItem==0)	lvitem.pszText	= "��Сֵ";
				if(iMainItem==1)	lvitem.pszText	= "���ֵ";
				if(iMainItem==2)	lvitem.pszText	= "��ֵ";
				if(iMainItem==3)	lvitem.pszText	= "����";
				if(iMainItem==4)	lvitem.pszText	= "��׼��";
				if(iMainItem==5)	lvitem.pszText	= "����ϵ��";	
				if(iMainItem==6)	lvitem.pszText	= "ƫбϵ��";	
				if(iMainItem==7)	lvitem.pszText	= "���ϵ��";	
				if(iMainItem==8)	lvitem.pszText	= "�����";	
			}

			//�ڶ���
			if(iSubItem == 1)
			{
				if(iMainItem==0)	sprintf( chBuffer, "%.5lf", dMin);	
				if(iMainItem==1)	sprintf( chBuffer, "%.5lf", dMax);			
				if(iMainItem==2)	sprintf( chBuffer, "%.5lf", dMean);					
				if(iMainItem==3)	sprintf( chBuffer, "%.5lf", dVar);					
				if(iMainItem==4)	sprintf( chBuffer, "%.5lf", dStdVar);				
				if(iMainItem==5)	sprintf( chBuffer, "%.5lf", dCV);				
				if(iMainItem==6)	sprintf( chBuffer, "%.5lf", dSkewness);			
				if(iMainItem==7)	sprintf( chBuffer, "%.5lf", dKurtosis);				
				if(iMainItem==8)	sprintf( chBuffer, "%.5lf", dRMSE);				

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

void CSetSlopeDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(m_pImageList != NULL)
	{
		delete	m_pImageList;
		m_pImageList	= NULL;
	}	
	
}
