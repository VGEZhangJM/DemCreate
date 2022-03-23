// SetDeltaHPerDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetDeltaHPerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetDeltaHPerDlg dialog


CSetDeltaHPerDlg::CSetDeltaHPerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDeltaHPerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetDeltaHPerDlg)
	m_bCheckSlope = FALSE;
	m_bCheckPattern = FALSE;
	m_bCheckDistance = FALSE;
	m_bCheckPingYuan = FALSE;
	m_bCheckQiuLing = FALSE;
	m_bCheckShanDi = FALSE;
	m_dMinSlope = 0.0;
	m_dMaxSlope = 0.0;
	m_dMinPattern = 0.0;
	m_dMaxPattern = 0.0;
	m_dMinDistance = 0.0;
	m_dMaxDistance = 0.0;
	m_dMinPingYuan = 0.0;
	m_dMaxPingYuan = 0.0;
	m_dMinQiuLing = 0.0;
	m_dMaxQiuLing = 0.0;
	m_dMinShanDi = 0.0;
	m_dMaxShanDi = 0.0;
	m_szIDWPathName = _T("");
	m_szSPDPathName = _T("");
	m_szMQFPathName = _T("");
	m_szMLFPathName = _T("");
	m_szTPSFPathName = _T("");
	m_szNCSFPathName = _T("");
	m_szEXPPathName = _T("");
	m_szLINEPathName = _T("");
	m_iIDWCount = 0;
	m_iSPDCount = 0;
	m_iMQFCount = 0;
	m_iMLFCount = 0;
	m_iTPSFCount = 0;
	m_iNCSFCount = 0;
	m_iEXPCount = 0;
	m_iLINECount = 0;
	//}}AFX_DATA_INIT
}


void CSetDeltaHPerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetDeltaHPerDlg)
	DDX_Check(pDX, IDC_CHECK_SLOPE, m_bCheckSlope);
	DDX_Check(pDX, IDC_CHECK_PATTERN, m_bCheckPattern);
	DDX_Check(pDX, IDC_CHECK_DISTANCE, m_bCheckDistance);
	DDX_Check(pDX, IDC_CHECK_PINGYUAN, m_bCheckPingYuan);
	DDX_Check(pDX, IDC_CHECK_QIULING, m_bCheckQiuLing);
	DDX_Check(pDX, IDC_CHECK_SHANDI, m_bCheckShanDi);
	DDX_Text(pDX, IDC_EDIT_MIN_SLOPE, m_dMinSlope);
	DDX_Text(pDX, IDC_EDIT_MAX_SLOPE, m_dMaxSlope);
	DDX_Text(pDX, IDC_EDIT_MIN_PATTERN, m_dMinPattern);
	DDX_Text(pDX, IDC_EDIT_MAX_PATTERN, m_dMaxPattern);
	DDX_Text(pDX, IDC_EDIT_MIN_DISTANCE, m_dMinDistance);
	DDX_Text(pDX, IDC_EDIT_MAX_DISTANCE, m_dMaxDistance);
	DDX_Text(pDX, IDC_EDIT_MIN_PINGYUAN, m_dMinPingYuan);
	DDX_Text(pDX, IDC_EDIT_MAX_PINGYUAN, m_dMaxPingYuan);
	DDX_Text(pDX, IDC_EDIT_MIN_QIULING, m_dMinQiuLing);
	DDX_Text(pDX, IDC_EDIT_MAX_QIULING, m_dMaxQiuLing);
	DDX_Text(pDX, IDC_EDIT_MIN_SHANDI, m_dMinShanDi);
	DDX_Text(pDX, IDC_EDIT_MAX_SHANDI, m_dMaxShanDi);
	DDX_Text(pDX, IDC_EDIT_IDW, m_szIDWPathName);
	DDX_Text(pDX, IDC_EDIT_SPD, m_szSPDPathName);
	DDX_Text(pDX, IDC_EDIT_MQF, m_szMQFPathName);
	DDX_Text(pDX, IDC_EDIT_MLF, m_szMLFPathName);
	DDX_Text(pDX, IDC_EDIT_TPSF, m_szTPSFPathName);
	DDX_Text(pDX, IDC_EDIT_NCSF, m_szNCSFPathName);
	DDX_Text(pDX, IDC_EDIT_EXP, m_szEXPPathName);
	DDX_Text(pDX, IDC_EDIT_LINE, m_szLINEPathName);
	DDX_Text(pDX, IDC_EDIT_COUNT_IDW, m_iIDWCount);
	DDX_Text(pDX, IDC_EDIT_COUNT_SPD, m_iSPDCount);
	DDX_Text(pDX, IDC_EDIT_COUNT_MQF, m_iMQFCount);
	DDX_Text(pDX, IDC_EDIT_COUNT_MLF, m_iMLFCount);
	DDX_Text(pDX, IDC_EDIT_COUNT_TPSF, m_iTPSFCount);
	DDX_Text(pDX, IDC_EDIT_COUNT_NCSF, m_iNCSFCount);
	DDX_Text(pDX, IDC_EDIT_COUNT_EXP, m_iEXPCount);
	DDX_Text(pDX, IDC_EDIT_COUNT_LINE, m_iLINECount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetDeltaHPerDlg, CDialog)
	//{{AFX_MSG_MAP(CSetDeltaHPerDlg)
	ON_BN_CLICKED(IDC_CHECK_SLOPE, OnCheckSlope)
	ON_EN_CHANGE(IDC_EDIT_MIN_SLOPE, OnChangeEditMinSlope)
	ON_EN_CHANGE(IDC_EDIT_MAX_SLOPE, OnChangeEditMaxSlope)
	ON_BN_CLICKED(IDC_CHECK_PATTERN, OnCheckPattern)
	ON_EN_CHANGE(IDC_EDIT_MIN_PATTERN, OnChangeEditMinPattern)
	ON_EN_CHANGE(IDC_EDIT_MAX_PATTERN, OnChangeEditMaxPattern)
	ON_BN_CLICKED(IDC_CHECK_DISTANCE, OnCheckDistance)
	ON_EN_CHANGE(IDC_EDIT_MIN_DISTANCE, OnChangeEditMinDistance)
	ON_EN_CHANGE(IDC_EDIT_MAX_DISTANCE, OnChangeEditMaxDistance)
	ON_BN_CLICKED(IDC_CHECK_PINGYUAN, OnCheckPingyuan)
	ON_EN_CHANGE(IDC_EDIT_MIN_PINGYUAN, OnChangeEditMinPingyuan)
	ON_EN_CHANGE(IDC_EDIT_MAX_PINGYUAN, OnChangeEditMaxPingyuan)
	ON_BN_CLICKED(IDC_CHECK_QIULING, OnCheckQiuling)
	ON_EN_CHANGE(IDC_EDIT_MIN_QIULING, OnChangeEditMinQiuling)
	ON_EN_CHANGE(IDC_EDIT_MAX_QIULING, OnChangeEditMaxQiuling)
	ON_BN_CLICKED(IDC_CHECK_SHANDI, OnCheckShandi)
	ON_EN_CHANGE(IDC_EDIT_MIN_SHANDI, OnChangeEditMinShandi)
	ON_EN_CHANGE(IDC_EDIT_MAX_SHANDI, OnChangeEditMaxShandi)
	ON_BN_CLICKED(IDC_BUTTON_IDW, OnButtonIdw)
	ON_EN_CHANGE(IDC_EDIT_IDW, OnChangeEditIdw)
	ON_BN_CLICKED(IDC_BUTTON_SPD, OnButtonSpd)
	ON_EN_CHANGE(IDC_EDIT_SPD, OnChangeEditSpd)
	ON_BN_CLICKED(IDC_BUTTON_MQF, OnButtonMqf)
	ON_EN_CHANGE(IDC_EDIT_MQF, OnChangeEditMqf)
	ON_BN_CLICKED(IDC_BUTTON_MLF, OnButtonMlf)
	ON_EN_CHANGE(IDC_EDIT_MLF, OnChangeEditMlf)
	ON_BN_CLICKED(IDC_BUTTON_TPSF, OnButtonTpsf)
	ON_EN_CHANGE(IDC_EDIT_TPSF, OnChangeEditTpsf)
	ON_BN_CLICKED(IDC_BUTTON_NCSF, OnButtonNcsf)
	ON_EN_CHANGE(IDC_EDIT_NCSF, OnChangeEditNcsf)
	ON_BN_CLICKED(IDC_BUTTON_EXP, OnButtonExp)
	ON_EN_CHANGE(IDC_EDIT_EXP, OnChangeEditExp)
	ON_BN_CLICKED(IDC_BUTTON_LINE, OnButtonLine)
	ON_EN_CHANGE(IDC_EDIT_LINE, OnChangeEditLine)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetDeltaHPerDlg message handlers

BOOL CSetDeltaHPerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//
	if(m_bCheckSlope)	{	GetDlgItem(IDC_EDIT_MIN_SLOPE)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_MAX_SLOPE)->EnableWindow(TRUE);		}
	else				{	GetDlgItem(IDC_EDIT_MIN_SLOPE)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_MAX_SLOPE)->EnableWindow(FALSE);	}

	//
	if(m_bCheckPattern)	{	GetDlgItem(IDC_EDIT_MIN_PATTERN)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_MAX_PATTERN)->EnableWindow(TRUE);		}
	else				{	GetDlgItem(IDC_EDIT_MIN_PATTERN)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_MAX_PATTERN)->EnableWindow(FALSE);	}

	//
	if(m_bCheckDistance){	GetDlgItem(IDC_EDIT_MIN_DISTANCE)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_MAX_DISTANCE)->EnableWindow(TRUE);		}
	else				{	GetDlgItem(IDC_EDIT_MIN_DISTANCE)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_MAX_DISTANCE)->EnableWindow(FALSE);	}
	
	//
	if(m_bCheckPingYuan){	GetDlgItem(IDC_EDIT_MIN_PINGYUAN)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_MAX_PINGYUAN)->EnableWindow(TRUE);		}
	else				{	GetDlgItem(IDC_EDIT_MIN_PINGYUAN)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_MAX_PINGYUAN)->EnableWindow(FALSE);	}

	//
	if(m_bCheckQiuLing)	{	GetDlgItem(IDC_EDIT_MIN_QIULING)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_MAX_QIULING)->EnableWindow(TRUE);		}
	else				{	GetDlgItem(IDC_EDIT_MIN_QIULING)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_MAX_QIULING)->EnableWindow(FALSE);	}

	//
	if(m_bCheckShanDi)	{	GetDlgItem(IDC_EDIT_MIN_SHANDI)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_MAX_SHANDI)->EnableWindow(TRUE);		}
	else				{	GetDlgItem(IDC_EDIT_MIN_SHANDI)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_MAX_SHANDI)->EnableWindow(FALSE);	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetDeltaHPerDlg::OnCheckSlope() 
{
	m_bCheckSlope	= 	!m_bCheckSlope;

	//
	if(m_bCheckSlope)	{	GetDlgItem(IDC_EDIT_MIN_SLOPE)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_MAX_SLOPE)->EnableWindow(TRUE);		}
	else				{	GetDlgItem(IDC_EDIT_MIN_SLOPE)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_MAX_SLOPE)->EnableWindow(FALSE);	}
}
	

void CSetDeltaHPerDlg::OnChangeEditMinSlope() 
{
	UpdateData(TRUE);
}

void CSetDeltaHPerDlg::OnChangeEditMaxSlope() 
{
	UpdateData(TRUE);
}

void CSetDeltaHPerDlg::OnCheckPattern() 
{
	m_bCheckPattern	= 	!m_bCheckPattern;

	//
	if(m_bCheckPattern)	{	GetDlgItem(IDC_EDIT_MIN_PATTERN)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_MAX_PATTERN)->EnableWindow(TRUE);		}
	else				{	GetDlgItem(IDC_EDIT_MIN_PATTERN)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_MAX_PATTERN)->EnableWindow(FALSE);	}
}

void CSetDeltaHPerDlg::OnChangeEditMinPattern() 
{
	UpdateData(TRUE);
}

void CSetDeltaHPerDlg::OnChangeEditMaxPattern() 
{
	UpdateData(TRUE);
}

void CSetDeltaHPerDlg::OnCheckDistance() 
{
	m_bCheckDistance	= 	!m_bCheckDistance;

	//
	if(m_bCheckDistance){	GetDlgItem(IDC_EDIT_MIN_DISTANCE)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_MAX_DISTANCE)->EnableWindow(TRUE);		}
	else				{	GetDlgItem(IDC_EDIT_MIN_DISTANCE)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_MAX_DISTANCE)->EnableWindow(FALSE);	}
}

void CSetDeltaHPerDlg::OnChangeEditMinDistance() 
{
	UpdateData(TRUE);
}

void CSetDeltaHPerDlg::OnChangeEditMaxDistance() 
{
	UpdateData(TRUE);
}

void CSetDeltaHPerDlg::OnCheckPingyuan() 
{
	m_bCheckPingYuan	= 	!m_bCheckPingYuan;

	//
	if(m_bCheckPingYuan){	GetDlgItem(IDC_EDIT_MIN_PINGYUAN)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_MAX_PINGYUAN)->EnableWindow(TRUE);		}
	else				{	GetDlgItem(IDC_EDIT_MIN_PINGYUAN)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_MAX_PINGYUAN)->EnableWindow(FALSE);	}
}

void CSetDeltaHPerDlg::OnChangeEditMinPingyuan() 
{
	UpdateData(TRUE);
}

void CSetDeltaHPerDlg::OnChangeEditMaxPingyuan() 
{
	UpdateData(TRUE);
}

void CSetDeltaHPerDlg::OnCheckQiuling() 
{
	m_bCheckQiuLing	= 	!m_bCheckQiuLing;

	//
	if(m_bCheckQiuLing)	{	GetDlgItem(IDC_EDIT_MIN_QIULING)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_MAX_QIULING)->EnableWindow(TRUE);		}
	else				{	GetDlgItem(IDC_EDIT_MIN_QIULING)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_MAX_QIULING)->EnableWindow(FALSE);	}
}

void CSetDeltaHPerDlg::OnChangeEditMinQiuling() 
{
	UpdateData(TRUE);
}

void CSetDeltaHPerDlg::OnChangeEditMaxQiuling() 
{
	UpdateData(TRUE);	
}

void CSetDeltaHPerDlg::OnCheckShandi() 
{
	m_bCheckShanDi	= 	!m_bCheckShanDi;

	//
	if(m_bCheckShanDi)	{	GetDlgItem(IDC_EDIT_MIN_SHANDI)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_MAX_SHANDI)->EnableWindow(TRUE);		}
	else				{	GetDlgItem(IDC_EDIT_MIN_SHANDI)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_MAX_SHANDI)->EnableWindow(FALSE);	}
}

void CSetDeltaHPerDlg::OnChangeEditMinShandi() 
{
	UpdateData(TRUE);
}

void CSetDeltaHPerDlg::OnChangeEditMaxShandi() 
{
	UpdateData(TRUE);
}

void CSetDeltaHPerDlg::OnButtonIdw() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"地形特征-残差组合数据文件(*.ltf)|*.ltf||");
	dlg.m_ofn.lpstrTitle="打开地形特征-残差组合数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szIDWPathName	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);			
}

void CSetDeltaHPerDlg::OnChangeEditIdw() 
{
	UpdateData(TRUE);
}

void CSetDeltaHPerDlg::OnButtonSpd() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"地形特征-残差组合数据文件(*.ltf)|*.ltf||");
	dlg.m_ofn.lpstrTitle="打开地形特征-残差组合数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szSPDPathName	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);			
}

void CSetDeltaHPerDlg::OnChangeEditSpd() 
{
	UpdateData(TRUE);
}

void CSetDeltaHPerDlg::OnButtonMqf() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"地形特征-残差组合数据文件(*.ltf)|*.ltf||");
	dlg.m_ofn.lpstrTitle="打开地形特征-残差组合数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szMQFPathName	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);			
}

void CSetDeltaHPerDlg::OnChangeEditMqf() 
{
	UpdateData(TRUE);
}

void CSetDeltaHPerDlg::OnButtonMlf() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"地形特征-残差组合数据文件(*.ltf)|*.ltf||");
	dlg.m_ofn.lpstrTitle="打开地形特征-残差组合数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szMLFPathName	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);				
}

void CSetDeltaHPerDlg::OnChangeEditMlf() 
{
	UpdateData(TRUE);
}

void CSetDeltaHPerDlg::OnButtonTpsf() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"地形特征-残差组合数据文件(*.ltf)|*.ltf||");
	dlg.m_ofn.lpstrTitle="打开地形特征-残差组合数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szTPSFPathName	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);				
}

void CSetDeltaHPerDlg::OnChangeEditTpsf() 
{
	UpdateData(TRUE);
}

void CSetDeltaHPerDlg::OnButtonNcsf() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"地形特征-残差组合数据文件(*.ltf)|*.ltf||");
	dlg.m_ofn.lpstrTitle="打开地形特征-残差组合数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szNCSFPathName	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);				
}

void CSetDeltaHPerDlg::OnChangeEditNcsf() 
{
	UpdateData(TRUE);
}

void CSetDeltaHPerDlg::OnButtonExp() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"地形特征-残差组合数据文件(*.ltf)|*.ltf||");
	dlg.m_ofn.lpstrTitle="打开地形特征-残差组合数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szEXPPathName	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);				
}

void CSetDeltaHPerDlg::OnChangeEditExp() 
{
	UpdateData(TRUE);
}

void CSetDeltaHPerDlg::OnButtonLine() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"地形特征-残差组合数据文件(*.ltf)|*.ltf||");
	dlg.m_ofn.lpstrTitle="打开地形特征-残差组合数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szLINEPathName	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);				
}

void CSetDeltaHPerDlg::OnChangeEditLine() 
{
	UpdateData(TRUE);
}

void CSetDeltaHPerDlg::OnButtonClear() 
{
	m_iIDWCount		= 0;
	m_iSPDCount		= 0;
	m_iMQFCount		= 0;
	m_iMLFCount		= 0;
	m_iTPSFCount	= 0;
	m_iNCSFCount	= 0;
	m_iEXPCount		= 0;
	m_iLINECount	= 0;
	
	UpdateData(FALSE);

	Invalidate(TRUE);
}

void CSetDeltaHPerDlg::OnButtonStart() 
{
	//
	AfxGetApp()->DoWaitCursor(1);

	//首先判断文件是否存在
	{
		{
			FILE	*fp;
			if(m_szIDWPathName.IsEmpty())
			{
				MessageBox("请先选择文件【IDW插值算法】!","提示信息",MB_OK);
				return;
			}
			
			fp	= fopen(m_szIDWPathName,"r");
			if(fp == NULL)
			{
				MessageBox("所选文件不存在【IDW插值算法】","提示信息",MB_OK);
				return;
			}
			fclose(fp);
		}

		{
			FILE	*fp;
			if(m_szSPDPathName.IsEmpty())	
			{
				MessageBox("请先选择文件【SPD插值算法】!","提示信息",MB_OK);
				return;
			}
			
			fp	= fopen(m_szSPDPathName,"r");
			if(fp == NULL)
			{
				MessageBox("所选文件不存在【SPD插值算法】!","提示信息",MB_OK);
				return;
			}
			fclose(fp);
		}

		{
			FILE	*fp;
			if(m_szMQFPathName.IsEmpty())
			{
				MessageBox("请先选择文件【MQF插值算法】!","提示信息",MB_OK);
				return;
			}
			
			fp	= fopen(m_szMQFPathName,"r");
			if(fp == NULL)
			{
				MessageBox("所选文件不存在【MQF插值算法】!","提示信息",MB_OK);
				return;
			}
			fclose(fp);
		}

		{
			FILE	*fp;
			if(m_szMLFPathName.IsEmpty())
			{
				MessageBox("请先选择文件【MLF插值算法】!","提示信息",MB_OK);
				return;
			}
			
			fp	= fopen(m_szMLFPathName,"r");
			if(fp == NULL)
			{
				MessageBox("所选文件不存在【MLF插值算法】!","提示信息",MB_OK);
				return;
			}
			fclose(fp);
		}

		{
			FILE	*fp;
			if(m_szTPSFPathName.IsEmpty())
			{
				MessageBox("请先选择文件【TPSF插值算法】!","提示信息",MB_OK);
				return;
			}
			
			fp	= fopen(m_szTPSFPathName,"r");
			if(fp == NULL)
			{
				MessageBox("所选文件不存在【TPSF插值算法】!","提示信息",MB_OK);
				return;
			}
			fclose(fp);
		}

		{
			FILE	*fp;
			if(m_szNCSFPathName.IsEmpty())
			{
				MessageBox("请先选择文件【NCSF插值算法】!","提示信息",MB_OK);
				return;
			}
			
			fp	= fopen(m_szNCSFPathName,"r");
			if(fp == NULL)
			{
				MessageBox("所选文件不存在【NCSF插值算法】!","提示信息",MB_OK);
				return;
			}
			fclose(fp);
		}

		{
			FILE	*fp;
			if(m_szEXPPathName.IsEmpty())
			{
				MessageBox("请先选择文件【EXP插值算法】!","提示信息",MB_OK);
				return;
			}

			fp	= fopen(m_szEXPPathName,"r");
			if(fp == NULL)
			{
				MessageBox("所选文件不存在【EXP插值算法】!","提示信息",MB_OK);
				return;
			}
			fclose(fp);
		}

		{
			FILE	*fp;
			if(m_szLINEPathName.IsEmpty())
			{
				MessageBox("请先选择文件【LINE插值算法】!","提示信息",MB_OK);
				return;
			}

			fp	= fopen(m_szLINEPathName,"r");
			if(fp == NULL)
			{
				MessageBox("所选文件不存在【LINE插值算法】!","提示信息",MB_OK);
				return;
			}
			fclose(fp);
		}
	}

	//开始判断
	int		iCount;
	iCount	= 0;
	{
		FILE	*fpIDW;
		FILE	*fpSPD;
		FILE	*fpMQF;
		FILE	*fpMLF;
		FILE	*fpTPSF;
		FILE	*fpNCSF;
		FILE	*fpEXP;
		FILE	*fpLINE;
		
		//
		fpIDW	= fopen(m_szIDWPathName,"r");
		//
		fpSPD	= fopen(m_szSPDPathName,"r");
		//
		fpMQF	= fopen(m_szMQFPathName,"r");
		//
		fpMLF	= fopen(m_szMLFPathName,"r");
		//
		fpTPSF	= fopen(m_szTPSFPathName,"r");
		//
		fpNCSF	= fopen(m_szNCSFPathName,"r");
		//
		fpEXP	= fopen(m_szEXPPathName,"r");
		//
		fpLINE	= fopen(m_szLINEPathName,"r");
		
		//
		double	dSlopeInIDW,dPatternInIDW,dDistanceInIDW,dDeltaHInIDW,dXInIDW,dYInIDW;
		double	dSlopeInSPD,dPatternInSPD,dDistanceInSPD,dDeltaHInSPD,dXInSPD,dYInSPD;
		double	dSlopeInMQF,dPatternInMQF,dDistanceInMQF,dDeltaHInMQF,dXInMQF,dYInMQF;
		double	dSlopeInMLF,dPatternInMLF,dDistanceInMLF,dDeltaHInMLF,dXInMLF,dYInMLF;
		double	dSlopeInTPSF,dPatternInTPSF,dDistanceInTPSF,dDeltaHInTPSF,dXInTPSF,dYInTPSF;
		double	dSlopeInNCSF,dPatternInNCSF,dDistanceInNCSF,dDeltaHInNCSF,dXInNCSF,dYInNCSF;
		double	dSlopeInEXP,dPatternInEXP,dDistanceInEXP,dDeltaHInEXP,dXInEXP,dYInEXP;
		double	dSlopeInLINE,dPatternInLINE,dDistanceInLINE,dDeltaHInLINE,dXInLINE,dYInLINE;
		
		//
		char	chName[128];
		fscanf(fpIDW,"%s%s%s%s%s%s",chName,chName,chName,chName,chName,chName);
		fscanf(fpSPD,"%s%s%s%s%s%s",chName,chName,chName,chName,chName,chName);
		fscanf(fpMQF,"%s%s%s%s%s%s",chName,chName,chName,chName,chName,chName);
		fscanf(fpMLF,"%s%s%s%s%s%s",chName,chName,chName,chName,chName,chName);
		fscanf(fpTPSF,"%s%s%s%s%s%s",chName,chName,chName,chName,chName,chName);
		fscanf(fpNCSF,"%s%s%s%s%s%s",chName,chName,chName,chName,chName,chName);
		fscanf(fpEXP,"%s%s%s%s%s%s",chName,chName,chName,chName,chName,chName);
		fscanf(fpLINE,"%s%s%s%s%s%s",chName,chName,chName,chName,chName,chName);

		//以IDW为基准,循环!
		while(fscanf(fpIDW,"%lf%lf%lf%lf%lf%lf",&dSlopeInIDW,&dPatternInIDW,&dDistanceInIDW,&dDeltaHInIDW,&dXInIDW,&dYInIDW) == 6)
		{
			//
			fscanf(fpSPD,"%lf%lf%lf%lf%lf%lf",&dSlopeInSPD,&dPatternInSPD,&dDistanceInSPD,&dDeltaHInSPD,&dXInSPD,&dYInSPD);
			//
			fscanf(fpMQF,"%lf%lf%lf%lf%lf%lf",&dSlopeInMQF,&dPatternInMQF,&dDistanceInMQF,&dDeltaHInMQF,&dXInMQF,&dYInMQF);
			//
			fscanf(fpMLF,"%lf%lf%lf%lf%lf%lf",&dSlopeInMLF,&dPatternInMLF,&dDistanceInMLF,&dDeltaHInMLF,&dXInMLF,&dYInMLF);

			//
			fscanf(fpTPSF,"%lf%lf%lf%lf%lf%lf",&dSlopeInTPSF,&dPatternInTPSF,&dDistanceInTPSF,&dDeltaHInTPSF,&dXInTPSF,&dYInTPSF);
			//
			fscanf(fpNCSF,"%lf%lf%lf%lf%lf%lf",&dSlopeInNCSF,&dPatternInNCSF,&dDistanceInNCSF,&dDeltaHInNCSF,&dXInNCSF,&dYInNCSF);
			//
			fscanf(fpEXP,"%lf%lf%lf%lf%lf%lf",&dSlopeInEXP,&dPatternInEXP,&dDistanceInEXP,&dDeltaHInEXP,&dXInEXP,&dYInEXP);
			//
			fscanf(fpLINE,"%lf%lf%lf%lf%lf%lf",&dSlopeInLINE,&dPatternInLINE,&dDistanceInLINE,&dDeltaHInLINE,&dXInLINE,&dYInLINE);
			
			bool bTheSameCoord;
			bTheSameCoord	= true;

			//判断坐标是否相等
			if(fabs(dXInIDW-dXInSPD)>1e-5 || fabs(dYInIDW-dYInSPD)>1e-5)
			{
				bTheSameCoord	= false;
			}

			if(fabs(dXInIDW-dXInMQF)>1e-5 || fabs(dYInIDW-dYInMQF)>1e-5)
			{
				bTheSameCoord	= false;
			}

			if(fabs(dXInIDW-dXInMLF)>1e-5 || fabs(dYInIDW-dYInMLF)>1e-5)
			{
				bTheSameCoord	= false;
			}

			if(fabs(dXInIDW-dXInTPSF)>1e-5 || fabs(dYInIDW-dYInTPSF)>1e-5)
			{
				bTheSameCoord	= false;
			}

			if(fabs(dXInIDW-dXInNCSF)>1e-5 || fabs(dYInIDW-dYInNCSF)>1e-5)
			{
				bTheSameCoord	= false;
			}
			
			if(fabs(dXInIDW-dXInEXP)>1e-5 || fabs(dYInIDW-dYInEXP)>1e-5)
			{
				bTheSameCoord	= false;
			}
								
			if(fabs(dXInIDW-dXInLINE)>1e-5 || fabs(dYInIDW-dYInLINE)>1e-5)
			{
				bTheSameCoord	= false;
			}
			
			//具有相同坐标
			if(bTheSameCoord)
			{
				//是否在条件允许的范围之内
				bool	bInSlopeCondition	= true;
			
				if(m_bCheckSlope) 
				{
					if(dSlopeInIDW<m_dMinSlope || dSlopeInIDW>m_dMaxSlope)
						bInSlopeCondition	= false;
				}

				bool	bInPatternCondition	= true;
				if(m_bCheckPattern) 
				{
					if(dPatternInIDW<m_dMinPattern || dPatternInIDW>m_dMaxPattern)
						bInPatternCondition	= false;
				}

				bool	bInDistanceCondition	= true;
				if(m_bCheckDistance) 
				{
					if(dDistanceInIDW<m_dMinDistance|| dDistanceInIDW>m_dMaxDistance)
						bInDistanceCondition	= false;
				}

				bool	bInPingYuanCondition	= true;
				if(m_bCheckPingYuan) 
				{
					//沿用Slope
					if(dSlopeInIDW<m_dMinPingYuan|| dSlopeInIDW>m_dMaxPingYuan)
						bInPingYuanCondition	= false;
				}

				bool	bInQiuLingCondition	= true;
				if(m_bCheckQiuLing) 
				{
					if(dPatternInIDW<m_dMinQiuLing|| dPatternInIDW>m_dMaxQiuLing)
						bInQiuLingCondition	= false;
				}

				bool	bInShanDiCondition	= true;
				if(m_bCheckShanDi) 
				{
					if(dDistanceInIDW<m_dMinShanDi|| dDistanceInIDW>m_dMaxShanDi)
						bInShanDiCondition	= false;
				}

				if(bInSlopeCondition && bInPatternCondition && bInDistanceCondition && bInPingYuanCondition && bInQiuLingCondition && bInShanDiCondition)
				{
					//总数
					iCount++;
					
					double	dDeltaH[8];
					dDeltaH[0]	= dDeltaHInIDW;
					dDeltaH[1]	= dDeltaHInSPD;
					dDeltaH[2]	= dDeltaHInMQF;
					dDeltaH[3]	= dDeltaHInMLF;
					dDeltaH[4]	= dDeltaHInTPSF;
					dDeltaH[5]	= dDeltaHInNCSF;
					dDeltaH[6]	= dDeltaHInEXP;
					dDeltaH[7]	= dDeltaHInLINE;
					
					//排序
					for(int I=0;I<8;I++)
					{
						for(int J=I+1;J<8;J++)
						{
							if(dDeltaH[I] > dDeltaH[J])
							{
								double	dTemp;
								dTemp		= dDeltaH[I];
								dDeltaH[I]	= dDeltaH[J];
								dDeltaH[J]	= dTemp;
							}
						}
					}

					//寻找最小残差
					double	dMinH;
					for(I=0;I<8;I++)
					{
						if(dDeltaH[I] != INVALID_VALUE)
						{
							dMinH	= dDeltaH[I];
							break;
						}
					}
						
					//检查哪些插值算法计算的残差值和最小值相等						
					if( fabs(dMinH - dDeltaHInIDW) < 1e-5)	m_iIDWCount++;
					if( fabs(dMinH - dDeltaHInSPD) < 1e-5)	m_iSPDCount++;
					if( fabs(dMinH - dDeltaHInMQF) < 1e-5)	m_iMQFCount++;
					if( fabs(dMinH - dDeltaHInMLF) < 1e-5)	m_iMLFCount++;
					if( fabs(dMinH - dDeltaHInTPSF) < 1e-5)	m_iTPSFCount++;
					if( fabs(dMinH - dDeltaHInNCSF) < 1e-5)	m_iNCSFCount++;
					if( fabs(dMinH - dDeltaHInEXP) < 1e-5)	m_iEXPCount++;
					if( fabs(dMinH - dDeltaHInLINE) < 1e-5)	m_iLINECount++;

					//UpdateData(FALSE);
					//OnDraw();
					{
						CClientDC	dc(this);
						DrawCount(&dc);
					}	

				}
			}	
		}

		fclose(fpIDW);
		fclose(fpSPD);
		fclose(fpMQF);
		fclose(fpMLF);
		fclose(fpTPSF);
		fclose(fpNCSF);
		fclose(fpEXP);
		fclose(fpLINE);
	}
	//
	AfxGetApp()->DoWaitCursor(-1);

	CString	szInfo;
	
	szInfo.Format("在当前条件限制下，总共比较了 %ld 个插值点",iCount);

	MessageBox(szInfo,"提示信息",MB_OK);
}

void CSetDeltaHPerDlg::DrawCount(CDC *pDC)
{
	CRect	rectClient;
	CWnd	*pWnd;

	//
	pDC->SetTextAlign(VTA_CENTER|TA_CENTER);
	
	///////////////////////////////////////////
	pWnd	= GetDlgItem(IDC_EDIT_COUNT_IDW);
	if(pWnd)
	{
		pWnd->GetClientRect(rectClient);
		pWnd->MapWindowPoints(this,rectClient);
			
		//
		CString	szInfo;
		szInfo.Format("%ld",m_iIDWCount);


		pDC->TextOut((rectClient.left+rectClient.right)/2,rectClient.top,szInfo);
	}
	///////////////////////////////////////////	

	///////////////////////////////////////////
	pWnd	= GetDlgItem(IDC_EDIT_COUNT_SPD);
	if(pWnd)
	{
		pWnd->GetClientRect(rectClient);
		pWnd->MapWindowPoints(this,rectClient);
			
		//
		CString	szInfo;
		szInfo.Format("%ld",m_iSPDCount);
		pDC->TextOut((rectClient.left+rectClient.right)/2,rectClient.top,szInfo);
	}
	///////////////////////////////////////////	

	///////////////////////////////////////////
	pWnd	= GetDlgItem(IDC_EDIT_COUNT_MQF);
	if(pWnd)
	{
		pWnd->GetClientRect(rectClient);
		pWnd->MapWindowPoints(this,rectClient);
			
		//
		CString	szInfo;
		szInfo.Format("%ld",m_iMQFCount);
		pDC->TextOut((rectClient.left+rectClient.right)/2,rectClient.top,szInfo);
	}
	///////////////////////////////////////////	

	///////////////////////////////////////////
	pWnd	= GetDlgItem(IDC_EDIT_COUNT_MLF);
	if(pWnd)
	{
		pWnd->GetClientRect(rectClient);
		pWnd->MapWindowPoints(this,rectClient);
			
		//
		CString	szInfo;
		szInfo.Format("%ld",m_iMLFCount);
		pDC->TextOut((rectClient.left+rectClient.right)/2,rectClient.top,szInfo);
	}
	///////////////////////////////////////////	

	///////////////////////////////////////////
	pWnd	= GetDlgItem(IDC_EDIT_COUNT_TPSF);
	if(pWnd)
	{
		pWnd->GetClientRect(rectClient);
		pWnd->MapWindowPoints(this,rectClient);
			
		//
		CString	szInfo;
		szInfo.Format("%ld",m_iTPSFCount);
		pDC->TextOut((rectClient.left+rectClient.right)/2,rectClient.top,szInfo);
	}
	///////////////////////////////////////////	

	///////////////////////////////////////////
	pWnd	= GetDlgItem(IDC_EDIT_COUNT_NCSF);
	if(pWnd)
	{
		pWnd->GetClientRect(rectClient);
		pWnd->MapWindowPoints(this,rectClient);
			
		//
		CString	szInfo;
		szInfo.Format("%ld",m_iNCSFCount);
		pDC->TextOut((rectClient.left+rectClient.right)/2,rectClient.top,szInfo);
	}
	///////////////////////////////////////////	

	///////////////////////////////////////////
	pWnd	= GetDlgItem(IDC_EDIT_COUNT_EXP);
	if(pWnd)
	{
		pWnd->GetClientRect(rectClient);
		pWnd->MapWindowPoints(this,rectClient);
			
		//
		CString	szInfo;
		szInfo.Format("%ld",m_iEXPCount);
		pDC->TextOut((rectClient.left+rectClient.right)/2,rectClient.top,szInfo);
	}
	///////////////////////////////////////////	

	///////////////////////////////////////////
	pWnd	= GetDlgItem(IDC_EDIT_COUNT_LINE);
	if(pWnd)
	{
		pWnd->GetClientRect(rectClient);
		pWnd->MapWindowPoints(this,rectClient);
			
		//
		CString	szInfo;
		szInfo.Format("%ld",m_iLINECount);
		pDC->TextOut((rectClient.left+rectClient.right)/2,rectClient.top,szInfo);
	}
	///////////////////////////////////////////	

}

void CSetDeltaHPerDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	DrawCount(&dc);

}
