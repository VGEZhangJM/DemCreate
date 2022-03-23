// SetRemainsErrorDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "SetRemainsErrorDlg.h"

#include "DCR_Statistics.h"
#include "matlib.h"

#include "DrawHistogram.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetRemainsErrorDlg dialog


CSetRemainsErrorDlg::CSetRemainsErrorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetRemainsErrorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetRemainsErrorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	iSampleCount	= 0;
	//
	dSampleX		= NULL;
	//
	dSampleY		= NULL;
	//
	dSampleZ		= NULL;
	//
	dSampleCZ		= NULL;
}


void CSetRemainsErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetRemainsErrorDlg)
	DDX_Control(pDX, IDC_LIST_STATS, m_pListStats);
	DDX_Control(pDX, IDC_LIST_CTRL, m_pListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetRemainsErrorDlg, CDialog)
	//{{AFX_MSG_MAP(CSetRemainsErrorDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetRemainsErrorDlg message handlers

BOOL CSetRemainsErrorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	double	*dRemainsError;
	dRemainsError	= new	double[iSampleCount];
	for(int i=0;i<iSampleCount;i++)
	{
		dRemainsError[i]	= dSampleZ[i]-dSampleCZ[i];
	}	
	
	CDCR_Statistics	pStatistics;
	//
	pStatistics.SetStatisticsData(iSampleCount,dRemainsError);
	pStatistics.Statistics();

	double	dMin,dMax;
	double	dMean;
	double	dVar;
	double	dStdVar;
	double	dCV;
	double	dSkewness;
	double	dKurtosis;
	double	dRMSE;
	int		iCount;

	pStatistics.GetDataCount(iCount);
	pStatistics.GetDataMinMax(dMin,dMax);
	pStatistics.GetDataMean(dMean);
	pStatistics.GetDataVar(dVar);
	pStatistics.GetDataStandardVar(dStdVar);
	pStatistics.GetDataCV(dCV);
	pStatistics.GetDataSkewness(dSkewness);
	pStatistics.GetDataKurtosis(dKurtosis);
	pStatistics.GetDataRMSE(dRMSE);
	
	//����
	pStatistics.Save(m_szPathName + "\\Lib\\Creator\\StaticsRemainsError.txt");
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//{
	{
		//m_pListCtrl.SetTextColor(RGB(0,0,255));

		LV_COLUMN	lvcolumn;
		CRect		rect;
		m_pListCtrl.GetWindowRect( &rect );
		TCHAR rgtsz[5][20] = {_T("���"), _T("Xֵ"), _T("Yֵ"), _T("Zֵ"), _T("�в�")};
		
		for(i = 0; i < 5; i++)  
		{
			lvcolumn.mask		= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
			lvcolumn.fmt		= LVCFMT_LEFT;
			lvcolumn.pszText	= rgtsz[i];
			lvcolumn.iSubItem	= i;
			lvcolumn.iOrder		= i;

			if(i==0)	lvcolumn.cx	= 60;
			if(i==1)	lvcolumn.cx	= (rect.Width()-60)/4;
			if(i==2)	lvcolumn.cx	= (rect.Width()-60)/4;
			if(i==3)	lvcolumn.cx	= (rect.Width()-60)/4;
			if(i==4)	lvcolumn.cx	= (rect.Width()-60)/4;

			m_pListCtrl.InsertColumn(i, &lvcolumn);  
		}	

		//�������
		m_pListCtrl.DeleteAllItems();

		//ͳ�ƶ��ٸ�ͳ���� 
		for(i=0;i<iSampleCount;i++)
		{
			LV_ITEM		lvitem;
			TCHAR		chBuffer[30];
			
			for(int iSubItem=0;iSubItem<5;iSubItem++)
			{
				lvitem.mask		= LVIF_TEXT;
				lvitem.iItem	= i;
				lvitem.iSubItem = iSubItem;
				lvitem.iImage	= 0;

				//��һ��
				if(iSubItem == 0)
				{
					sprintf(chBuffer,"%ld",i);
				}

				//�ڶ���
				if(iSubItem == 1)
				{
					sprintf(chBuffer,"%.5lf",dSampleX[i]);
				}

				//������
				if(iSubItem == 2)
				{
					sprintf(chBuffer,"%.5lf",dSampleY[i]);
				}

				//������
				if(iSubItem == 3)
				{
					sprintf(chBuffer,"%.5lf",dSampleZ[i]);
				}

				//������
				if(iSubItem == 4)
				{
					sprintf(chBuffer,"%.5lf",dRemainsError[i]);
				}

				lvitem.pszText	= chBuffer;

				if (iSubItem == 0)
					m_pListCtrl.InsertItem(&lvitem);
				else
					m_pListCtrl.SetItem(&lvitem);
			}
		}
	}
	//}
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//{
	{
		//m_pListStats.SetTextColor(RGB(0,0,255));

		LV_COLUMN	lvcolumn;
		CRect		rect;
		m_pListStats.GetWindowRect( &rect );
		TCHAR rgtsz[2][20] = {_T("ͳ����"), _T("ͳ��ֵ")};
		
		int				i;

		for(i = 0; i < 2; i++)  
		{
			lvcolumn.mask		= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
			lvcolumn.fmt		= LVCFMT_LEFT;
			lvcolumn.pszText	= rgtsz[i];
			lvcolumn.iSubItem	= i;
			lvcolumn.iOrder		= i;

			if(i==0)	lvcolumn.cx	= (rect.Width())/2;
			if(i==1)	lvcolumn.cx	= (rect.Width())/2;

			m_pListStats.InsertColumn(i, &lvcolumn);  
		}	

		//ͳ�ƶ��ٸ�ͳ���� 
		for(int iMainItem=0;iMainItem<10;iMainItem++)
		{
			LV_ITEM		lvitem;
			TCHAR		chBuffer[30];
			
			for(int iSubItem=0;iSubItem<2;iSubItem++)
			{
				lvitem.mask		= LVIF_TEXT;
				lvitem.iItem	= iMainItem;
				lvitem.iSubItem = iSubItem;
				lvitem.iImage	= 0;
				//��һ��
				if(iSubItem == 0)
				{
					if(iMainItem==0)	lvitem.pszText	= "��������";
					if(iMainItem==1)	lvitem.pszText	= "��Сֵ";
					if(iMainItem==2)	lvitem.pszText	= "���ֵ";
					if(iMainItem==3)	lvitem.pszText	= "������ֵ";
					if(iMainItem==4)	lvitem.pszText	= "��������";
					if(iMainItem==5)	lvitem.pszText	= "������׼����";
					if(iMainItem==6)	lvitem.pszText	= "��������ϵ��";	
					if(iMainItem==7)	lvitem.pszText	= "����ƫбϵ��";	
					if(iMainItem==8)	lvitem.pszText	= "�������ϵ��";	
					if(iMainItem==9)	lvitem.pszText	= "���������";	
				}

				//�ڶ���
				if(iSubItem == 1)
				{
					if(iMainItem==0)	sprintf( chBuffer, "%ld", iCount);	
					if(iMainItem==1)	sprintf( chBuffer, "%.5lf", dMin);	
					if(iMainItem==2)	sprintf( chBuffer, "%.5lf", dMax);			
					if(iMainItem==3)	sprintf( chBuffer, "%.5lf", dMean);					
					if(iMainItem==4)	sprintf( chBuffer, "%.5lf", dVar);					
					if(iMainItem==5)	sprintf( chBuffer, "%.5lf", dStdVar);				
					if(iMainItem==6)	sprintf( chBuffer, "%.5lf", dCV);				
					if(iMainItem==7)	sprintf( chBuffer, "%.5lf", dSkewness);			
					if(iMainItem==8)	sprintf( chBuffer, "%.5lf", dKurtosis);				
					if(iMainItem==9)	sprintf( chBuffer, "%.5lf", dRMSE);				

					lvitem.pszText	= chBuffer;
				}

				if (iSubItem == 0)
					m_pListStats.InsertItem(&lvitem);
				else
					m_pListStats.SetItem(&lvitem);
			}
		}
	}
	//}
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	
	//��ʾֱ��ͼ
	if(iSampleCount != 0)
	{
		//����MatCom
		initM(MATCOM_VERSION);

		//��ͼ����ʾ	
		Mm maxtixX;

		maxtixX	= zeros(1,iSampleCount);

		for(int i=0;i<iSampleCount;i++)
		{	
			maxtixX.r(1+i)	= dRemainsError[i];
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

		//����
		DrawHistogram(maxtixX);
		//DrawRemainsError(maxtixX);

		//�˳�MatCom
//		exitM();	
	}
	

	delete	[]dRemainsError;
	dRemainsError	= NULL;
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
